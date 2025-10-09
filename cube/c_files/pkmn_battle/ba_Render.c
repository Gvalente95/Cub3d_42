/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ba_Render.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:34:06 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/09 21:47:23 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	render_trans_screen(t_md *md, t_BA_d *bd, double elapsed)
{
	int				offst;
	int				i;
	t_vec2			s;
	const int		slh = md->win_sz.y / 50;
	const double	t = minf(1.0, elapsed / bd->trans_dur);

	bd->pk_basep[0].x = max(bd->pk_p[0].x, 0 + (bd->pk_p[0].x - 0) * t);
	bd->pk_basep[BME].x = \
		min(bd->pk_p[1].x, md->win_sz.x + (bd->pk_p[1].x - md->win_sz.x) * t);
	render_battle(md, bd);
	offst = minmax(0, md->win_sz.x, (elapsed * md->win_sz.x) / bd->trans_dur);
	s = (t_vec2){md->win_sz.x - offst, slh};
	i = -1;
	while (++i < 50)
	{
		if (i % 2 == 0)
			draw_pixels(md->screen, (t_vec2){0, slh * i}, s, _BLACK);
		else
			draw_pixels(md->screen, \
				(t_vec2){md->win_sz.x - s.x, slh * i}, s, _BLACK);
	}
	mlx_put_image_to_window(md->mlx, md->win, md->screen->img, 0, 0);
	reset_mlx_values(md);
}

void	render_BA_opponent(t_md *md)
{
	t_BA_d* 	bd = &md->BA_d;
	t_vec2			f_p;
	t_image			*img;
	int				clr;
	const t_vec2	sz = scale_vec2(bd->pk_sz, .6f);
	t_ent			* pok = bd->pk[BFOE];
	t_vec2			base_p = bd->pk_basep[BFOE];

	f_p = base_p;
	clr = -1;
	if (pok) {
		if (bd->stored_dealt[BFOE] > 0 && pok->hp && r_range(0, 5) == 0)
			f_p.x += r_range(-10, 10);
		if (bd->stored_dealt[BME] != 0)
			f_p = add_vec2(f_p, scale_vec2(sub_vec2(bd->pk_p[1], f_p), 0.1));
		if (!pok->hp || bd->bstate == BT_START)
			clr = _BLACK;
		else if (bd->stored_dealt[BFOE])
			clr = md->rgb[bd->stored_dealt[BFOE] < 0];
		else if (pok->pkStatus.active)
			clr = pok->pkStatus.color;
		img = copy_image(md, pok->frame, sz, clr);
	}
	else
		img = copy_image(md, bd->opponent->frame, sz, clr);
	draw_sphere(md->screen, \
		(t_vec2){f_p.x - sz.x * .1, f_p.y + sz.y * .7}, \
		(t_vec2){sz.x * 1.2, sz.y * .3}, (t_vec3){_BLACK, 5, 1});
	draw_img(img, md->screen, f_p, -1);
	free_image_data(md, img);
}

void	render_BA_me(t_md *md)
{
	t_BA_d		*bd = &md->BA_d;
	t_vec2			final_p;
	t_image			*img;
	int				clr;
	const t_vec2	sz = bd->pk_sz;
	t_ent* pok = bd->pk[BME];
	t_vec2	base_p = bd->pk_basep[BME];

	if (bd->bstate != BT_ON && bd->bstate <= BT_MYPKSET && !pok)
		pok = &md->plr;
	if (!pok) return;
	final_p = base_p;
	if (bd->stored_dealt[BME] > 0)
		final_p.x += r_range(-10, 10);
	if (bd->stored_dealt[BFOE] != 0)
		final_p = add_vec2(final_p, \
	scale_vec2(sub_vec2(bd->pk_p[!BME], final_p), 0.1));
	clr = -1;
	if (!pok->hp || bd->bstate == BT_START)
		clr = _BLACK;
	else if (bd->stored_dealt[BME])
		clr = md->rgb[bd->stored_dealt[BME] < 0];
	img = copy_image(md, pok->frame, sz, clr);
	flipx_image_data(img);
	draw_img(img, md->screen, final_p, -1);
	draw_sphere(md->screen, \
		(t_vec2){final_p.x, final_p.y + sz.y * .8}, \
		(t_vec2) {
		sz.x * 1.2, sz.y * .3
	}, (t_vec3) { _BLACK, 4, 1 });
	free_image_data(md, img);
}

void	refresh_battleground(t_md *md, t_BA_d *bd)
{
	const t_parameters	prv_prm = md->prm;
	const t_vec3f		prv_cam = md->cam.rot;
	const t_vec3f		prv_plr_p = md->plr.pos;
	const t_vec3f		prv_plr_dir = md->plr.dir;

	md->prm.alternate_draw = 0;
	md->prm.ray_mod = 0;
	md->plr.pos = (t_vec3f){ md->map.size.x / 2 * md->t_len, -md->t_len * 5, 0 };
	md->plr.dir.x = 90;
	md->plr.dir.y = 25;
	md->cam.rot.x = 90;
	md->cam.rot.y = 30;
	update_player(md, &md->plr);
	render_raw(md);
	draw_img(md->screen, bd->overlay, _v2(0), _WHITE);
	md->prm = prv_prm;
	md->plr.pos = prv_plr_p;
	md->plr.dir = prv_plr_dir;
	md->cam.rot = prv_cam;
}

void	render_BA_debug(t_md* md, t_BA_d* bd) {
	const char* TransNames[11] = { "BT_ON", "BT_START",
		"BT_OPPKSET", "BT_MYPKSET", "BT_WAIT",
		"BT_SWITCH_TURN", "BT_END", "BT_WON",
		"BT_REWARD", "BT_PUNITION", "BT_QUIT" };
	const char* actionNames[7] = { "BA_NO_ACTION", "BA_MOVE_SELF",
		"BA_MOVE_USED", "BA_ITEM_USED",
		"BA_MOVE_MISSED", "BA_STATUS_EFFECT"};
	t_ent *opPk = bd->pk[BFOE] ? bd->pk[BFOE] : bd->opponent;
	t_ent *myPk = bd->pk[BME];
	rnd_fast_txt(md, (t_txtd) { 50, 40, -1, -1, md->screen }, "%s", TransNames[bd->bstate]);
	rnd_fast_txt(md, (t_txtd) { 50, 60, -1, -1, md->screen }, "%s", actionNames[bd->action_type]);
	if (opPk) rnd_fast_txt(md, (t_txtd) { 50, 80, -1, -1, md->screen }, "opPk: %s st: \"%s\" ", opPk->label, opPk->pkStatus.name);
	if (myPk) rnd_fast_txt(md, (t_txtd) { 50, 100, -1, -1, md->screen }, "myPk: %s st: \"%s\" ", myPk->label, myPk->pkStatus.name);
	rnd_fast_txt(md, (t_txtd) { 50, 120, -1, -1, md->screen }, "Turn: %s", bd->my_turn == BME ? "My Turn" : "Opponent Turn");
}

void	render_battle(t_md* md, t_BA_d* bd)
{
	draw_img(bd->overlay, md->screen, _v2(0), -1);
	render_BA_opponent(md);
	render_BA_me(md);
	render_BA_hud(md, bd);
	if (1) render_BA_debug(md, bd);
	mlx_put_image_to_window(md->mlx, md->win, md->screen->img, 0, 0);
}
