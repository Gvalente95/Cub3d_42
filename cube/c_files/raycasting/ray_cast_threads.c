/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_threads.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:31:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/24 11:51:13 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	update_ray_data(t_md *md, t_ray *ray, t_vec3f dir_val)
{
	ray->hit_data[0].hit = NULL;
	ray->hit_data[0].vertical_hit_at_e = 0;
	ray->hits_len = 0;
	init_base_ray(ray, ray->index, md->cam.pos, 0);
	ray->wall_strip_pos = _v2(md->hud.floor_start);
	ray->dir = get_v3f(dir_val.x, dir_val.y, 0);
	ray->angle = dir_val.z;
	ray->check_hit = NULL;
	ray->wall_hit = NULL;
}

int	draw_stored_sprite_hits(t_md *md, t_ray *ray)
{
	t_hit_data		*hit_data;
	int				ret_val;

	ret_val = 1;
	while (ray->hits_len--)
	{
		hit_data = &ray->hit_data[ray->hits_len];
		if (!hit_data->hit)
		{
			ret_val = 0;
			break ;
		}
		if (ray->check_hit && ray->check_hit->type == nt_door)
		{
			ret_val = 1;
			break ;
		}
		ray->vertical_hit = hit_data->vertical_hit_at_e;
		ray->pos = hit_data->post_at_hit;
		draw_wall_line(md, hit_data->dist_at_e, hit_data->hit, ray);
	}
	return (ret_val);
}

//	x = (from x index, to x index)
void	draw_strip(t_image *from, t_image *to, t_vec2 x, int stop_y)
{
	t_vec2			pos;
	int				clr;

	if (x.x < 0)
		x.x = x.y - 1;
	pos = v2(x.x, 0);
	while (++pos.y < stop_y)
	{
		clr = from->src[pos.y * from->size_line / 4 + x.x];
		draw_pixel(to, v2(x.y, pos.y), clr, 1);
	}
}

int	cast_thread_ray(t_md *md, int x, int *last_valid)
{
	t_ray			*ray;
	t_image			*scrn;

	ray = &md->rays[x];
	ray->active = 0;
	if (md->prm.alternate_draw && !((x % 2 == 0) == (md->timer.time % 2 == 0)))
		return (0);
	scrn = md->screen;
	if (md->prm.ray_mod >= 2.0f && x % (int)floorf(md->prm.ray_mod) != 0)
		return (draw_strip(scrn, scrn, v2(x - 1, x), md->win_sz.y), 0);
	ray->active = 1;
	ray->index = x;
	update_ray_data(md, ray, md->thrd_manager.dir_vals[ray->index]);
	ray_move(md, ray, md->thrd_manager.ray_visu_offset);
	if (!ray->check_hit && ray->wall_hit != NULL)
		draw_wall_line(md, ray->distance, ray->wall_hit, ray);
	draw_raycast_background(md, ray);
	*last_valid = x;
	if (ray->hits_len > 0)
		return (draw_stored_sprite_hits(md, ray));
	return (1);
}

void	*cast_thread_batch(void *content)
{
	t_thread_worker	*thread;
	t_md			*md;
	t_ray			*ray;
	int				i;
	int				ray_index;

	thread = (t_thread_worker *)content;
	md = thread->md;
	i = -1;
	while (++i < THREADS_BATCH)
	{
		ray_index = thread->index + i;
		if (ray_index >= md->win_sz.x)
			break ;
		ray = &md->rays[thread->index + i];
		ray->is_floor_worker = ((i + 1) % FLOOR_WORKERS == 0);
		if (!cast_thread_ray(md, ray_index, NULL))
			break ;
	}
	return (NULL);
}
