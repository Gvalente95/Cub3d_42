/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:37:26 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/01 14:09:21 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	start_battle(t_md *md, t_battle_d *bd, t_ent *ennemy, t_ent *ennemy_pok)
{
	if (!ennemy_pok)
		return ;
	bd->active = 1;
	bd->trainer = ennemy;
	bd->pk[0] = ennemy_pok;
	bd->sub_i = -1;
	bd->pk[1] = get_valid_pkmn(md->inv.pokemon_team, md->inv.team_size);
	bd->stored_dealt[0] = 0;
	bd->stored_dealt[1] = 0;
	refresh_battleground(md, bd);
	bd->pk_basep[0] = (t_vec2){-bd->pk_sz.x, bd->pk_p[0].y};
	bd->pk_basep[1] = (t_vec2){md->win_sz.x, bd->pk_p[1].y};
	bd->my_turn = 1;
	bd->opt_i = 0;
	bd->in_sub = 0;
	bd->trans_lvl = 1;
	bd->quitting = 0;
	bd->trans_start = md->timer.cur_tm;
	stop_sound(md->au.mus_pid);
	if (ennemy)
		update_battle_text(bd, "%s wants a challenge!", ennemy->label);
	else
		update_battle_text(bd, "A fow %s wants to fight", ennemy_pok->label);
	refresh_battle_screen(md, bd);
}

void	get_bt_butn(t_md *md, t_vec2 sz, t_vec2 p, const char *label)
{
	int				txt_scl;
	t_vec2			txt_p;
	const int		l = 2;
	t_battle_d		*bd;
	t_image			*img;

	img = init_img(md, sz, NULL, -1);
	flush_gradient(img, _YELLOW, _ORANGE, 10);
	bd = &md->battle_d;
	draw_pixels(img, _v2(0), (t_vec2){sz.x - l, l}, _BLACK);
	draw_pixels(img, (t_vec2){0, sz.y - l}, (t_vec2){sz.x - l, l}, _BLACK);
	draw_pixels(img, _v2(0), (t_vec2){l, sz.y}, _BLACK);
	draw_pixels(img, (t_vec2){sz.x - l, 0}, (t_vec2){l, sz.y}, _BLACK);
	txt_p = div_v2(sz, 2);
	txt_scl = md->prm.txt_sc;
	txt_p.y -= txt_scl / 2;
	txt_p.x -= (txt_scl * ft_strlen(label)) / 2;
	rnd_fast_txt(md, (t_txtd){txt_p.x, txt_p.y, -1, txt_scl, img}, label);
	bd->buttons[bd->but_i] = img;
	bd->but_ps[bd->but_i++] = p;
}

void	draw_hp_bar(t_md *md, t_ent *e, t_vec2 pos, t_vec2 sz)
{
	t_image			*img;
	t_image			*bgr;
	t_image			*inside;
	const t_vec2	brd_sz = v2(2, 2);
	int				hp_prc;

	hp_prc = (e->hp * sz.x) / e->max_hp;
	bgr = md->screen;
	if (!md->battle_d.active)
		bgr = md->inv.img;
	img = init_img(md, sz, NULL, _BLACK);
	inside = init_img(md, sub_vec2(sz, _v2(4)), NULL, -1);
	flush_gradient(inside, _GREEN, v4_to_color(20, 20, 20, 20), 4);
	draw_img(inside, img, brd_sz, -1);
	free_image_data(md, inside);
	draw_pixels(img, \
		v2(brd_sz.x + hp_prc, brd_sz.y), \
		v2(sz.x - hp_prc - brd_sz.x * 2, sz.y - brd_sz.y * 2), \
		v4_to_color(200, 20, 80, 255));
	rnd_fast_txt(md, (t_txtd){sz.x * .3, sz.y * .3, -1, -1, img}, \
		"%d/%d", e->hp, e->max_hp);
	draw_img(img, bgr, pos, -1);
	free_image_data(md, img);
}

void	draw_team_slot(t_md *md, t_battle_d *bd, t_vec2 p, int i)
{
	t_image		*img;
	t_vec2		sz;
	t_vec3		clr_d;

	clr_d = (t_vec3){_BLACK, 3, 1};
	sz = bd->pk_slotsz;
	if (bd->opt_i == i)
	{
		clr_d.x = _RED;
		scale_vec2(sz, 2);
	}
	rnd_fast_txt(md, (t_txtd){p.x, p.y, -1, -1, md->screen}, "%d", i);
	draw_sphere(md->screen, \
		(t_vec2){p.x, p.y + sz.y * .8}, div_v2(sz, 2), clr_d);
	if (md->inv.pokemon_team[i])
	{
		if (!md->inv.pokemon_team[i]->hp)
			img = copy_image(md, md->inv.pokemon_team[i]->frame, sz, _BLACK);
		else
			img = copy_image(md, md->inv.pokemon_team[i]->frame, sz, -1);
		draw_img(img, md->screen, p, -1);
		free_image_data(md, img);
	}
}

void	init_battle_data(t_md *md, t_battle_d *bd)
{
	const t_vec2	win_sz = md->win_sz;
	const t_vec2	but_sz = (t_vec2){md->prm.txt_sc * 10, md->prm.txt_sc * 2};
	t_vec2			ps;

	bd->pk_sz = _v2(win_sz.x * .23);
	bd->pk_p[0] = v2(win_sz.x * .8 - bd->pk_sz.x, win_sz.y * .2);
	bd->pk_p[1] = v2(bd->pk_sz.x / 2, win_sz.y * .8 - bd->pk_sz.y);
	bd->trans_dur = 1;
	bd->overlay = init_img(md, win_sz, NULL, _WHITE);
	bd->action_dur = 1;
	bd->pk_slotsz = div_v2(win_sz, 6);
	bd->but_sz = but_sz;
	ps = add_vec2(bd->pk_p[1], bd->pk_sz);
	ps.y -= bd->pk_sz.y / 2;
	bd->but_i = 0;
	get_bt_butn(md, but_sz, ps, "ATTACK");
	get_bt_butn(md, but_sz, (t_vec2){ps.x + 5, ps.y + but_sz.y * 1.1}, "BAG");
	get_bt_butn(md, but_sz, (t_vec2){ps.x + 10, ps.y + but_sz.y * 2.2}, "TEAM");
	get_bt_butn(md, but_sz, (t_vec2){ps.x + 15, ps.y + but_sz.y * 3.3}, "EXIT");
	bd->active = 0;
}
