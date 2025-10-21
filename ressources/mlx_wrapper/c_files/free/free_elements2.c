/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_elements2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 12:51:14 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/15 01:20:35 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

int	free_ray_data(t_md *md)
{
	int	i;
	int	fa;

	fa = 0;
	i = -1;
	while (++i < md->win_sz.x)
	{
		free(md->rays[i].dirty_checks);
		free(md->rays[i].hit_data);
		fa += 2;
	}
	printf("RAY_DATA %-5s%d%s\n", PGREEN, fa, PRESET);
	return (fa);
}

int	free_menu(t_md *md, t_menu *menu)
{
	int	i;
	int	fa;

	fa = 0;
	fa += free_image_data(md, menu->overlay);
	fa += free_image_data(md, menu->bgr_overlay);
	fa += free_image_data(md, menu->ui_overlay);
	i = -1;
	while (++i < 3)
		fa += free_image_data(md, menu->clrp[i].img);
	printf("MENU %-5s%d%s\n", PGREEN, fa, PRESET);
	return (fa);
}
