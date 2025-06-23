/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_background.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 22:45:20 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/11 19:42:58 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	draw_sky(t_md *md, t_image *bufr, t_vec3 *sky_pos)
{
	int		i;
	t_vec2	win_sz;
	t_vec3	trimm;
	t_vec2	pos;
	t_image	*to_draw;

	win_sz = md->win_sz;
	trimm = v3(md->win_sz.x, md->hud.floor_start, md->hud.bgr_color);
	i = -1;
	while (++i < 6)
	{
		pos = v2(sky_pos[i].x, sky_pos[i].y);
		if (pos.y < -win_sz.y || pos.y > md->hud.floor_start || \
			pos.y < -win_sz.x || pos.x > win_sz.x)
			continue ;
		to_draw = md->hud.sky;
		if (sky_pos[i].z)
			to_draw = md->hud.sky_flipy;
		draw_img(to_draw, bufr, pos, -1);
		continue ;
		draw_trimmed(to_draw, bufr, pos, trimm);
	}
}

int	render_sky(t_md *md)
{
	t_vec3	sky_pos[6];
	t_vec3f	speed;
	t_vec3f	offst;
	t_vec2	scrl;
	t_vec2	win_sz;

	speed = get_v3f(3, 1, 0);
	win_sz = md->win_sz;
	offst.x = win_sz.x * speed.x;
	offst.y = win_sz.y * speed.y;
	scrl.x = fmod((((md->cam.rot.x + 180.0)) / 360.0) * offst.x, win_sz.x);
	scrl.y = fmod((((md->cam.rot.y + 90.0)) / 180.0) * offst.y, win_sz.y);
	sky_pos[0] = v3(-scrl.x, -scrl.y, 0);
	sky_pos[1] = v3(-scrl.x, win_sz.y - scrl.y, 1);
	sky_pos[2] = v3(win_sz.x - scrl.x, -scrl.y, 0);
	sky_pos[3] = v3(win_sz.x - scrl.x, -win_sz.y - scrl.y, 0);
	sky_pos[4] = v3(-scrl.x, -scrl.y + win_sz.y, 1);
	sky_pos[5] = v3(win_sz.x - scrl.x, -scrl.y + win_sz.y, 1);
	draw_sky(md, md->hud.sky_buffer, sky_pos);
	return (1);
}

void	render_2d_floor(t_md *md)
{
	t_vec2	pos[9];
	t_vec2	centr;
	t_vec2	win_sz;
	t_vec3f	cam;
	int		i;

	win_sz = v2(md->win_sz.x * .5, md->win_sz.y * .5);
	cam = get_v3f(md->cam.ofst.x / md->t_len, md->cam.ofst.y / md->t_len, 0);
	centr.x = fmod(win_sz.x - md->txd.size_2d - (cam.x * md->txd.size_2d), \
		md->win_sz.x);
	centr.y = fmod(win_sz.y - md->txd.size_2d - (cam.y * md->txd.size_2d), \
		md->win_sz.y);
	pos[0] = v2(centr.x, centr.y + md->win_sz.y);
	pos[1] = v2(centr.x, centr.y - md->win_sz.y);
	pos[2] = v2(centr.x + md->win_sz.x, centr.y - md->win_sz.y);
	pos[3] = v2(centr.x + md->win_sz.x, centr.y + md->win_sz.y);
	pos[4] = v2(centr.x - md->win_sz.x, centr.y - md->win_sz.y);
	pos[5] = v2(centr.x - md->win_sz.x, centr.y + md->win_sz.y);
	pos[6] = v2(centr.x + md->win_sz.x, centr.y);
	pos[7] = v2(centr.x - md->win_sz.x, centr.y);
	pos[8] = centr;
	i = -1;
	while (++i < 9)
		if (v2_touch(pos[i], md->win_sz, _v2(0), md->win_sz))
			draw_img(md->hud.floor, md->screen, pos[i], md->hud.bgr_color);
}

void	render_background(t_md *md)
{
	t_hud	*hud;

	hud = &md->hud;
	if (md->prm.view_2d)
		render_2d_floor(md);
	if (!md->prm.show_ceiling)
	{
		if (md->prm.show_sky)
			render_sky(md);
		else
			draw_pixels(md->screen, \
	_v2(0), v2(md->win_sz.x, hud->floor_start + 1), hud->ceiling_color);
	}
	if (!md->prm.show_floor)
		draw_pixels(md->screen, \
			v2(0, hud->floor_start), md->win_sz, hud->floor_color);
}
