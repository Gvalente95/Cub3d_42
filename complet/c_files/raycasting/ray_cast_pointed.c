/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_pointed.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 19:49:34 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/06 18:00:09 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	display_options(t_md *md, t_ent *e, const char *lbl, t_txtd txtd)
{
	rnd_fast_txt(md, txtd, e->label);
	txtd.color = _WHITE;
	txtd.y += md->prm.txt_sc;
	rnd_fast_txt(md, txtd, lbl);
	if (md->mouse.click != MOUSE_PRESS && md->mouse.pressed != MOUSE_PRESS)
		return (0);
	if (e->type == nt_item)
		collect_item(md, &md->inv, e);
	if (e->type == nt_pokemon)
		grab_item(md, &md->inv, Pokeball);
	if (e->type != nt_item)
		return (0);
	if (e->pckp_type != (t_pckp_types)nt_pokemon && \
	e->pckp_type != (t_pckp_types)Keys)
		return (0);
	grab_item(md, &md->inv, e->pckp_type);
	return (1);
}

void	show_pointed_data(t_md *md, t_vec2 p, t_ent *e)
{
	const t_vec2	sz = (t_vec2){md->prm.txt_sc * 10, md->prm.txt_sc * 2};
	const t_txtd	txt = (t_txtd){p.x, p.y, _BLUE, md->prm.txt_sc, md->screen};

	draw_pixels(md->screen, p, sz, set_alpha(_BLACK, .4));
	if (e->type == nt_pokemon)
	{
		rnd_fast_txt(md, txt, md->txd.pkmn_names[e->mob_type]);
		display_options(md, e, "Capture", txt);
	}
	else if (e->type == nt_item)
	{
		rnd_fast_txt(md, txt, md->txd.item_names[e->pckp_type]);
		display_options(md, e, "Take", txt);
	}
	else if (e->type == nt_mob)
		rnd_fast_txt(md, txt, md->txd.mob_names[e->mob_type]);
}

void	update_pointed(t_md *md, t_vec2 draw_p, t_vec3 sz_scale, t_ent *e)
{
	const t_vec2	sz = v2(sz_scale.x, sz_scale.y);
	const int		scale = sz_scale.z;
	const t_vec2	pkpos = md->inv.pkbl_p;
	int				is_pointed;

	is_pointed = 0;
	if (md->inv.held_i == -1)
		is_pointed = v2_bounds(draw_p, sub_vec2(div_v2(md->win_sz, 2), sz), sz);
	else if (e->type == nt_pokemon)
		is_pointed = v2_touch(draw_p, div_v2(sz, 2), pkpos, md->inv.held_sz);
	if (is_pointed)
		md->cam.prv_pointed_ent = e;
	if (md->cam.pointed_ent == e)
		md->cam.last_pointed_ent_pos = sub_vec2(draw_p, _v2(scale * .2));
}

void	update_pointed_ent(t_md *md)
{
	t_vec2			last_p;
	t_ent			*sel;
	const t_vec2	mouse_r = md->mouse.real;

	if (!md->cam.prv_pointed_ent)
	{
		md->cam.pointed_ent = NULL;
		return ;
	}
	sel = md->cam.pointed_ent;
	if (sel && sel->screen_p.x != -1)
		last_p = sub_vec2(sel->screen_p, scale_vec2(sel->screen_sz, .2f));
	else
		last_p = md->cam.last_pointed_ent_pos;
	if (md->prm.alternate_draw && md->timer.time % 2 != 0)
		return ;
	if (md->cam.pointed && !md->cam.pointed->in_screen)
		md->cam.pointed_ent = NULL;
	if (!md->inv.held_i && !v2_touch(last_p, mouse_r, _v2(2), _v2(md->t_len)))
		md->cam.pointed_ent = NULL;
	if (sel)
		md->cam.pointed_door = NULL;
	if (sel && md->inv.held_i == -1)
		show_pointed_data(md, last_p, sel);
}
