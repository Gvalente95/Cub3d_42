/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_update_sliders.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 15:34:39 by gvalente          #+#    #+#             */
/*   Updated: 2025/06/01 15:36:04 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	update_slider(t_md *md, t_slider *sld)
{
	int		x_pos;
	t_vec2	sz;

	sz = sld->size;
	x_pos = sld->base_point;
	if (!md->key_prs[CTRL_KEY])
		x_pos = ((md->mouse.real.x - sld->pos.x) * (sld->steps - 1)) / sz.x;
	if (x_pos == sld->point)
		return (0);
	sld->point = minmax(0, sld->steps - 1, x_pos);
	*sld->value = sld->limits.x + ((sld->limits.z - sld->limits.x) * \
		((float)sld->point / (sld->steps - 1)));
	if (md->prm.fov <= 0)
		md->prm.fov = 1;
	md->plr.pos.z = -md->prm.height;
	md->cam.pos.z = md->plr.pos.z - md->prm.height;
	if (!ft_strncmp(sld->label, "win", 3))
		return (replace_window(md, md->prm.win_x, md->prm.win_y));
	render(md);
	return (1);
}

void	update_sliders(t_md *md, t_menu *menu, t_vec2 sz)
{
	t_slider		*sld;
	int				i;
	int				cur_hov;

	if (md->key_click == R_KEY || md->key_click == S_KEY)
		randomize_sliders(md, menu, md->key_click == S_KEY);
	cur_hov = -1;
	i = -1;
	while (menu->sliders[++i].active)
	{
		sld = &menu->sliders[i];
		if (!v2_bounds(md->mouse.real, sld->pos, v2(sz.x, sz.y * 1.3)))
			continue ;
		cur_hov = i;
		if (md->mouse.pressed == MOUSE_PRESS)
			menu->selected_slider = sld;
		if (menu->slider_hov != cur_hov)
		{
			menu->slider_hov = i;
			menu->refresh_ui = 1;
			break ;
		}
		return ;
	}
	update_menu_element_end(menu, &menu->slider_hov, cur_hov);
}

void	randomize_sliders(t_md *md, t_menu *menu, int is_reset)
{
	t_slider	*sld;
	int			i;

	i = -1;
	while (menu->sliders[++i].active)
	{
		sld = &menu->sliders[i];
		if (is_reset)
			sld->point = sld->base_point;
		else
			sld->point = r_range(0, sld->steps - 1);
		*sld->value = sld->limits.x + ((sld->limits.z - sld->limits.x) * \
			((float)sld->point / (sld->steps - 1)));
	}
	if (md->prm.fov <= 0)
		md->prm.fov = 1;
	md->plr.pos.z = -md->prm.height;
	md->cam.pos.z = md->plr.pos.z - md->prm.height;
	render(md);
	menu->refresh_bg = 1;
	menu->refresh_ui = 1;
}
