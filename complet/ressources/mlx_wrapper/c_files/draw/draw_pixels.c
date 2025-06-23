/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_pixels.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:55:24 by gvalente          #+#    #+#             */
/*   Updated: 2025/05/26 22:20:01 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

int	put_pxl_if_vis(t_draw_d *dd, int overlap, int blend, float opacty)
{
	t_image	*src_img;
	t_image	*dst;
	int		src_i;
	int		dst_i;
	int		clr;

	if ((dd->dst_pos.y < 0 || dd->dst_pos.y > dd->dst->size.y) || \
	dd->dst_pos.x < 0 || dd->dst_pos.x >= dd->dst->size.x)
		return (0);
	src_img = dd->src;
	dst = dd->dst;
	dst_i = dd->dst_pos.y * (dst->size_line / 4) + dd->dst_pos.x;
	if (overlap != -1 && dst->src[dst_i] != (unsigned int)overlap)
		return (0);
	clr = dd->src_pos.x;
	if (src_img && src_img->src)
	{
		src_i = dd->src_pos.y * (src_img->size_line / 4) + dd->src_pos.x;
		if (((src_img->src[src_i] >> 24) & 0xFF) == 255)
			return (0);
		clr = src_img->src[src_i];
	}
	if (blend)
		return (dst->src[dst_i] = blend_color(dst->src[dst_i], clr, opacty), 1);
	return (dst->src[dst_i] = clr, 1);
}

int	draw_pixel(t_image *dst, t_vec2 pos, int color, float opacity)
{
	int	index;

	if (!dst || !dst->src)
		return (0);
	if (pos.x < 0 || pos.x >= dst->size.x || \
		pos.y < 0 || pos.y >= dst->size.y)
		return (0);
	index = pos.y * (dst->size_line / 4) + pos.x;
	if (opacity < 0.0f)
		dst->src[index] = color;
	else
		dst->src[index] = blend_color(dst->src[index], color, opacity);
	return (1);
}

int	draw_pixels(t_image *dst, t_vec2 pos, t_vec2 draw_size, int color)
{
	int		idx;
	int		line_size;
	t_vec2	p;
	t_vec2	limit;
	float	alpha;

	if (pos.x + draw_size.x < 0 || pos.y + draw_size.y < 0)
		return (0);
	line_size = dst->size_line / 4;
	alpha = maxf(0.2f, 1.0f - ((float)((color >> 24) & 0xFF) / 255.0f));
	limit.x = min(dst->size.x, pos.x + draw_size.x);
	limit.y = min(dst->size.y, pos.y + draw_size.y);
	p.y = pos.y - 1;
	while (++p.y < limit.y)
	{
		p.x = pos.x - 1;
		while (++p.x < limit.x)
		{
			idx = p.y * line_size + p.x;
			dst->src[idx] = blend_color(dst->src[idx], color, alpha);
		}
	}
	return (1);
}

//		clr_ign.x = color | clr_ign.y = drawover
int	draw_safe_pxl(t_image *img, t_vec2 pos, t_vec2 clr_over, float opacity)
{
	int	index;
	int	cur_clr;

	if (!img || !img->src)
		return (0);
	if (pos.x < 0 || pos.x >= img->size.x || \
		pos.y < 0 || pos.y >= img->size.y)
		return (0);
	if ((img->size_line / 4) != img->size.x)
		return (printf("Warning: size_line / 4 != size.x\n"), 0);
	index = pos.y * (img->size_line / 4) + pos.x;
	cur_clr = img->src[index];
	if (cur_clr != clr_over.y)
		return (0);
	if (opacity < 0)
		img->src[index] = clr_over.x;
	else
		img->src[index] = blend_color(img->src[index], clr_over.x, opacity);
	return (1);
}

int	draw_safe_pxls(t_image *txtr, t_vec2 pos, t_vec2 draw_size, int color)
{
	t_vec2	draw_pos;
	t_vec2	end_coord;
	int		draw_count;
	float	alpha;

	if (!txtr || !txtr->src || pos.x < 0 || pos.y < 0)
		return (0);
	alpha = 1.0f - ((float)((color >> 24) & 0xFF) / 255.0f);
	if (alpha < 0.2f)
		alpha = 0.2f;
	end_coord = v2(pos.x + draw_size.x, pos.y + draw_size.y);
	draw_count = 0;
	draw_pos = pos;
	while (draw_pos.y < end_coord.y)
	{
		draw_pos.x = pos.x - 1;
		while (++draw_pos.x < end_coord.x)
			draw_count += draw_safe_pxl(txtr, draw_pos, \
				v2(color, v4_to_color(0, 0, 0, 0)), alpha);
		draw_pos.y++;
	}
	return (draw_count);
}
