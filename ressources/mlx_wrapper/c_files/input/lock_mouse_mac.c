/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lock_mouse_mac.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 13:39:53 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/28 17:42:51 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

void	show_cursor(t_md *md)
{
	mlx_mouse_show(md->mlx, md->win);
}

void	hide_cursor(t_md *md)
{
	mlx_mouse_hide(md->mlx, md->win);
}

void	lock_mouse_center(t_md *md)
{
	hide_cursor(md);
}
