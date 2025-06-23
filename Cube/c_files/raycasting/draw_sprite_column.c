/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprite_column.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 04:30:37 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/23 16:44:18 by gvalente         ###   ########.fr       */
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
