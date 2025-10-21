/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ba_Render.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:34:06 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/10 12:42:41 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

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
	const char* TransNames[11] = { "BT ON", "BT START",
		"BT OPPKSET", "BT MYPKSET", "BT WAIT",
		"BT SWITCH TURN", "BT END", "BT WON",
		"BT REWARD", "BT PUNITION", "BT QUIT" };
	const char* actionNames[7] = { "BA NO ACTION", "BA MOVE SELF",
		"BA MOVE USED", "BA ITEM USED",
		"BA MOVE MISSED", "BA STATUS EFFECT"};
	t_ent *opPk = bd->pk[BFOE] ? bd->pk[BFOE] : bd->opponent;
	t_ent *myPk = bd->pk[BME];
	rnd_fast_txt(md, (t_txtd) { 50, 40, -1, -1, md->screen }, "%s", TransNames[bd->bstate]);
	rnd_fast_txt(md, (t_txtd) { 50, 60, -1, -1, md->screen }, "%s", actionNames[bd->action_type]);
	if (opPk) rnd_fast_txt(md, (t_txtd) { 50, 80, -1, -1, md->screen }, "opPk: %s st: \"%s\" ", opPk->label, opPk->pkStatus.name);
	if (myPk) rnd_fast_txt(md, (t_txtd) { 50, 100, -1, -1, md->screen }, "myPk: %s st: \"%s\" ", myPk->label, myPk->pkStatus.name);
	rnd_fast_txt(md, (t_txtd) { 50, 120, -1, -1, md->screen }, "Turn: %s", bd->turn == BME ? "My Turn" : "Opponent Turn");
}

void	render_BA_opponent(t_md* md)
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

void	render_battle(t_md* md, t_BA_d* bd)
{
	draw_img(bd->overlay, md->screen, _v2(0), -1);
	render_BA_opponent(md);
	render_BA_me(md);
	render_BA_hud(md, bd);
	if (1) render_BA_debug(md, bd);
	mlx_put_image_to_window(md->mlx, md->win, md->screen->img, 0, 0);
}
