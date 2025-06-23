/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 12:39:01 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/07 14:00:00 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	init_thread_pool(t_md *md, int thread_count)
{
	t_thrd_manager	*rm;
	int				width;
	t_thread_data	*thrd;
	int				i;

	rm = &md->thrd_manager;
	width = md->win_sz.x;
	soft_barrier_init(&rm->barrier, thread_count + 1);
	rm->threads_amount = thread_count;
	i = -1;
	while (++i < thread_count)
	{
		thrd = &rm->thrdlp[i];
		thrd->md = md;
		thrd->thread_id = i;
		thrd->start_x = (width * i) / thread_count;
		thrd->end_x = (width * (i + 1)) / thread_count;
		thrd->should_work = 0;
		thrd->should_exit = 0;
		pthread_mutex_init(&thrd->mutex, NULL);
		pthread_cond_init(&thrd->cond, NULL);
		pthread_create(&thrd->thread, NULL, cast_thread_loop, thrd);
	}
}
