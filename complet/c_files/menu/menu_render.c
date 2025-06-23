/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:30:53 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/01 15:34:17 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	render_slider(t_md *md, t_slider *sldr, t_image *screen, float alpha)
{
	t_txtd		txt_d;
	const int	end = sldr->size.x * sldr->point / (sldr->steps - 1);
	const int	fill_clr = set_alpha(_RED, alpha);
	const int	bgr_clr = set_alpha(_WHITE, .2 + (alpha == 1) * .8);

	draw_grad_pxls(screen, sldr->pos, \
		v2(end, sldr->size.y), v3(fill_clr, _NULL, left));
	draw_pixels(screen, \
			v2(sldr->pos.x + end, sldr->pos.y), \
			v2(sldr->size.x - end, sldr->size.y), bgr_clr);
	txt_d.scale = md->prm.txt_sc;
	txt_d.color = _WHITE;
	if (md->menu.slider_hov == sldr->index)
		txt_d.color = _BLUE;
	txt_d.x = sldr->pos.x - (txt_d.scale) * ft_strlen(sldr->label) - 30;
	txt_d.y = sldr->pos.y + sldr->size.y / 2 - md->prm.txt_sc / 2;
	txt_d.onto = screen;
	rnd_fast_txt(md, txt_d, "%s", sldr->label);
	if (md->menu.slider_hov != sldr->index)
		return ;
	txt_d.x = sldr->pos.x + sldr->size.x / 2 - md->prm.txt_sc * 2;
	rnd_fast_txt(md, txt_d, "%.1f", *sldr->value);
}

void	render_sliders(t_md *md, t_menu *menu, t_image *screen)
{
	t_slider	*slider;
	int			i;

	if (md->menu.selected_slider)
	{
		render_slider(md, md->menu.selected_slider, screen, .75);
		return ;
	}
	i = -1;
	while (menu->sliders[++i].active)
	{
		slider = &menu->sliders[i];
		if (menu->slider_hov == i)
			render_slider(md, slider, screen, 1);
		else
			render_slider(md, slider, screen, .75);
	}
}

void	render_buttons(t_md *md, t_menu *menu)
{
	t_button	*but;
	t_txtd		txt_data;
	int			i;

	i = -1;
	while (menu->buttons[++i].active)
	{
		but = &menu->buttons[i];
		txt_data.scale = md->prm.txt_sc;
		txt_data.x = but->pos.x;
		txt_data.y = but->pos.y;
		txt_data.color = md->rgb[RGB_RED + (*but->value == 1)];
		txt_data.onto = menu->overlay;
		if (menu->button_hov == i)
			txt_data.color += 500;
		rnd_fast_txt(md, txt_data, "%s", but->label);
		txt_data.x -= md->prm.txt_sc * 2;
		txt_data.color = -1;
		txt_data.scale *= .75;
		if (but->key_show[0])
			rnd_fast_txt(md, txt_data, "%s", but->key_show);
	}
}

void	render_color_wheels(t_md *md, t_menu *menu, \
	t_image *screen, int touch_sz)
{
	t_vec2	sphere_p;
	int		i;
	t_clrp	*w;
	t_txtd	txtd;
	t_vec3	sphere_d;

	txtd = (t_txtd){0, 0, -1, md->prm.txt_sc, screen};
	sphere_d = (t_vec3){_BLACK, 50, 1};
	i = -1;
	while (++i < 3)
	{
		w = &menu->clrp[i];
		draw_img(w->img, screen, w->pos, -1);
		txtd.x = w->pos.x + w->size.x / 2 - \
			(md->prm.txt_sc * (ft_strlen(w->label)) / 2);
		txtd.y = w->pos.y + w->size.y + 5;
		rnd_fast_txt(md, txtd, w->label);
		if (w->mouse_touch.x == -1)
			continue ;
		sphere_p = sub_vec2(w->mouse_touch, _v2(touch_sz / 2));
		draw_sphere(screen, sphere_p, _v2(touch_sz), sphere_d);
		draw_sphere(screen, add_vec2(sphere_p, _v2(2)), \
			_v2(touch_sz - 4), v3(*w->color, 50, 1));
	}
}

void	render_menu(t_md *md, t_menu *menu)
{
	const t_vec2	win_sz = md->win_sz;

	if (menu->refresh_bg)
	{
		draw_img(md->screen, menu->overlay, _v2(0), -1);
		menu->refresh_bg = 0;
	}
	if (menu->refresh_ui)
	{
		draw_img(md->screen, menu->overlay, _v2(0), -1);
		render_buttons(md, menu);
		render_sliders(md, menu, menu->overlay);
		render_color_wheels(md, menu, menu->overlay, 16);
		center_menu_txt(md, v2(0, -win_sz.y * .4), win_sz.x / 20, "Menu");
		center_menu_txt(md, v2(0, -win_sz.y * .3), win_sz.x / 60, "cube3d");
		menu->refresh_ui = 0;
	}
	else if (menu->selected_slider)
		render_slider(md, menu->selected_slider, menu->overlay, 1);
	md->mlx_put(md->mlx, md->win, menu->overlay->img, 0, 0);
}
