/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_triangle_tools.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 20:58:14 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/06 18:00:32 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

int	is_in_shape(t_vec2 z, t_vec2 tri[][3], int amount)
{
	int		i;
	t_vec2	a[3];
	t_vec3	d;
	int		has_neg;
	int		has_pos;

	i = -1;
	while (++i < amount)
	{
		a[0] = tri[i][0];
		a[1] = tri[i][1];
		a[2] = tri[i][2];
		d.x = (z.x - a[1].x) * (a[0].y - a[1].y) - \
	(a[0].x - a[1].x) * (z.y - a[1].y);
		d.y = (z.x - a[2].x) * (a[1].y - a[2].y) - \
	(a[1].x - a[2].x) * (z.y - a[2].y);
		d.z = (z.x - a[0].x) * (a[2].y - a[0].y) - \
	(a[2].x - a[0].x) * (z.y - a[0].y);
		has_neg = ((d.x < 0) || (d.y < 0) || (d.z < 0));
		has_pos = ((d.x > 0) || (d.y > 0) || (d.z > 0));
		if (!(has_neg && has_pos))
			return (1);
	}
	return (0);
}

float	dist2(t_vec2 a, t_vec2 b)
{
	const float	dx = b.x - a.x;
	const float	dy = b.y - a.y;

	return (dx * dx + dy * dy);
}

int	is_flat_tri(t_vec2 v[3], int treshold)
{
	int	min_y;
	int	max_y;

	min_y = v[0].y;
	max_y = v[0].y;
	if (v[1].y < min_y)
		min_y = v[1].y;
	if (v[2].y < min_y)
		min_y = v[2].y;
	if (v[1].y > max_y)
		max_y = v[1].y;
	if (v[2].y > max_y)
		max_y = v[2].y;
	return (max_y - min_y < treshold);
}
