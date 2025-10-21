/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_key.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 18:33:59 by gvalente          #+#    #+#             */
/*   Updated: 2025/05/26 19:41:41 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

int	handle_key_press(int keycode, t_md *md)
{
	if (keycode < 0 || keycode > 65536)
		return (0);
	md->last_key = keycode;
	md->key_click = keycode;
	md->key_prs[keycode] = 1;
	return (0);
}

int	handle_key_release(int keycode, t_md *md)
{
	if (keycode >= 0 && keycode < 65536)
		md->key_prs[keycode] = 0;
	md->key_click = -1;
	return (0);
}

void	reset_mlx_values(t_md *md)
{
	md->key_click = -1;
	md->mouse.click = MOUSE_NOPRESS;
	if (md->mouse.pressed == MOUSE_RELEASE)
		md->mouse.pressed = 0;
	md->mouse.delta = v2f(0);
	md->mouse.scroll_delta = scale_vec2f(md->mouse.scroll_delta, .8f);
	md->mouse.scroll_delta.x += md->mouse.scroll_raw.x;
	md->mouse.scroll_delta.y += md->mouse.scroll_raw.y;
	md->mouse.scroll_raw = _v2(0);
}

int	close_window(t_md *md)
{
	free_md(md, 1);
	exit(0);
	return (0);
}
