/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dyodlm <dyodlm@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 06:04:42 by dyodlm            #+#    #+#             */
/*   Updated: 2025/06/23 07:01:52 by dyodlm           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdbool.h>
#include "cub.h"

int	extract_length(t_data *data, int x, int y)
{
	float	dx;
	float	dy;

	dx = x - data->run.player.px;
	dy = y - data->run.player.py;
	return (sqrt(dx * dx + dy * dy));
}

void	draw_vertical_line_(t_data *data,
	t_point segment,
	int ray,
	int color)
{
	while (segment.x < segment.y)
	{
		if (segment.x >= 0 && segment.x < HI)
		{
			if (ray < data->run.map.max.x * (data->run.map.map_s / 3)
				&& segment.x < data->run.map.max.y * (data->run.map.map_s / 3))
			{
				segment.x++;
				continue ;
			}
			my_mlx_pixel_put2(data, ray, segment.x, color);
		}
		segment.x++;
	}
}

void	cast_length(t_data *data, float distance, int ray)
{
	float	wall_height;
	int		start_y;
	int		end_y;
	t_point	y_segment;

	distance = distance * cos(data->ray.ra - data->run.player.pa);
	wall_height = PROJECTION_CONSTANT / (distance * 2);
	start_y = HI / 2 - wall_height / 2;
	end_y = HI / 2 + wall_height / 2;
	if (start_y < 0)
		start_y = 0;
	if (end_y > HI)
		end_y = HI;
	y_segment.x = 0;
	y_segment.y = start_y;
	draw_vertical_line_(data, y_segment, ray, data->tokens.color[C_COLOR]);
	y_segment.x = y_segment.y;
	y_segment.y = end_y;
	draw_vertical_line_(data, y_segment, ray, WHITE);
	y_segment.x = y_segment.y;
	y_segment.y = HI;
	draw_vertical_line_(data, y_segment, ray, data->tokens.color[F_COLOR]);
}

static void	trace_single_ray(t_data *data, t_ray *ray, float angle)
{
	if (angle < 0)
		angle += 2 * PI;
	else if (angle > 2 * PI)
		angle -= 2 * PI;
	ray->ra = angle;
	adjust_ray_data(ray, data);
	ray->depth = 0;
	while (ray->depth++ < 1000)
	{
		update_ray_pos(ray, &data->run.map);
		if (wall_hit(ray->mapx_idx, ray->mapy_idx, ray, &data->run.map))
			break ;
	}
}

void	raycasting(t_data *data)
{
	t_ray	ray;
	t_point	p0;
	t_point	p1;
	int		r;
	float	ra;

	r = NUM_RAYS;
	ra = data->run.player.pa - (FOV / 2);
	ft_bzero(&p0, sizeof(t_point));
	ft_bzero(&p1, sizeof(t_point));
	while (r > 0)
	{
		trace_single_ray(data, &ray, ra);
		ray.distance = extract_length(data, ray.rx, ray.ry);
		data->ray = ray;
		cast_length(data, ray.distance, r);
		p0.x = data->run.player.px + PSIZE / 2;
		p0.y = data->run.player.py + PSIZE / 2;
		p1.x = (int)ray.rx;
		p1.y = (int)ray.ry;
		draw_line(p0, p1, data);
		ra += FOV / NUM_RAYS;
		r--;
	}
}
