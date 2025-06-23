/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_pixels_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 13:54:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/01 15:25:00 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

float	get_grad_t(t_dir dir, t_vec2 p, t_vec2 sz)
{
	if (dir == right)
		return ((float)p.x / (float)(sz.x - 1));
	else if (dir == left)
		return ((float)(sz.x - p.x) / (float)(sz.x - 1));
	else if (dir == down)
		return ((float)p.y / (float)(sz.y - 1));
	return ((float)(sz.y - p.y) / (float)(sz.y - 1));
}

void	draw_grad_pxls(t_image *dst, t_vec2 pos, t_vec2 sz, t_vec3 clr)
{
	t_vec2	p;
	float	t;
	int		grad_color;
	int		*dst_src;
	t_dir	dir;

	dir = clr.z;
	dst_src = dst->src;
	p = _v2(-1);
	while (++p.y < sz.y)
	{
		p.x = -1;
		while (++p.x < sz.x)
		{
			t = get_grad_t(dir, p, sz);
			if (clr.y == _NULL)
			{
				draw_pixel(dst, add_vec2(pos, p), clr.x, t);
				continue ;
			}
			grad_color = blend_color(clr.x, clr.y, t);
			draw_pixel(dst, add_vec2(pos, p), grad_color, 1);
		}
	}
}

void	draw_from_pos(t_image *src, t_image *dst, t_vec2 pos, t_vec2 draw_start)
{
	t_draw_d	draw_d;

	if (!src || !dst || !src->src || !dst->src)
		return ;
	draw_d.src = src;
	draw_d.dst = dst;
	draw_d.pos = draw_start;
	while (++draw_d.pos.y < src->size.y)
	{
		draw_d.src_pos.y = draw_d.pos.y;
		draw_d.dst_pos.y = pos.y + draw_d.pos.y;
		if (draw_d.dst_pos.y < 0 || draw_d.dst_pos.y >= dst->size.y)
			continue ;
		draw_d.pos.x = -1;
		while (++draw_d.pos.x < src->size.x)
		{
			draw_d.src_pos.x = draw_d.pos.x;
			draw_d.dst_pos.x = pos.x + draw_d.pos.x;
			if (src->src[draw_d.pos.y * \
				(src->size_line / 4) + draw_d.pos.x] == _NULL)
				continue ;
			put_pxl_if_vis(&draw_d, -1, 0, 0);
		}
	}
}

void	draw_img_except(t_image *src, t_image *dst, t_vec2 pos, int excpt)
{
	t_vec2	draw_p;
	t_vec2	dst_p;
	t_vec2	lim;
	int		src_c;
	int		dst_i;

	lim.x = min(src->size.x, dst->size.x - pos.x);
	lim.y = min(src->size.y, dst->size.y - pos.y);
	draw_p.y = -1;
	while (++draw_p.y < lim.y)
	{
		draw_p.x = -1;
		while (++draw_p.x < lim.x)
		{
			dst_p.x = pos.x + draw_p.x;
			dst_p.y = pos.y + draw_p.y;
			src_c = src->src[draw_p.y * (src->size_line / 4) + draw_p.x];
			if (src_c == excpt)
				continue ;
			dst_i = dst_p.y * (dst->size_line / 4) + dst_p.x;
			dst->src[dst_i] = blend_color(src_c, dst->src[dst_i], \
				((src_c >> 24) & 0xFF) / 255.0f);
		}
	}
}
