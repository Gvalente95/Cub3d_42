/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_ents.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 17:57:44 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/26 18:29:56 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	update_pk_frames(t_inventory *inv, t_ent *e)
{
	int	i;

	if (inv->active)
	{
		i = -1;
		while (inv->pokemon_team[++i])
		{
			if (inv->pokemon_team[i] != e)
				continue ;
			if (inv->hov_indexes[1] == i)
				inv->update_img = 1;
			else
				return ;
		}
	}
	e->frame_index++;
	if (!e->frames[e->frame_index])
		e->frame_index = 0;
	e->frame = e->frames[e->frame_index];
	return ;
}

void	update_ent_frame(t_ent *e)
{
	if (e->was_hit == 1)
	{
		if (e->hp <= 0)
			e->frame_index = 0;
		e->was_hit++;
	}
	else
		e->was_hit = 0;
	if (!e->anim[e->action][e->frame_index])
		e->frame_index = 0;
	else
		e->frame_index++;
	if (!e->anim[e->action][e->frame_index])
	{
		e->frame_index = 0;
		if (e->action == m_death)
		{
			e->is_active = 0;
			return ;
		}
	}
	e->frame = e->anim[e->action][e->frame_index];
	e->size = e->frame->size;
}

static int	update_wall(t_md *md, t_ent *e)
{
	t_image	*frame;
	int		i;
	float	interp_factor;
	int		grn_fct;
	int		red_fct;

	if (!md->timer.trig_fe)
		return (0);
	i = -1;
	while (++i < 4)
	{
		frame = e->frames[i];
		interp_factor = 1 - (float)e->crp_pxl.y / (frame->size.y - 1);
		grn_fct = 50 + (int)((255 - 50) * interp_factor);
		grn_fct = minmax(0, 255, grn_fct);
		red_fct = (int)((255) * interp_factor);
		red_fct = minmax(0, 255, red_fct / 3);
		interp_factor = 1 - (float)e->crp_pxl.x / (frame->size.y - 1);
		draw_pixel(frame, e->crp_pxl, v4_to_color(red_fct, grn_fct, 0, 255), 1);
		e->crp_pxl.x = minmax(0, frame->size.x - 1, \
			e->crp_pxl.x + r_range(-1, 1));
		e->crp_pxl.y = minmax(0, frame->size.y - 1, \
			e->crp_pxl.y + r_range(-1, 1));
	}
	return (1);
}

static int	update_ent(t_md *md, t_ent *e)
{
	e->in_screen = 0;
	if (e->type == nt_wall)
		return (update_wall(md, e));
	if (e->type == nt_door)
		return (1);
	if (!md->prm.ent_mode)
		return (1);
	if (md->timer.trig_anim)
	{
		if (e->type == nt_pokemon)
			update_pk_frames(&md->inv, e);
		else if (e->type == nt_mob)
			update_ent_frame(e);
	}
	if (e->type != nt_mob || !e->is_active)
		return (1);
	if (e->hp > 0)
		update_mob_actions(md, e);
	else
		e->action = m_death;
	e->in_screen = 0;
	return (1);
}

int	update_ents(t_md *md)
{
	t_ent	*e;
	t_dblst	*node;
	t_dblst	*next;
	int		upd_render;

	upd_render = 0;
	node = md->entities;
	while (node)
	{
		next = node->next;
		e = (t_ent *)node->content;
		if (e && e->type != nt_empty)
			upd_render += update_ent(md, e);
		node = next;
	}
	return (upd_render);
}
