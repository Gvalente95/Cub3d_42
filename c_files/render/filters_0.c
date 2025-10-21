/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filters_0.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 12:51:08 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 09:51:04 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	apply_d2(t_image *img, t_vec2 pos, t_vec3 *ij, t_vec4f *rgb)
{
	t_vec2			p;
	unsigned int	pixel;

	p = v2(pos.x + ij->x, pos.y + ij->y);
	if (p.x < 0 || p.x >= img->size.x || p.y < 0 || p.y >= img->size.y)
		return ;
	pixel = img->src[p.y * img->size.x + p.x];
	rgb->r += (pixel >> 16) & 0xFF;
	rgb->g += (pixel >> 8) & 0xFF;
	rgb->b += pixel & 0xFF;
	rgb->a += (pixel >> 24) & 0xFF;
	ij->z++;
}

void	apply_d(t_image *img, unsigned int *new_d, t_vec2 pos, float half_krnfl)
{
	t_vec4f			rgb;
	t_vec3			ij;
	unsigned int	rgba[4];

	rgb = get_v4f(0, 0, 0, 0);
	ij.z = 0;
	ij.x = -half_krnfl - 1;
	while (++ij.x <= half_krnfl)
	{
		ij.y = -half_krnfl - 1;
		while (++ij.y <= half_krnfl)
			apply_d2(img, pos, &ij, &rgb);
	}
	if (ij.z == 0)
		ij.z = 1;
	rgba[0] = minmaxf(0, 255, rgb.r / (float)ij.z);
	rgba[1] = minmaxf(0, 255, rgb.g / (float)ij.z);
	rgba[2] = minmaxf(0, 255, rgb.b / (float)ij.z);
	rgba[3] = minmaxf(0, 255, rgb.a / (float)ij.z);
	new_d[pos.y * img->size.x + pos.x] = \
		(rgba[3] << 24) | (rgba[0] << 16) | (rgba[1] << 8) | rgba[2];
}

void	apply_antialiasing(t_image *img)
{
	t_vec2			pos;
	float			half_kernel;
	unsigned int	*new_data;
	t_vec2			size;

	size = img->size;
	half_kernel = 3.0 / 2.0;
	new_data = malloc(size.x * size.y * sizeof(unsigned int));
	if (!new_data)
		return ;
	pos = v2(-1, -1);
	while (++pos.y < size.y)
	{
		pos.x = -1;
		while (++pos.x < size.x)
			apply_d(img, new_data, pos, half_kernel);
	}
	ft_memcpy(img->src, new_data, size.x * size.y * sizeof(unsigned int));
	free(new_data);
}

void	apply_scanlines(t_image *img, float factor)
{
	t_vec2			pos;
	unsigned char	rgba[4];
	unsigned int	pixel;

	pos.y = -1;
	while (++pos.y < img->size.y)
	{
		pos.x = -1;
		while (++pos.x < img->size.x)
		{
			if (pos.y % 2 == 0)
				continue ;
			pixel = img->src[pos.y * img->size.x + pos.x];
			rgba[0] = (pixel >> 16) & 0xFF;
			rgba[1] = (pixel >> 8) & 0xFF;
			rgba[2] = pixel & 0xFF;
			rgba[3] = (pixel >> 24) & 0xFF;
			rgba[0] = (int)(rgba[0] * (1.0f - factor));
			rgba[1] = (int)(rgba[1] * (1.0f - factor));
			rgba[2] = (int)(rgba[2] * (1.0f - factor));
			img->src[pos.y * img->size.x + pos.x] = \
			(rgba[3] << 24) | (rgba[0] << 16) | (rgba[1] << 8) | rgba[2];
		}
	}
}

void	apply_noise(t_md *md, t_image *img, float factor, float colors_amount)
{
	int		i;
	t_vec4	rgb;
	int		dspl;
	int		total_pixels;
	int		gray;

	dspl = (int)(factor * 100);
	total_pixels = img->size.y * (img->size_line / 4);
	i = -1;
	while (++i < total_pixels)
	{
		rgb = color_to_v4(img->src[i]);
		rgb.r = minmax(0, 255, rgb.r + r_range_seed(&md->r_seed, -dspl, dspl));
		rgb.g = minmax(0, 255, rgb.g + r_range_seed(&md->r_seed, -dspl, dspl));
		rgb.b = minmax(0, 255, rgb.b + r_range_seed(&md->r_seed, -dspl, dspl));
		gray = (rgb.r + rgb.g + rgb.b) / 3;
		rgb.r = (rgb.r * colors_amount) + (gray * (1.0f - colors_amount));
		rgb.g = (rgb.g * colors_amount) + (gray * (1.0f - colors_amount));
		rgb.b = (rgb.b * colors_amount) + (gray * (1.0f - colors_amount));
		img->src[i] = v4_to_color((int)rgb.r, (int)rgb.g, (int)rgb.b, 255);
	}
}
