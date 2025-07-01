/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:44:12 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/07/01 14:41:41 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	solve_collision(t_ent *a, t_ent *b, t_vec2 a_size)
{
	float	abs_x;
	float	abs_y;

	a->col_hit = b;
	abs_x = fabsf((a->pos.x + a_size.x / 2 + a->mov.x) - \
					(b->pos.x + b->size.x / 2));
	abs_y = fabsf((a->pos.y + a_size.y / 2 + a->mov.y) - \
					(b->pos.y + b->size.y / 2));
	if (abs_y > abs_x)
	{
		a->mov.y = 0;
		if (a->pos.y < b->pos.y)
			a->pos.y = b->pos.y - a_size.y;
		else
			a->pos.y = b->pos.y + b->size.y;
	}
	else if (abs_x > abs_y)
	{
		a->mov.x = 0;
		if (a->pos.x < b->pos.x)
			a->pos.x = b->pos.x - a_size.x;
		else
			a->pos.x = b->pos.x + b->size.x;
	}
	return (1);
}

int	is_collision(t_md *md, t_ent *a, t_ent *b, t_vec2 a_size)
{
	const int	is_col = (!(a->pos.y + a->mov.y + a_size.y < b->pos.y || \
		a->pos.y + a->mov.y > b->pos.y + b->size.y || \
		a->pos.x + a->mov.x + a_size.x < b->pos.x || \
		a->pos.x + a->mov.x > b->pos.x + b->size.x));

	if (!is_col)
		return (0);
	if (a->coord.z == b->coord.z - 1)
	{
		if (fabsf(a->pos.z - (b->coord.z - 1) * md->t_len) < 10 && \
			a->mov.z >= 0)
		{
			a->grounded = 1;
			if (md->key_click != SPACE_KEY)
				a->mov.z = 0;
			a->on_floor = b->coord.z - 1;
		}
		return (0);
	}
	else if (b->coord.z != a->coord.z)
		return (0);
	return (1);
}

static int	validate_collision(t_md *md, t_ent *a, t_ent *b, t_vec2 a_size)
{
	const int	btp = b->type;
	const int	is_w = (btp == nt_wall || btp == nt_door || btp == nt_ext_wall);

	if (!b || !b->is_active || !is_w)
		return (0);
	if (!is_collision(md, a, b, a_size))
		return (0);
	if (btp == nt_wall && b->overlay)
		return (validate_portal_collision(md, b));
	return (1);
}

int	set_collisions(t_md *md, t_ent *e, t_vec2 e_size)
{
	const t_vec2	neighbors[] = {{-1, -1}, {0, -1}, {1, -1}, \
									{-1, 0}, {0, 0}, {1, 0}, \
									{-1, 1}, {0, 1}, {1, 1}};
	t_vec2			cord;
	t_ent			*col;
	int				i;
	int				col_amount;

	if (md->prm.fly_cam)
		return (1);
	e->on_floor = 0;
	col_amount = 0;
	e->col_hit = NULL;
	i = -1;
	while (++i < 9)
	{
		cord = v2(e->coord.x + neighbors[i].x, e->coord.y + neighbors[i].y);
		col = get_mapped_at_cord(md, cord);
		if (col && validate_collision(md, e, col, e_size))
			col_amount += solve_collision(e, col, e_size);
	}
	return (col_amount);
}
