/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_threads_Loop.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:26:11 by gvalente          #+#    #+#             */
/*   Updated: 2025/06/01 15:32:42 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	*cast_thread_loop(void *arg)
{
	t_thread_data	*data;
	int				x;
	int				last_valid_ray;

	last_valid_ray = -1;
	data = (t_thread_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->mutex);
		while (!data->should_work && !data->should_exit)
			pthread_cond_wait(&data->cond, &data->mutex);
		if (data->should_exit)
		{
			pthread_mutex_unlock(&data->mutex);
			break ;
		}
		data->should_work = 0;
		pthread_mutex_unlock(&data->mutex);
		x = data->start_x - 1;
		while (++x < data->end_x)
			cast_thread_ray(data->md, x, &last_valid_ray);
		soft_barrier_wait(&data->md->thrd_manager.barrier);
	}
	return (NULL);
}

static void	trigger_threads(t_md *md)
{
	t_thrd_manager	*rm;
	int				i;

	rm = &md->thrd_manager;
	i = -1;
	while (++i < rm->threads_amount)
	{
		pthread_mutex_lock(&rm->thrdlp[i].mutex);
		rm->thrdlp[i].should_work = 1;
		pthread_cond_signal(&rm->thrdlp[i].cond);
		pthread_mutex_unlock(&rm->thrdlp[i].mutex);
	}
	soft_barrier_wait(&rm->barrier);
}

void	cast_ray_threads_lp(t_md *md)
{
	t_thrd_manager	*rm;

	if (!md->prm.use_thrd)
	{
		cast_rays(md);
		return ;
	}
	md->env.overlay_y_start = md->win_sz.y;
	rm = &md->thrd_manager;
	rm->ray_visu_offset = get_2d_ray_pos(md);
	compute_ray_directions(md, rm->dir_vals, md->win_sz.x);
	trigger_threads(md);
	if (md->prm.show_grass)
	{
		draw_from_pos(md->env.grass_overlay, md->screen, \
			_v2(0), v2(0, md->env.overlay_y_start));
		flush_img(md->env.grass_overlay, _NULL, 10, 0);
	}
	if (rm->ents_to_draw)
		draw_found_ents(md, rm);
}
