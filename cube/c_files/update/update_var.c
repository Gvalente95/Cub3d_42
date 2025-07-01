/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 23:56:16 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/07/01 14:59:24 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	add_alert(t_md *md, double duration, int *out_cond, const char *alert)
{
	t_log	*ld;

	if (md->timer.cur_tm > md->alert.duration)
		play_sound(md, AU_LOW_CLICK);
	ld = &md->alert;
	ft_strlcpy(ld->buffer, alert, 254);
	ld->txt_d.x = md->win_sz.x / 2 - (ft_strlen(alert) * ld->txt_d.scale) / 2;
	ld->duration = md->timer.cur_tm + duration;
	ld->out_cond = out_cond;
}

void	update_phys(t_md *md)
{
	const float	h = 1.25f;
	const float	t = 0.35f;

	md->g = (2.0f * h / (t * t)) * md->t_len * 0.0008;
	md->v0 = -((2.0f * h / t) * md->t_len * 0.02);
}
