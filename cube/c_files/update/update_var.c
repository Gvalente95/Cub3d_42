/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 23:56:16 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/29 19:26:48 by giuliovalen      ###   ########.fr       */
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
