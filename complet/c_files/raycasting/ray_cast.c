/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:31:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/23 18:10:07 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	cast_check_ray(t_md *md, t_ray *ray, t_vec3f start_pos, t_ent *check)
{
	t_ray	*new_ray;
	float	fov;
	float	angle_step;
	float	yaw;
	float	ray_yaw;

	if (!ray || !check || ray->index >= md->t_len * 3)
		return (0);
	new_ray = &md->rays[ray->index + 1];
	new_ray->index = ray->index + 1;
	new_ray->check_hit = check;
	new_ray->init_steps = ray->steps + 1;
	new_ray->pos = start_pos;
	new_ray->had_door = ray->had_door;
	yaw = (md->cam.rot.x) * (_PI / 180.0f);
	if (yaw < -_PI)
		yaw += 2 * _PI;
	else if (yaw >= _PI)
		yaw -= 2 * _PI;
	fov = (int)md->prm.fov * (_PI / 180.0f);
	angle_step = fov / (float)(md->win_sz.x - 1);
	ray_yaw = yaw - (fov / 2.0f) + (angle_step * (-new_ray->index));
	new_ray->angle = atan2f(sinf(ray_yaw), cosf(ray_yaw));
	new_ray->dir = get_v3f(cosf(ray_yaw), sinf(ray_yaw), 0);
	return (cast_ray(md, new_ray, get_2d_ray_pos(md)));
}

int	cast_ray(t_md *md, t_ray *ray, t_vec2 visu_offset)
{
	ray->hit_data[0].hit = NULL;
	ray->hits_len = 0;
	ray->vertical_hit = 0;
	ray_move(md, ray, visu_offset);
	if (ray->wall_hit)
		draw_wall_line(md, ray->distance, ray->wall_hit, ray);
	if (ray->hits_len > 0)
		return (draw_stored_sprite_hits(md, ray));
	return (1);
}

void	compute_ray_directions(t_md *md, t_vec3f *dir_vals, int rays_amount)
{
	float	fov;
	float	angle_step;
	float	yaw;
	int		i;
	float	ray_yaw;

	yaw = md->cam.rot.x * (_PI / 180.0f);
	if (yaw < -_PI)
		yaw += 2 * _PI;
	else if (yaw >= _PI)
		yaw -= 2 * _PI;
	fov = (int)md->prm.fov * (_PI / 180.0f);
	angle_step = fov / (float)(md->win_sz.x - 1);
	i = -1;
	while (++i < rays_amount)
	{
		ray_yaw = yaw - (fov / 2.0f) + (angle_step * i);
		dir_vals[i].x = cosf(ray_yaw);
		dir_vals[i].y = sinf(ray_yaw);
		dir_vals[i].z = atan2f(dir_vals[i].y, dir_vals[i].x);
	}
}

void	cast_rays(t_md *md)
{
	t_thrd_manager			*rm;
	int						i;
	int						last_valid;

	last_valid = -1;
	rm = &md->thrd_manager;
	rm->ray_visu_offset = get_2d_ray_pos(md);
	compute_ray_directions(md, rm->dir_vals, md->win_sz.x);
	i = -1;
	while (++i < md->win_sz.x)
		cast_thread_ray(md, i, &last_valid);
	if (rm->ents_to_draw)
		draw_found_ents(md, rm);
}

int	is_in_list(t_dblst *lst, t_ent *e)
{
	t_dblst	*node;
	t_ent	*node_e;

	node = dblst_first(lst);
	while (node)
	{
		node_e = (t_ent *)node->content;
		if (node_e == e)
			return (1);
		node = node->next;
	}
	return (0);
}
