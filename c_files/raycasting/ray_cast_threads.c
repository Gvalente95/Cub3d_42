/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_threads.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:31:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/15 00:42:48 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"
#include <malloc/malloc.h>

void	update_ray_data(t_md *md, t_ray *ray, t_vec3f dir_val)
{
	memset(ray->dirty_checks, 0, sizeof(int) * (md->win_sz.y));
	ray->hit_data[0].hit = NULL;
	ray->hit_data[0].vertical_hit_at_e = 0;
	ray->hits_len = 0;
	ray->last_hit = NULL;
	init_base_ray(ray, ray->index, md->cam.pos, 0);
	ray->wall_strip_pos = _v2(md->hud.floor_start);
	ray->dir = get_v3f(dir_val.x, dir_val.y, 0);
	ray->angle = dir_val.z;
	ray->wall_hit = NULL;
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
	if (ray->wall_hit)
		draw_wall_line(md, ray->distance, ray->wall_hit, ray);
	draw_raycast_background(md, ray);
	*last_valid = x;
	if (ray->hits_len)
		return (draw_stored_door_hits(md, ray));
	return (1);
}

void	*cast_thread_batch(void *content)
{
	t_thread_worker	*thread;
	t_md			*md;
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
		if (!cast_thread_ray(md, ray_index, NULL))
			break ;
	}
	return (NULL);
}
