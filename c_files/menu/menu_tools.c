/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:50:28 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/26 22:03:08 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	render_logo_cube(t_md *md, t_menu *menu)
{
	const t_vec2	pos = v2(400, 100);
	const t_vec2	size = v2(50, 50);
	int				color;

	color = _NULL;
	draw_cube(md->menu.ui_overlay, v3(pos.x, pos.y, color), size, \
		menu->cube_logo_rot);
}

void	update_logo_cube(t_md *md, t_mouse mouse, t_menu *menu)
{
	const t_vec2	size = v2(100, 100);
	const t_vec2	mouse_p = v2(mouse.real.x, mouse.real.y);
	const t_vec2	pos = v2(350, 50);

	(void)md;
	if (v2_bounds(mouse_p, pos, size))
	{
		menu->cube_logo_rot.x -= mouse.delta.x;
		menu->cube_logo_rot.y += mouse.delta.y;
		menu->refresh_ui = 1;
		menu->refresh_bg = 1;
	}
	else
	{
		menu->cube_logo_rot.x += (md->timer.time % 10) * .3;
		menu->cube_logo_rot.y += (md->timer.time % 20) * .3;
		menu->refresh_ui = 1;
		menu->refresh_bg = 1;
	}
}

void	update_menu_element_end(t_menu *menu, int *hov, int cur_hov)
{
	if (*hov == -1 || cur_hov != -1)
		return ;
	*hov = -1;
	menu->refresh_ui = 1;
	menu->refresh_bg = 1;
}

void	center_menu_txt(t_md *md, t_vec2 pos_ofst, int scale, char *name)
{
	const t_vec2	win_cntr = v2(md->win_sz.x / 2, md->win_sz.y / 2);
	t_vec2			pos;
	const int		title_color = -1;
	t_txtd			txt_data;

	pos.x = win_cntr.x - scale * (ft_strlen(name) / 2) + pos_ofst.x;
	pos.y = win_cntr.y - scale / 2 + pos_ofst.y;
	txt_data = (t_txtd){pos.x, pos.y, title_color, scale, NULL};
	rnd_fast_txt(md, txt_data, name);
}

void	set_wheel(t_clrp *w, t_vec2 mouse_pos, t_vec2 img_center)
{
	t_vec2	dt;
	float	norm_dist;
	float	angle;
	t_vec3f	hu_st_v;
	t_vec4f	rgb;

	dt = sub_vec2(mouse_pos, img_center);
	norm_dist = (sqrtf(dt.x * dt.x + dt.y * dt.y)) / (w->size.x / 2.0f);
	if (norm_dist > 1.0f)
		return ;
	if (norm_dist > .8)
		*w->color = _BLACK;
	else
	{
		angle = atan2f(-dt.y, -dt.x);
		if (angle < 0)
			angle += 2 * _PI;
		hu_st_v.x = angle / (2 * _PI);
		hu_st_v.y = norm_dist;
		hu_st_v.z = 1.0f;
		hsv_to_rgb(hu_st_v.x, hu_st_v.y, hu_st_v.z, &rgb);
		*w->color = v4_to_color(rgb.r * 255, rgb.g * 255, rgb.b * 255, 255);
	}
	w->mouse_touch = mouse_pos;
}
