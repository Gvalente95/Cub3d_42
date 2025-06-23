/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_menu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:23:31 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/11 19:42:58 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	set_menu_pos(t_md *md, t_menu *menu, \
	t_vec3 sldr_offst, t_vec3 but_ofst)
{
	int				i;
	t_vec2			pos;
	const t_vec2	win_sz = md->win_sz;
	t_slider		*slider;
	t_button		*button;

	i = -1;
	while (menu->sliders[++i].active)
	{
		slider = &menu->sliders[i];
		slider->size = v2(md->win_sz.x / 6, sldr_offst.z);
		pos.x = md->win_sz.x / 4 + sldr_offst.x;
		pos.y = ((slider->size.y + sldr_offst.y) * i) + win_sz.y * .2;
		slider->pos = pos;
	}
	i = -1;
	while (menu->buttons[++i].active)
	{
		button = &menu->buttons[i];
		pos.x = md->win_sz.x - md->win_sz.x / 4 - but_ofst.x;
		pos.y = ((slider->size.y + sldr_offst.y) * i) + win_sz.y * .2;
		button->pos = pos;
	}
}

static void	init_menu_overlay(t_md *md, t_menu *menu)
{
	menu->bgr_color = _BLACK;
	menu->overlay = init_img(md, md->win_sz, NULL, menu->bgr_color);
	menu->ui_overlay = init_img(md, md->win_sz, NULL, -1);
	menu->bgr_overlay = init_img(md, md->win_sz, NULL, -1);
}

static void	set_clr_pck(t_md *md, t_clrp *clrP, int *value, const char *lbl)
{
	const t_vec2	size = _v2(md->var);

	clrP->label = lbl;
	clrP->color = value;
	clrP->img = init_img(md, size, "utils/clr_w_full.xpm", -1);
	clrP->size = size;
	clrP->pos = _v2(0);
	clrP->mouse_touch = _v2(-1);
	clrP->base_color = *value;
	clrP->picker_value = color_to_v4(*value);
}

void	set_color_pickers(t_md *md, t_menu *menu)
{
	const t_vec2	size = _v2(75);
	const t_vec2	winsz = md->win_sz;
	const t_vec2	base_p = v2(winsz.x - size.x * 4, winsz.y * .05);
	const int		spc = 10;

	md->var = size.x;
	set_clr_pck(md, &menu->clrp[0], &md->hud.floor_color, "Floor");
	set_clr_pck(md, &menu->clrp[1], &md->hud.ceiling_color, "Sky");
	set_clr_pck(md, &menu->clrp[2], &md->hud.fog_color, "Fog");
	menu->clrp[0].pos = v2(base_p.x, base_p.y);
	menu->clrp[1].pos = v2(base_p.x + md->var + spc, base_p.y);
	menu->clrp[2].pos = v2(base_p.x + md->var * 2 + spc * 2, base_p.y);
	md->var = 0;
}

void	init_menu(t_md *md, t_menu *menu)
{
	md->fx.hue = get_v4f(1, 1, 1, 0);
	init_menu_overlay(md, menu);
	init_menu_elements(md, menu);
	set_menu_pos(md, menu, v3(50, 2, 40), v3(100, 2, 40));
	set_color_pickers(md, menu);
	menu->refresh_ui = 0;
	menu->refresh_bg = 0;
	menu->cube_logo_rot = v3f(0);
}
