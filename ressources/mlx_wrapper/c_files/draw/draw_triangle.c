/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_triangle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 20:54:21 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 09:51:04 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

void	draw_tri(t_image *img, t_vec2 v[3], t_vec2f inv_slope, int clr)
{
	int		y;
	t_vec2f	x;

	if (is_flat_tri(v, 5))
		return ;
	y = v[1].y - 1;
	x = v2f(v[1].x);
	while (++y <= v[0].y)
	{
		draw_line(img, v2((int)x.x, y), \
			v2((int)x.y, y), v2(clr, 1));
		x = add_vec2f(x, inv_slope);
	}
	if (v[2].y - v[0].y != 0)
		inv_slope.x = (float)(v[2].x - v[0].x) / (v[2].y - v[0].y);
	x.x = v[0].x;
	y = v[0].y - 1;
	while (++y <= v[2].y)
	{
		draw_line(img, v2((int)x.x, y), \
			v2((int)x.y, y), v2(clr, 1));
		x = add_vec2f(x, inv_slope);
	}
}

void	draw_filled_triangle(t_image *img, t_vec2 v[3], int clr)
{
	t_vec2f	inv_slope;
	int		y;
	t_vec2f	x;

	if (v[0].y < v[1].y)
		swap_vec2(&v[1], &v[0]);
	if (v[2].y < v[1].y)
		swap_vec2(&v[1], &v[2]);
	if (v[2].y < v[0].y)
		swap_vec2(&v[0], &v[2]);
	inv_slope = v2f(0);
	if (v[0].y - v[1].y != 0)
		inv_slope.x = (float)(v[0].x - v[1].x) / (v[0].y - v[1].y);
	if (v[2].y - v[1].y != 0)
		inv_slope.y = (float)(v[2].x - v[1].x) / (v[2].y - v[1].y);
	draw_tri(img, v, inv_slope, clr);
}

void	shrink_quad(t_vec2 p[4], float amount, t_vec2 center)
{
	int		i;
	t_vec2	dir;
	float	len;

	i = -1;
	while (++i < 4)
	{
		dir = sub_vec2(p[i], center);
		len = sqrtf(dir.x * dir.x + dir.y * dir.y);
		if (len > 0.0f)
		{
			dir.x = dir.x / len * amount;
			dir.y = dir.y / len * amount;
		}
		p[i].x -= dir.x;
		p[i].y -= dir.y;
	}
}

void	draw_quad_contour(t_image *screen, t_vec2 p[4], int contour_clr)
{
	const t_vec2	ctr_d = v2(contour_clr, 2);

	draw_line(screen, p[0], p[1], ctr_d);
	draw_line(screen, p[1], p[2], ctr_d);
	draw_line(screen, p[2], p[3], ctr_d);
	draw_line(screen, p[3], p[0], ctr_d);
}

void	draw_quad(t_image *screen, t_vec2 p[4], int color, int contour_clr)
{
	const t_vec2	center = v2((p[0].x + p[2].x) / 2, \
	(p[0].y + p[2].y) / 2);
	t_vec2			tri[3];
	int				flat_diff;

	if (dist2(p[0], p[2]) <= dist2(p[1], p[3]))
	{
		flat_diff = abs(p[0].y - p[1].y - p[2].y - p[3].y);
		if (flat_diff < 3)
			return ;
		draw_filled_triangle(screen, (t_vec2[3]){p[0], p[1], p[2]}, color);
		draw_filled_triangle(screen, (t_vec2[3]){p[0], p[2], p[3]}, color);
	}
	else
	{
		flat_diff = abs(p[0].y - p[1].y - p[2].y - p[3].y);
		if (flat_diff < 3)
			return ;
		draw_filled_triangle(screen, (t_vec2[3]){p[1], p[2], p[3]}, color);
		draw_filled_triangle(screen, (t_vec2[3]){p[1], p[3], p[0]}, color);
	}
	if (contour_clr != -1)
		draw_quad_contour(screen, p, contour_clr);
}
