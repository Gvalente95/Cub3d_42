/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_threads_tools.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:30:23 by gvalente          #+#    #+#             */
/*   Updated: 2025/04/28 16:03:30 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

void	cleanup_thread_pool(t_md *md)
{
	t_thrd_manager	*rm;
	int				i;

	rm = &md->thrd_manager;
	i = -1;
	while (++i < rm->threads_amount)
	{
		pthread_mutex_lock(&rm->thrdlp[i].mutex);
		rm->thrdlp[i].should_exit = 1;
		pthread_cond_signal(&rm->thrdlp[i].cond);
		pthread_mutex_unlock(&rm->thrdlp[i].mutex);
	}
	i = -1;
	while (++i < rm->threads_amount)
		pthread_join(rm->thrdlp[i].thread, NULL);
	soft_barrier_destroy(&rm->barrier);
}

void	soft_barrier_init(t_soft_barrier *b, int total)
{
	pthread_mutex_init(&b->mutex, NULL);
	pthread_cond_init(&b->cond, NULL);
	b->count = 0;
	b->total = total;
}

void	soft_barrier_wait(t_soft_barrier *b)
{
	pthread_mutex_lock(&b->mutex);
	b->count++;
	if (b->count == b->total)
	{
		b->count = 0;
		pthread_cond_broadcast(&b->cond);
	}
	else
		pthread_cond_wait(&b->cond, &b->mutex);
	pthread_mutex_unlock(&b->mutex);
}

void	soft_barrier_destroy(t_soft_barrier *b)
{
	pthread_mutex_destroy(&b->mutex);
	pthread_cond_destroy(&b->cond);
}
