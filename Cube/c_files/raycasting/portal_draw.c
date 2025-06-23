/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal_draw.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:09:50 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 09:51:04 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	set_portal_pos(t_ent *e, int x_pos_offset, t_vec2 *out_pos)
{
	t_wrd_dir	dir;

	dir = e->overlay_dir;
	if (dir == NORTH)
	{
		out_pos->y = e->pos.y + e->size.y;
		out_pos->x = e->pos.x + x_pos_offset;
	}
	else if (dir == SOUTH)
	{
		out_pos->y = e->pos.y;
		out_pos->x = e->pos.x + x_pos_offset;
	}
	else if (dir == EAST)
	{
		out_pos->x = e->pos.x;
		out_pos->y = e->pos.y + x_pos_offset;
	}
	else
	{
		out_pos->x = e->pos.y + e->size.y;
		out_pos->y = e->pos.y + x_pos_offset;
	}
}

void	draw_portal(t_md *md, t_ent *e, t_vec2 pos)
{
	t_vec2	draw_sz;
	t_vec2	draw_p;
	t_vec2	out_pos;

	if (md->portal.found || e->overlay || e->type != nt_wall)
		return ;
	(void)pos;
	play_sound(md, AU_PORTAL_SHOOT);
	draw_sz = v2(md->t_len / 3, md->t_len * .75);
	draw_p = v2(e->size.x / 2 - draw_sz.x / 2, \
		md->t_len * .25);
	e->overlay = copy_image(md, e->frame, e->frame->size, -1);
	set_portal_pos(e, e->size.x / 2, &out_pos);
	draw_pixels(e->overlay, draw_p, draw_sz, _BLUE);
	md->portal.found = e;
	md->portal.out_pos = out_pos;
}

void	update_wall_pointed(t_md *md, t_ent *e, t_vec2 draw_limits)
{
	const t_vec2	center = div_v2(md->win_sz, 2);
	const int		draw_start = draw_limits.x;
	const int		draw_end = draw_limits.y;
	t_cam			*cam;

	if (center.y < draw_start || center.y > draw_end)
		return ;
	cam = &md->cam;
	cam->prv_pointed = e;
	if (e->type == nt_door)
	{
		if (e->was_hit)
			cam->pointed_door = cam->prv_pointed;
		else
		{
			if (!cam->pointed_door || !cam->prv_door)
				cam->pointed_door = cam->prv_pointed;
			if (cam->prv_door == cam->pointed_door && cam->pointed_door->hp)
				cam->pointed_door = cam->prv_pointed;
		}
	}
}
