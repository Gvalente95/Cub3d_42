/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_move.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:28:02 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/09 10:53:44 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	addHit(t_ray *ray, t_ent *hit, float distance) {
	ray->hit_data[ray->hits_len].post_at_hit = ray->pos;
	ray->hit_data[ray->hits_len].vertical_hit_at_e = ray->vertical_hit;
	ray->hit_data[ray->hits_len].dist_at_e = distance;
	ray->hit_data[ray->hits_len++].hit = hit;
	if (hit->type != nt_grass)
		ray->last_hit = hit;
}

t_ent* search_in_grid(t_md* md, t_ray* ray, float distance)
{
	t_ent			*e;

	e = get_mapped_at_pos(md, (t_vec2f) { ray->pos.x, ray->pos.y });
	if (!e)
		return (NULL);
	e->cam_distance = ray->distance;
	if (!e->revealed && ray->distance < md->t_len * REVEAL_DISTANCE)
		show_minimap_entity(md, e, md->mmap.bg, 1);
	if ((e->type == nt_wall || e->type == nt_ext_wall) && \
		(!md->prm.super_view && e->pos.z == 0)) {
		addHit(ray, e, distance);
		return (e);
	}
	if (validate_check_hit(md, ray, e, e->type)) {
		addHit(ray, e, distance);
		if (e->type == nt_door && !ray->had_door)
		{
			ray->dist_at_door = distance;
			ray->door = e;
			ray->had_door = 1;
		}
	}
	return (NULL);
}

static int	ray_can_look(t_md *md, t_ray *ray)
{
	if (!md->prm.ent_mode && !ray->on_grid) return (0);
	else if (!ray->on_grid && ray->last_hit) {
		t_ent_type lastHitType = ray->last_hit->type;
		if (lastHitType == nt_wall || lastHitType == nt_ext_wall)
			return (0);
	}
	if (ray->pos.x < -md->t_len || ray->pos.x > md->t_len * md->map.size.x)
		return (0);
	if (ray->pos.y < -md->t_len || ray->pos.y > md->t_len * md->map.size.y)
		return (0);
	return (1);
}

static int	ray_can_stop(t_md *md, t_ray *r, t_ent *hit, int tlen)
{
	t_vec3f	next;

	(void)hit;
	if (r->hits_len >= md->prm.max_view_sprite)
		return (1);
	if (r->pos.x >= 0 && r->pos.x <= md->map.size.x * tlen && \
		r->pos.y >= 0 && r->pos.y <= md->map.size.y * tlen)
	{
		next = add_vec3f(r->pos, scale_vec3f(r->dir, 3));
		if (next.x < 0 || next.x > md->map.size.x * tlen || \
			next.y < 0 || next.y > md->map.size.y * tlen)
			return (1);
	}
	return (0);
}

int	ray_move(t_md *md, t_ray *ray, t_vec2 visu_offset)
{
	t_ent	*hit;

	while (++ray->steps < md->prm.ray_depth)
	{
		ray->distance++;
		ray->pos = add_vec3f(ray->pos, ray->dir);
		ray->on_grid = update_ray_grid_pos(md, ray);
		if (!ray_can_look(md, ray))
			continue ;
		if (md->prm.view_2d && md->prm.show_rays)
			set_ray_color(md, ray);
		hit = search_in_grid(md, ray, ray->steps);
		render_ray(md, ray, visu_offset);
		if (ray_can_stop(md, ray, hit, md->t_len))
			return (-1);
		if (hit && (hit->type == nt_wall || hit->type == nt_ext_wall))
			return (ray->steps);
	}
	return (-1);
}
