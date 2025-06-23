/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_time_events.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:08:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/06 17:59:45 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	add_log_to_queue(t_md *md, int color, const char *format, ...)
{
	t_log		*logs;
	va_list		args;
	t_dblst		*new;

	logs = md_malloc(md, sizeof(t_log));
	va_start(args, format);
	vsnprintf(logs->buffer, sizeof(logs->buffer), format, args);
	va_end(args);
	logs->txt_d.x = md->timer.logs_p.x;
	logs->txt_d.y = md->timer.logs_p.y;
	logs->txt_d.onto = md->screen;
	logs->txt_d.scale = md->prm.txt_sc;
	logs->txt_d.color = color;
	logs->duration = 20;
	logs->out_cond = NULL;
	logs->timer_start = md->timer.cur_tm;
	new = dblst_new(logs);
	dblst_add_front(&md->timer.logs, new);
	if (dblst_size(md->timer.logs) > 5)
		dblst_delone(dblst_last(md->timer.logs), free);
}

int	node_expired(t_timer *tm, t_dblst *node, t_log *m)
{
	t_dblst			*next;
	const float		elapsed_time = tm->cur_tm - m->timer_start;
	const double	fade_tresh = m->duration * .75f;
	int				has_expired;

	(void)fade_tresh;
	next = node->next;
	has_expired = (m->duration > 0 && elapsed_time > m->duration);
	if ((m->out_cond && *m->out_cond) || has_expired)
	{
		if (node == tm->logs)
			tm->logs = next;
		dblst_delone(node, free);
		return (1);
	}
	return (0);
}

void	update_and_render_logs(t_md *md, t_timer *timer)
{
	t_dblst		*node;
	t_log		*log;
	t_dblst		*next;
	t_txtd		t;
	int			y;

	y = 1;
	node = timer->logs;
	while (node)
	{
		next = node->next;
		log = (t_log *)node->content;
		if (node_expired(timer, node, log))
		{
			node = next;
			continue ;
		}
		t = log->txt_d;
		t.y = log->txt_d.y - y;
		rnd_fast_txt(md, t, log->buffer);
		y += t.scale;
		node = node->next;
	}
}

void	update_timers(t_md *md, t_timer *tm)
{
	if (md->prm.cap_fps < 100)
		cap_fps(&md->timer, 1.0f / md->prm.cap_fps);
	tm->cur_tm = get_time_in_seconds();
	md->timer.fps++;
	if (md->timer.fps > 50)
		reset_fps_timer(tm);
	tm->avrg_fps = (float)(++tm->frm_cnt) / (tm->cur_tm - tm->fps_tm);
	if (tm->cur_tm - tm->elapsed_pause >= 1)
	{
		tm->elapsed_pause = tm->cur_tm;
		tm->prv_fps = tm->fps;
		tm->fps = 0;
	}
	upd_timer(&tm->tm_fe, tm->cur_tm, .005 / md->prm.fe_speed, &tm->trig_fe);
	upd_timer(&tm->tm_anim, tm->cur_tm, ANIM_REFRESH, &tm->trig_anim);
	upd_timer(&tm->tm_walk, tm->cur_tm, \
		WALK_REFRESH / (1 + (md->key_prs[SHIFT_KEY] == 1)), &tm->trig_walk);
	tm->delta_time = (tm->cur_tm - tm->prev_time);
	tm->prev_time = tm->cur_tm;
	tm->time++;
	md->timer.fe_time += md->prm.fe_speed;
}

void	render_time_logs(t_md *md, t_timer *timer)
{
	update_timers(md, timer);
	if (timer->cur_tm < md->alert.duration)
		rnd_fast_txt(md, md->alert.txt_d, md->alert.buffer);
	if (!timer->logs)
		update_and_render_logs(md, timer);
}
