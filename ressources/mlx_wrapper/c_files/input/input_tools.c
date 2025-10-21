/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:00:30 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/10 02:42:19 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

void	wrap_mouse(t_md* md, int delta_x, int delta_y)
{
	t_vec2	block_pos;

	return;
	lock_mouse_center(md);
	block_pos = v2(md->win_sz.x - 200, md->win_sz.y - 200);
	md->mouse.prev = block_pos;
	md->mouse.world = block_pos;
}

void	unlock_mouse(t_md *md)
{
	show_cursor(md);
	md->mouse.locked = 0;
}
