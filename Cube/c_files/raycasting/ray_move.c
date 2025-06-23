/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_move.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:28:02 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/06 18:01:08 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

t_ent	*search_in_grid(t_md *md, t_ray *ray, float distance)
{
	t_ent			*e;

	e = get_mapped_at_pos(md, get_v2f(ray->pos.x, ray->pos.y));
	if (!e)
		return (NULL);
	e->cam_distance = ray->distance;
	if (!e->revealed && ray->distance < md->t_len * REVEAL_DISTANCE)
		show_minimap_entity(md, e, md->mmap.bg, 1);
	if ((e->type == nt_wall || e->type == nt_ext_wall) && \
		(!md->prm.super_view))
		return (e);
	if (!validate_check_hit(md, ray, e, e->type))
		return (NULL);
	if (!md->prm.use_thrd && (ray->index == 0 || ray->check_hit))
		cast_check_ray(md, ray, ray->start, e);
	ray->hit_data[ray->hits_len].post_at_hit = ray->pos;
	ray->hit_data[ray->hits_len].vertical_hit_at_e = ray->vertical_hit;
	ray->hit_data[ray->hits_len].dist_at_e = distance;
	ray->hit_data[ray->hits_len++].hit = e;
	if (e->type != nt_door || ray->had_door)
		return (NULL);
	ray->door = e;
	ray->dist_at_door = distance;
	ray->had_door = 1;
	return (NULL);
}

static int	ray_can_look(t_md *md, t_ray *ray, int on_grid)
{
	(void)ray;
	if (!on_grid && !md->prm.ent_mode)
		return (0);
	return (1);
}

static int	ray_can_stop(t_md *md, t_ray *ray, t_ent *hit)
{
	if (ray->hits_len >= md->prm.max_view_sprite)
		return (1);
	if (hit && ray->check_hit && ray->check_hit == hit)
		return (1);
	if (ray->check_hit && ray->steps > ray->init_steps + 2)
		return (1);
	if (ray->start.x >= -md->t_len && \
		ray->start.x < md->map.size.x * md->t_len && \
		ray->start.y >= -md->t_len && \
		ray->start.y < md->map.size.y * md->t_len)
	{
		if (ray->pos.x < -md->t_len || ray->pos.x > md->t_len * md->map.size.x)
			return (1);
		if (ray->pos.y < -md->t_len || ray->pos.y > md->t_len * md->map.size.y)
			return (1);
	}
	return (0);
}

int	ray_move(t_md *md, t_ray *ray, t_vec2 visu_offset)
{
	t_ent	*hit;
	int		on_grid;
	int		hit_wall;

	while (++ray->steps < md->prm.ray_depth)
	{
		ray->distance++;
		ray->pos = add_vec3f(ray->pos, ray->dir);
		on_grid = update_ray_grid_pos(md, ray);
		if (md->prm.view_2d && md->prm.show_rays)
			set_ray_color(md, ray);
		if (!ray_can_look(md, ray, on_grid))
			continue ;
		hit = search_in_grid(md, ray, ray->steps);
		render_ray(md, ray, visu_offset);
		if (ray_can_stop(md, ray, hit))
			return (-1);
		hit_wall = (hit && (hit->type == nt_wall || hit->type == nt_ext_wall));
		if (hit_wall && !ray->check_hit)
			return (ray->wall_hit = hit, ray->steps);
	}
	ray->wall_hit = NULL;
	return (-1);
}
