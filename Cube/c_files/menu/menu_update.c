/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_update.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:33:17 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/01 15:35:44 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	update_buttons(t_md *md, t_menu *menu, t_vec2 size)
{
	t_button			*but;
	int					cur_hov;
	int					i;

	cur_hov = -1;
	i = -1;
	while (menu->buttons[++i].active)
	{
		but = &menu->buttons[i];
		if (!v2_bounds(md->mouse.real, but->pos, size))
			continue ;
		cur_hov = i;
		if (cur_hov != menu->button_hov)
			menu->button_hov = cur_hov;
		if (md->mouse.click != MOUSE_PRESS)
			continue ;
		*but->value = !(*but->value);
		render(md);
		menu->refresh_bg = 1;
		menu->refresh_ui = 1;
	}
	update_menu_element_end(menu, &menu->button_hov, cur_hov);
}

void	update_color_wheels(t_md *md, t_menu *menu)
{
	t_clrp	*w;
	int		i;

	if (md->mouse.pressed != MOUSE_PRESS)
		return ;
	i = -1;
	while (++i < 3)
	{
		w = &menu->clrp[i];
		if (!v2_bounds(md->mouse.real, w->pos, w->size))
			continue ;
		if (md->key_prs[CTRL_KEY])
		{
			w->mouse_touch.x = -1;
			*w->color = w->base_color;
		}
		else
			set_wheel(w, md->mouse.real, add_vec2(w->pos, _v2(w->size.x / 2)));
		menu->refresh_bg = 1;
		menu->refresh_ui = 1;
		render(md);
		break ;
	}
}

int	update_menu(t_md *md, t_menu *menu)
{
	if (md->mouse.click == MOUSE_PRESS)
		play_sound(md, AU_MOUSE_CLICK);
	if (md->mouse.click == MOUSE_RELEASE)
		play_sound(md, AU_MOUSE_RELEASE);
	update_input(md);
	if (!menu->selected_slider)
		update_sliders(md, menu, menu->sliders[0].size);
	if (menu->selected_slider)
	{
		if (!md->mouse.pressed)
		{
			menu->refresh_ui = 1;
			menu->selected_slider = NULL;
		}
		else
			menu->refresh_bg = update_slider(md, menu->selected_slider);
	}
	else if (menu->slider_hov == -1)
	{
		update_color_wheels(md, menu);
		update_buttons(md, menu, \
			v2(md->prm.txt_sc * 15, md->prm.txt_sc + 10));
	}
	return (render_menu(md, menu), reset_mlx_values(md), 1);
}
