/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_move_dda.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 09:31:36 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/08 16:28:58 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	draw_wall_line_dda(t_md *md, float dist, t_ent *hit, t_ray *ray)
{
	t_vec3f	txtr_cord;
	float	wall_x;

	ray->wall_hit = hit;
	ray->distance = maxf(5, dist);
	hit->hp = ray->distance > md->t_len * 2;
	if (ray->vertical_hit == 0)
		wall_x = ray->start.y + ray->distance * ray->dir.y;
	else
		wall_x = ray->start.x + ray->distance * ray->dir.x;
	wall_x -= floorf(wall_x);
	txtr_cord.x = (int)(wall_x * hit->size.x);
	if ((ray->vertical_hit == 0 && ray->dir.x > 0) || \
		(ray->vertical_hit == 1 && ray->dir.y < 0))
		txtr_cord.x = hit->size.x - txtr_cord.x - 1;
	txtr_cord.y = correct_fisheye(md, ray, hit, ray->distance);
	return (1);
}

t_ent	*check_segment_at(t_md *md, t_ray *ray, t_vec2 cord)
{
	t_ent_type	type;
	t_ent		*ent;

	ent = get_mapped_at_cord(md, cord);
	if (!ent)
		return (NULL);
	if (!ent->revealed && ray->distance < md->t_len * REVEAL_DISTANCE)
		show_minimap_entity(md, ent, md->mmap.bg, 1);
	type = ent->type;
	if (type == nt_wall)
		return (ent);
	if (!validate_check_hit(md, ray, ent, type))
		return (NULL);
	ray->hit_data[ray->hits_len].post_at_hit = ray->pos;
	ray->hit_data[ray->hits_len].vertical_hit_at_e = ray->vertical_hit;
	ray->hit_data[ray->hits_len].dist_at_e = ray->distance;
	ray->hit_data[ray->hits_len++].hit = ent;
	if (ent->type == nt_door)
		ray->had_door = 1;
	return (NULL);
}

t_ent	*step_ray_dda(t_md *md, t_ray *ray)
{
	if (ray->side_dist.x < ray->side_dist.y)
	{
		ray->side_dist.x += ray->delta.x;
		ray->cord.x += ray->step.x;
		ray->vertical_hit = 0;
	}
	else
	{
		ray->side_dist.y += ray->delta.y;
		ray->cord.y += ray->step.y;
		ray->vertical_hit = 1;
	}
	ray->steps++;
	if (ray->vertical_hit == 0)
		ray->distance = ray->side_dist.x - ray->delta.x;
	else
		ray->distance = ray->side_dist.y - ray->delta.y;
	return (check_segment_at(md, ray, ray->cord));
}

int	ray_move_dda(t_md *md, t_ray *ray)
{
	t_ent	*hit;

	while (++ray->steps < md->prm.ray_depth)
	{
		hit = step_ray_dda(md, ray);
		if (hit && hit->type == nt_wall)
			ray->wall_hit = hit;
		if (ray->wall_hit)
			return (ray->steps);
		if (ray->check_hit && ray->steps > ray->init_steps + 2)
			break ;
	}
	ray->wall_hit = NULL;
	return (-1);
}
