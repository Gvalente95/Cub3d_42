/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_color.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 10:42:14 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 09:51:04 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

void	remove_img_color(t_image *img, int remove_color)
{
	int		*img_data;
	int		total_pixels;
	int		i;
	t_vec4	target;
	t_vec4	clr;

	if (!img || !img->src)
		return ;
	total_pixels = img->size.x * img->size.y;
	i = -1;
	while (++i < total_pixels)
	{
		clr = color_to_v4(img->src[i]);
		target = color_to_v4(remove_color);
		if (clr.r == target.r && clr.g == target.g && clr.b == target.b)
			img->src[i] = 0xFF000000;
	}
}

int	blend_color(int a, int b, float o)
{
	t_vec4	rgb_a;
	t_vec4	rgb_b;
	t_vec4	ab;

	if (o <= 0.0f)
		return (a);
	if (o >= 1.0f)
		return (b);
	rgb_a.a = (a >> 24) & 0xFF;
	rgb_a.r = (a >> 16) & 0xFF;
	rgb_a.g = (a >> 8) & 0xFF;
	rgb_a.b = a & 0xFF;
	rgb_b.a = (b >> 24) & 0xFF;
	rgb_b.r = (b >> 16) & 0xFF;
	rgb_b.g = (b >> 8) & 0xFF;
	rgb_b.b = b & 0xFF;
	ab.r = rgb_a.r * (1.0f - o) + rgb_b.r * o;
	ab.g = rgb_a.g * (1.0f - o) + rgb_b.g * o;
	ab.b = rgb_a.b * (1.0f - o) + rgb_b.b * o;
	ab.a = rgb_a.a * (1.0f - o) + rgb_b.a * o;
	return ((ab.a << 24) | (ab.r << 16) | (ab.g << 8) | ab.b);
}

//		color_d: x = colr - y = thickness
void	draw_line(t_image *onto, t_vec2 start, t_vec2 end, t_vec2 color_d)
{
	t_vec2	delta;
	t_vec2f	step;
	float	steps;
	float	i;
	t_vec2f	pos;

	delta = sub_vec2(end, start);
	steps = fmaxf(abs(delta.x), abs(delta.y));
	if (steps == 0)
		return ;
	step = get_v2f(delta.x / steps, delta.y / steps);
	pos = get_v2f(start.x, start.y);
	i = -1;
	while (++i <= steps)
	{
		start = (t_vec2){(int)(pos.x + 0.5f), (int)(pos.y + 0.5f)};
		if (get_alpha(onto->src[start.y * \
			(onto->size_line / 4) + start.x]) == 0)
			break ;
		if (color_d.y > 1)
			draw_pixels(onto, start, _v2(color_d.y), color_d.x);
		else
			draw_pixel(onto, start, color_d.x, get_alpha(color_d.x));
		pos = add_vec2f(pos, step);
	}
}

void	draw_alpha_img(t_image *src, t_image *dst, t_vec2 pos, float trnsp)
{
	t_draw_d	draw_d;
	t_vec2		draw_pos;

	if (!src || !dst || !src->src || !dst->src)
		return ;
	draw_d.src = src;
	draw_d.dst = dst;
	draw_pos = v2(-1, -1);
	while (++draw_pos.y < src->size.y)
	{
		draw_d.src_pos.y = draw_pos.y;
		draw_d.dst_pos.y = pos.y + draw_pos.y;
		if (draw_d.dst_pos.y < 0 || draw_d.dst_pos.y >= dst->size.y)
			continue ;
		draw_pos.x = -1;
		while (++draw_pos.x < src->size.x)
		{
			draw_d.src_pos.x = draw_pos.x;
			draw_d.dst_pos.x = pos.x + draw_pos.x;
			put_pxl_if_vis(&draw_d, -1, 1, trnsp);
		}
	}
}

void	hsv_to_rgb(float h, float s, float v, t_vec4f *rgb_v)
{
	const float	c = v * s;
	const float	x = c * (1.0f - fabsf(fmodf(h * 6.0f, 2.0f) - 1.0f));
	const float	m = v - c;
	t_vec4f		rgb;

	if (h < 1.0f / 6.0f)
		rgb = (t_vec4f){c, x, 0, 1};
	else if (h < 2.0f / 6.0f)
		rgb = (t_vec4f){x, c, 0, 1};
	else if (h < 3.0f / 6.0f)
		rgb = (t_vec4f){0, c, x, 1};
	else if (h < 4.0f / 6.0f)
		rgb = (t_vec4f){0, x, c, 1};
	else if (h < 5.0f / 6.0f)
		rgb = (t_vec4f){x, 0, c, 1};
	else
		rgb = (t_vec4f){c, 0, x, 1};
	rgb_v->r = rgb.r + m;
	rgb_v->g = rgb.g + m;
	rgb_v->b = rgb.b + m;
}
