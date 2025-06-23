/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:55:38 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/28 10:35:30 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

double	get_time_in_seconds(void)
{
	struct timespec	ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec + (ts.tv_nsec / 1000000000.0));
}

void	start_timer(double *timer)
{
	*timer = get_time_in_seconds();
}

void	init_timer(t_md *md, t_timer *tm)
{
	const t_vec2	win_sz = (t_vec2){md->win_sz.x, md->win_sz.y};

	md->menu.menu_time = 0;
	tm->time = 0;
	tm->fps = 0;
	tm->avrg_fps = 0;
	tm->prv_fps = 0;
	tm->delta_time = 0;
	tm->tm_level = 0;
	tm->shoot_timer = 0;
	tm->tm_menu = 0;
	tm->frm_cnt = 0;
	tm->tm_anim = 0;
	tm->tm_walk = 0;
	tm->trig_anim = 0;
	tm->logs_p = (t_vec2){win_sz.x * .8, win_sz.y * .5};
	tm->no_portal_cooldown = 0;
	tm->prev_time = get_time_in_seconds();
	tm->fps_tm = tm->game_start;
	tm->game_start = tm->prev_time;
	tm->logs = NULL;
}

pid_t	play_index(t_md *md, const char *filepath, int index)
{
	pid_t	pid;
	char	*index_txt;
	char	*path_with_index;
	char	*full_path;

	if (!md->prm.au_on)
		return (0);
	index_txt = ft_itoa(index);
	path_with_index = ft_strjoin(index_txt, ".mp3");
	full_path = ft_strjoin(filepath, path_with_index);
	pid = play_sound(md, full_path);
	free(full_path);
	free(path_with_index);
	free(index_txt);
	return (pid);
}
