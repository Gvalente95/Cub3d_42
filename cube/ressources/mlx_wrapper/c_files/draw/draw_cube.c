/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_cube.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 00:03:18 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/23 18:10:07 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

void	init_cube_draw_data(t_cube_drawd *d, t_vec3 pos, \
	t_vec2 size, t_vec3f rot)
{
	const t_vec3f	c = {size.x / 2.0f, size.y / 2.0f, -size.y / 2.0f};
	const int		tmp_faces[6][4] = {{0, 2, 6, 4}, {0, 1, 3, 2}, \
	{4, 5, 7, 6}, {0, 1, 5, 4}, {2, 3, 7, 6}, {1, 3, 7, 5}};
	const t_vec3f	tmp_pts[8] = {{-c.x, -c.y, c.z}, {c.x, -c.y, c.z}, \
	{-c.x, c.y, c.z}, {c.x, c.y, c.z}, {-c.x, -c.y, -c.z}, {c.x, -c.y, -c.z}, \
	{-c.x, c.y, -c.z}, {c.x, c.y, -c.z}};
	t_vec2			ij;

	d->angle = scale_vec3f(rot, (_PI / 180.0f));
	ij.x = -1;
	while (++ij.x < 8)
	{
		d->cube_pts[ij.x] = tmp_pts[ij.x];
		d->projected[ij.x] = project(rotate_vec3f(d->cube_pts[ij.x], d->angle), \
		v2(pos.x, pos.y));
	}
	d->color = pos.z;
	d->cnt = c;
	ij.x = -1;
	while (++ij.x < 6)
	{
		ij.y = -1;
		while (++ij.y < 4)
			d->faces[ij.x][ij.y] = tmp_faces[ij.x][ij.y];
	}
}

void	draw_cube(t_image *screen, t_vec3 pos, \
	t_vec2 size, t_vec3f rot)
{
	t_cube_drawd	cd;
	t_vec2			face[6][4];
	const int		colors[6] = {_YELLOW, _RED, _BLUE, _GREEN, _WHITE, _ORANGE};
	int				i;

	init_cube_draw_data(&cd, pos, size, rot);
	i = -1;
	while (++i < 6)
	{
		face[i][0] = cd.projected[cd.faces[i][0]];
		face[i][1] = cd.projected[cd.faces[i][1]];
		face[i][2] = cd.projected[cd.faces[i][2]];
		face[i][3] = cd.projected[cd.faces[i][3]];
		if (pos.z != _NULL)
			draw_quad(screen, face[i], cd.color, pos.z);
		draw_quad_contour(screen, face[i], colors[i]);
	}
}
