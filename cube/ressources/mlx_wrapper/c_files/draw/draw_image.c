/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_image.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:55:24 by gvalente          #+#    #+#             */
/*   Updated: 2025/05/05 11:04:50 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

static int	draw_clr_pxl(t_image *src, t_image *dst, t_vec3 data)
{
	t_vec2	pxl_i;

	if (dst->pos.x > dst->size.x)
		return (0);
	if (dst->pos.x < 0)
		return (1);
	pxl_i.x = src->pos.y * (src->size_line / 4) + src->pos.x;
	if (((src->src[pxl_i.x] >> 24) & 0xFF) == 255)
		return (1);
	pxl_i.y = dst->pos.y * (dst->size_line / 4) + dst->pos.x;
	if (src->src[pxl_i.x] == data.z)
		dst->src[pxl_i.y] = data.z;
	else
		dst->src[pxl_i.y] = blend_color(src->src[pxl_i.x], \
			data.x, (float)data.y / 10);
	return (1);
}

//	data: x = color | y = intensity | z = overlap_color
void	draw_clr_img(t_image *src, t_image *dst, t_vec2 pos, t_vec3 data)
{
	t_vec2	draw_pos;
	t_vec2	dst_pos;
	t_vec2	pxl_i;

	draw_pos = _v2(-1);
	while (++draw_pos.y < src->size.y)
	{
		dst_pos.y = pos.y + draw_pos.y;
		if (dst_pos.y < 0)
			continue ;
		if (dst->pos.y > dst->size.y)
			break ;
		draw_pos.x = -1;
		while (++draw_pos.x < src->size.x)
		{
			dst_pos.x = pos.x + draw_pos.x;
			src->pos = draw_pos;
			dst->pos = dst_pos;
			if (!draw_clr_pxl(src, dst, data))
				break ;
		}
	}
}

int	draw_img(t_image *src, t_image *dst, t_vec2 pos, int overlap_color)
{
	t_draw_d	draw_d;
	t_vec2		draw_pos;

	if (!src || !dst || !src->src || !dst->src)
		return (printf("Error\ndraw img on null >> %s\n", src->path), 0);
	draw_d.src = src;
	draw_d.dst = dst;
	draw_pos.y = -1;
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
			if (draw_d.dst_pos.x < 0 || draw_d.dst_pos.x >= dst->size.x)
				continue ;
			put_pxl_if_vis(&draw_d, overlap_color, 0, 0);
		}
	}
	return (1);
}

void	flush_img(t_image *src, int color, float transp, int ign_alpha_pxls)
{
	const int	total_pixels = src->size.x * src->size.y;
	int			*p;
	int			*end;

	p = src->src;
	end = src->src + total_pixels;
	if (color == -1)
		color = _BLACK;
	if (transp > 0)
		transp = 1 - transp;
	while (p < end)
	{
		if (ign_alpha_pxls && (*p & 0xFF000000) != 0x00000000)
		{
			p++;
			continue ;
		}
		if (transp >= 0 && color != _NULL)
			*p = blend_color(*p, color, transp);
		else
			*p = color;
		p++;
	}
}

void	draw_img_contour(t_md *md, t_image *src, t_vec2 pos, t_vec2 clr_thk)
{
	const t_vec2	new_size = add_vec2(src->size, _v2(clr_thk.y * 2));
	t_image			*outline;
	t_vec2			offset;

	outline = copy_image(md, src, new_size, -1);
	flush_img(outline, clr_thk.x, 10, 1);
	offset = sub_vec2(pos, _v2(clr_thk.y));
	draw_img(outline, md->screen, offset, -1);
	free_image_data(md, outline);
	draw_img(src, md->screen, pos, -1);
}
