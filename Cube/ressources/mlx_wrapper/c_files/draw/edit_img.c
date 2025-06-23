/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edit_img.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:12:41 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 09:51:04 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

void	flipx_image_data(t_image *img)
{
	t_vec2	pos;
	int		tmp_pxl;
	int		end_pxl;
	int		cur_index;
	int		end_index;

	pos = v2(-1, -1);
	while (++pos.y < img->size.y)
	{
		pos.x = -1;
		while (++pos.x < img->size.x / 2)
		{
			cur_index = pos.y * (img->size_line / 4) + pos.x;
			end_index = pos.y * (img->size_line / 4) + \
			(img->size_line / 4 - 1 - pos.x);
			tmp_pxl = img->src[cur_index];
			img->src[cur_index] = img->src[end_index];
			img->src[end_index] = tmp_pxl;
		}
	}
}

void	flipy_image_data(t_image *img)
{
	t_vec2	pos;
	int		top_index;
	int		bot_index;
	int		tmp_pxl;
	int		width;

	width = img->size_line / 4;
	pos.y = -1;
	while (++pos.y < img->size.y / 2)
	{
		pos.x = -1;
		while (++pos.x < img->size.x)
		{
			top_index = pos.y * width + pos.x;
			bot_index = (img->size.y - 1 - pos.y) * width + pos.x;
			tmp_pxl = img->src[top_index];
			img->src[top_index] = img->src[bot_index];
			img->src[bot_index] = tmp_pxl;
		}
	}
}

void	clean_img(t_image *img)
{
	int			bg;
	const int	total_pixels = img->size.x * img->size.y;
	int			i;
	int			color;
	t_vec4		rgb;

	if (!img || !img->src)
		return ;
	i = -1;
	while (++i < total_pixels)
	{
		color = img->src[i];
		if ((color & 0xFF000000) != 0x00000000)
			continue ;
		rgb = color_to_v4(color);
		if (rgb.r > 200 && rgb.g > 200 && rgb.b > 200)
			continue ;
		color = v4_to_color(0, 0, 0, 255);
		img->src[i] = color;
	}
}

void	replace_image(t_md *md, t_image **dst, t_image *src)
{
	if (!md || !dst || !src)
		return ;
	if (*dst)
		free_image_data(md, *dst);
	*dst = copy_image(md, src, src->size, -1);
}

void	set_img_color(t_image *img_data, t_vec2 size, int col, float str)
{
	t_vec2	pos;
	t_vec2	ratio;
	t_vec4	rgb;
	double	x_rat;
	int		pixel_index;

	pos.y = -1;
	while (++pos.y < size.y)
	{
		pos.x = -1;
		while (++pos.x < size.x)
		{
			pixel_index = (pos.y * img_data->size_line / 4) + pos.x;
			x_rat = img_data->src[pixel_index];
			if (get_t(x_rat) == 0)
			{
				rgb.r = (int)((get_r(x_rat) * (1 - str)) + (get_r(col) * str));
				rgb.g = (int)((get_g(x_rat) * (1 - str)) + (get_g(col) * str));
				rgb.b = (int)((get_b(x_rat) * (1 - str)) + (get_b(col) * str));
				img_data->src[pixel_index] = \
					get_trgb(get_t(x_rat), rgb.r, rgb.g, rgb.a);
			}
		}
	}
}
