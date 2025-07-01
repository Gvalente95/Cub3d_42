/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprite_column.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 04:30:37 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/07/01 04:42:00 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	paint_ent(t_md *md, t_ent *e, t_vec2 txtr_coord)
{
	int			action;
	int			frame_i;
	int			dmg;

	if (e->type != nt_mob)
		return ;
	dmg = (1 + (txtr_coord.y < e->frame->size.y / 2));
	txtr_coord = v2(\
		e->frame->size.x / 2 + r_range(-5, 5), \
		r_range(20, e->frame->size.y - 20));
	action = -1;
	while (++action < ENT_ACTION_LEN)
	{
		if (action == m_death)
			txtr_coord.y += e->frame->size.y * .5;
		frame_i = -1;
		while (e->anim[action][++frame_i])
			draw_blood(md, e->anim[action][frame_i], txtr_coord, _RED);
	}
	e->was_hit = 1;
	e->hp -= dmg;
	if (!e->hp)
		e->frame_index = 0;
}

void	draw_wall_shadow(t_md *md, t_ray_draw_d *d, int prev_start)
{
	const int	yplus = prev_start + d->win_start.y - md->cam.pos.z;
	const int	start_y = yplus + d->wp.z;
	int			y;

	if (start_y < 0 || start_y > md->win_sz.y || d->ray->dirty_checks[start_y])
		return ;
	draw_pixel(md->screen, v2(d->ray->index, start_y), _BLACK, .1);
	d->ray->dirty_checks[start_y] = 1;
	if (d->ray->is_double_hit)
	{
		y = start_y + 1;
		while (!d->ray->dirty_checks[y] && y < md->win_sz.y)
			draw_pixel(md->screen, v2(d->ray->index, y++), _BLACK, .1);
	}
}
