/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_background.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 02:19:07 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/11 19:47:45 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	init_clouds(t_image *sky, t_vec2 win_sz)
{
	int		base_color;
	int		i;
	t_vec2	r_pos;

	base_color = _WHITE;
	i = -1;
	while (++i < 50)
	{
		r_pos = v2(r_range(30, win_sz.x * .7), r_range(30, win_sz.y * .6));
		draw_sphere(sky, r_pos, \
v2(win_sz.x / 3, win_sz.y / 6), v3(base_color, 2, 1));
	}
}

static void	init_nebulas(t_image *sky, t_vec2 win_sz)
{
	t_vec4	rgb;
	int		base_color;
	int		i;
	t_vec2	r_pos;

	i = -1;
	while (++i < 50)
	{
		r_pos = v2(r_range(0, win_sz.x * .7), r_range(0, win_sz.y * .6));
		rgb = v4(r_range(120, 120), r_range(0, 100), \
			r_range(150, 255), 255);
		base_color = v4_to_color(rgb.r, rgb.g, rgb.b, rgb.a);
		draw_sphere(sky, r_pos, \
_v2(r_range(win_sz.x * .2, win_sz.x * .3)), v3(base_color, 2, 1));
	}
}

static void	darken_towards_dir(t_image *img, t_dir dir, float intensity)
{
	t_vec2	pos;
	int		index;
	int		factor;
	int		color;
	t_vec4	rgb;

	if (dir == up)
		pos.y = img->size.y;
	else
		pos.y = -1;
	while ((dir == up && --pos.y >= 0) || (dir != up && ++pos.y < img->size.y))
	{
		factor = ((pos.y * 255) / img->size.y) * intensity;
		pos.x = -1;
		while (++pos.x < img->size.x)
		{
			index = pos.x + pos.y * (img->size_line / 4);
			color = img->src[index];
			rgb = color_to_v4(color);
			rgb.r = max(0, rgb.r - factor);
			rgb.g = max(0, rgb.g - factor);
			rgb.b = max(0, rgb.b - factor);
			img->src[index] = v4_to_color(rgb.r, rgb.g, rgb.b, 255);
		}
	}
}

static void	init_sky(t_md *md, t_hud *hud, t_vec2 win_sz)
{
	int		i;
	t_vec2	r_pos;
	int		base_color;
	int		rad;

	hud->base_sky = init_img(md, md->win_sz, NULL, hud->ceiling_color);
	hud->sky = init_img(md, md->win_sz, NULL, hud->ceiling_color);
	darken_towards_dir(hud->sky, left, .5);
	init_nebulas(hud->sky, win_sz);
	i = -1;
	while (++i < STARS_AMOUNT)
	{
		base_color = v4_to_color(r_range(150, 254), \
			r_range(50, 120), r_range(150, 254), 20);
		rad = r_range(3, 10);
		r_pos = v2(r_range(10, win_sz.x - 10), r_range(0, win_sz.y));
		draw_sphere(md->hud.sky, r_pos, _v2(rad), \
			v3(base_color, r_range(0, 2), 1));
		draw_pixel(md->hud.sky, v2(r_pos.x + rad / 2, r_pos.y + rad / 2), \
			_WHITE, f_range(0.3, 1));
	}
	hud->sky_flipy = copy_image(md, hud->sky, _v2(-1), -1);
	hud->sky_buffer = copy_image(md, hud->sky, _v2(-1), -1);
	flipy_image_data(hud->sky_flipy);
}

void	init_background(t_md *md, t_hud *hud, t_vec2 win_sz)
{
	hud->ceiling = init_img(md, _v2(md->t_len), "utils/ceiling.xpm", -1);
	hud->floor = init_img(md, _v2(md->t_len), "utils/floor.xpm", -1);
	hud->wall = init_img(md, _v2(md->t_len), "utils/wall.xpm", -1);
	hud->floor2d = copy_image(md, hud->wall, _v2(md->txd.size_2d), -1);
	flush_img(hud->floor, hud->floor_color, 0.5, 0);
	init_sky(md, hud, win_sz);
}
