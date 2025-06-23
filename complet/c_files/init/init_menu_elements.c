/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_menu_elements.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 21:25:49 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/01 15:09:02 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

//data: r = min, data.g = current data.b = max data.a = (int)steps
void	inisld(t_md *md, char *label, t_vec4f data, float *value)
{
	t_slider	*sld;
	t_vec3f		slider_limits;

	sld = &md->menu.sliders[md->menu.slider_index];
	sld->index = md->menu.slider_index;
	sld->steps = (int)data.a;
	slider_limits = get_v3f(data.r, data.g, data.b);
	ft_strlcpy(sld->label, label, 50);
	sld->point = minmax(0, sld->steps - 1, \
	((data.g - data.r) / (data.b - data.r)) * (sld->steps - 1));
	sld->base_point = sld->point;
	*value = data.g;
	sld->value = value;
	sld->limits = slider_limits;
	sld->active = 1;
	md->menu.slider_index++;
}

static void	init_sliders(t_md *md, t_parameters *pm, t_fx_data *fx)
{
	inisld(md, "Move speed", get_v4f(200, 800, 4000, 3801), &pm->plr_speed);
	inisld(md, "Rotation speed", get_v4f(.01, MOUSESPD, 1, 10), &pm->rot_speed);
	inisld(md, "Camera Height", get_v4f(0, 0, md->t_len, 100), &pm->height);
	inisld(md, "Camera bob", get_v4f(0, BOB_AMOUNT, 1, 100), &pm->bob_amount);
	inisld(md, "Cap FPS", get_v4f(1, 100, 100, 100), &pm->cap_fps);
	inisld(md, "Ray modulator", get_v4f(1, 5, 20, 100), &pm->ray_mod);
	inisld(md, "fov", get_v4f(1, 60, 600, 599), &pm->fov);
	inisld(md, "fov floor", get_v4f(.1, .5, 1.5, 100), &pm->floor_fov);
	inisld(md, "floor glide", get_v4f(0, 3, 3, 100), &pm->floor_glide);
	inisld(md, "Grass width", get_v4f(2, 2, 50, 49), &pm->grass_sz.x);
	inisld(md, "Grass height", get_v4f(1, 100, 400, 399), &pm->grass_sz.y);
	inisld(md, "Grass speed", get_v4f(0, .02, 3, 100), &pm->fe_speed);
	inisld(md, "text size", get_v4f(10, pm->txt_sc, 30, 20), &pm->txt_sc);
	inisld(md, "move drift", get_v4f(.1, ACCSPD, 1, 20), &pm->mov_drift);
	inisld(md, "ray len", get_v4f(0, 9999, 9999, 100), &pm->ray_depth);
	inisld(md, "Fog", get_v4f(0, .8, 3, 100), &md->fx.fog);
	inisld(md, "noise", get_v4f(0, 0, 1, 100), &fx->noise);
	inisld(md, "scanlines fx", get_v4f(0, 0, 1, 100), &fx->scanlines);
	inisld(md, "barrel fx", get_v4f(0, 0, 5, 100), &fx->barrel_amount);
	inisld(md, "Color band", get_v4f(0, 0, 1, 100), &fx->color_band);
	inisld(md, "Glow fx", get_v4f(0, 0, 1, 100), &fx->bloom_threshold);
	inisld(md, "contrast", get_v4f(-1, 0, 1, 201), &fx->contrast);
	inisld(md, "r", get_v4f(0, 1, 2, 100), &fx->hue.r);
	inisld(md, "g", get_v4f(0, 1, 2, 100), &fx->hue.g);
	inisld(md, "b", get_v4f(0, 1, 2, 100), &fx->hue.b);
}

static void	inibut(t_button *but, int *value, char *label, unsigned int map_c)
{
	char	*xtr_lbl;

	but->key_show[0] = '\0';
	but->key_trigger = map_c;
	xtr_lbl = ft_strchr(label, '_');
	if (xtr_lbl)
	{
		ft_strlcpy(but->key_show, label, \
			ft_strlen(label) - ft_strlen(xtr_lbl) + 1);
		ft_strlcpy(but->label, xtr_lbl + 1, 50);
	}
	else
		ft_strlcpy(but->label, label, 50);
	but->value = value;
	but->active = 1;
}

static void	init_buttons(t_md *md, t_menu *menu, t_parameters *prm, int i)
{
	i = 0;
	inibut(&menu->buttons[i++], &prm->debug_mode, "1_debug mode", _1_KEY);
	inibut(&menu->buttons[i++], &prm->view_2d, "2_Show Grid", _2_KEY);
	inibut(&menu->buttons[i++], &prm->show_rays, "3_show rays", _3_KEY);
	inibut(&menu->buttons[i++], &prm->show_walls, "4_show walls", _4_KEY);
	inibut(&menu->buttons[i++], &prm->show_ceiling, "5_Show ceiling", _5_KEY);
	inibut(&menu->buttons[i++], &prm->show_sky, "4_Show sky", _6_KEY);
	inibut(&menu->buttons[i++], &prm->show_wmap, "Y_Show Wmap", Y_KEY);
	inibut(&menu->buttons[i++], &prm->show_fps, "5_Show FPS", _7_KEY);
	inibut(&menu->buttons[i++], &prm->show_hud, "H_Show HUD", H_KEY);
	inibut(&menu->buttons[i++], &prm->show_floor, "B_Show floor", B_KEY);
	inibut(&menu->buttons[i++], &prm->ent_mode, "E_Show sprites", E_KEY);
	inibut(&menu->buttons[i++], &prm->show_grass, "G_Show grass", G_KEY);
	inibut(&menu->buttons[i++], &prm->alternate_draw, "O_ALTERNATE", O_KEY);
	inibut(&menu->buttons[i++], &prm->fly_cam, "F_Fly cam", F_KEY);
	inibut(&menu->buttons[i++], &prm->use_thrd, "T_Use Threads", T_KEY);
	inibut(&menu->buttons[i++], &prm->au_on, "U_audio", U_KEY);
	inibut(&menu->buttons[i++], &prm->super_view, "V_Super view", V_KEY);
	inibut(&menu->buttons[i++], &md->mmap.active, "M_Show minmap", M_KEY);
	inibut(&menu->buttons[i++], &md->mmap.cmps, "N_Change minimap", N_KEY);
	inibut(&menu->buttons[i++], &md->mouse.lock_rot.x, "X_x_mouse", X_KEY);
	inibut(&menu->buttons[i++], &md->mouse.lock_rot.y, "Y_y_mouse", Y_KEY);
	inibut(&menu->buttons[i++], &menu->active, "TAB_back", -1);
	menu->buttons[i].active = 0;
}

void	init_menu_elements(t_md *md, t_menu *menu)
{
	md->menu.slider_hov = -1;
	md->menu.button_hov = -1;
	menu->selected_slider = NULL;
	menu->slider_bgr_clr = _BLACK;
	menu->slider_fill_clr = _RED;
	menu->slider_index = 0;
	init_sliders(md, &md->prm, &md->fx);
	inisld(md, "Camera Zoom", get_v4f(0, 0, 3, 100), &md->prm.zoom);
	menu->sliders[menu->slider_index].active = 0;
	init_buttons(md, menu, &md->prm, 0);
}
