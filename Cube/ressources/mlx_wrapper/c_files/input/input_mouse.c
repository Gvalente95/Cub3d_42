/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_mouse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:57:28 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/11 18:12:16 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

int	mouse_event_handler(int but, int x, int y, void *param)
{
	t_md	*md;
	t_vec2	scroll;

	md = (t_md *)param;
	md->mouse.pressed = but;
	if (md->mouse.pressed == MOUSE_PRESS)
		md->mouse.click = MOUSE_PRESS;
	scroll.y = (but == MOUSE_SCRL_D) - (but == MOUSE_SCRL_UP);
	scroll.x = (but == MOUSE_SCRL_L) - (but == MOUSE_SCRL_R);
	md->mouse.scroll_raw = scroll;
	return (0);
}

int	mouse_release_handler(int button, int x, int y, void *param)
{
	t_md	*md;

	(void)x;
	(void)y;
	(void)button;
	md = (t_md *)param;
	md->mouse.pressed = MOUSE_RELEASE;
	md->mouse.click = MOUSE_RELEASE;
	return (0);
}

int	mouse_motion_handler(int x, int y, void *param)
{
	t_vec3	grid_pos;
	t_md	*md;
	t_mouse	*msd;
	t_vec2	raw;

	md = (t_md *)param;
	msd = &md->mouse;
	msd->real = v2(x, y);
	raw = v2(x - msd->prev.x, y - msd->prev.y);
	msd->delta_raw = raw;
	msd->delta.x = raw.x * (1.0f - MOUSE_SMTH) + raw.x * MOUSE_SMTH;
	msd->delta.y = raw.y * (1.0f - MOUSE_SMTH) + raw.y * MOUSE_SMTH;
	msd->prev = v2(x, y);
	msd->pos.x += msd->delta.x * MOUSE_SENSITIVITY;
	msd->pos.y += msd->delta.y * MOUSE_SENSITIVITY;
	grid_pos = get_grid_pos(md, v3(msd->pos.x, msd->pos.y, 0));
	msd->grid_pos = v2((grid_pos.x + md->cam.ofst.x) / md->t_len, \
		(grid_pos.y + md->cam.ofst.y) / md->t_len);
	if (msd->locked && !md->menu.active && !md->battle_d.active && \
		(x < 50 || x > md->win_sz.x - 50 || y < 50 || y > md->win_sz.y - 50))
		wrap_mouse(md, raw.x, raw.y);
	return (msd->focus = 1, 0);
}

int	update_mouse(t_md *md)
{
	t_mouse	*msd;

	msd = &md->mouse;
	msd->world = v2(msd->pos.x + \
		md->cam.ofst.x, msd->pos.y + md->cam.ofst.y);
	return (cmp_vec2(msd->prev, v2((int)msd->pos.x, (int)msd->pos.y)));
}
