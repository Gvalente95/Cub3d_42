/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:16:08 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/09 20:19:09 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

void	init_draw_d(t_draw_d *draw_d, t_image *src, t_image *dst, \
	t_vec2 src_pos)
{
	draw_d->src = src;
	draw_d->dst = dst;
	draw_d->src_pos = src_pos;
}

int	color_diff(int c1, int c2)
{
	t_vec4	delta_;

	delta_.r = ((c1 >> 16) & 0xFF) - ((c2 >> 16) & 0xFF);
	delta_.g = ((c1 >> 8) & 0xFF) - ((c2 >> 8) & 0xFF);
	delta_.b = (c1 & 0xFF) - (c2 & 0xFF);
	return (delta_.r * delta_.r + delta_.g * delta_.g + delta_.b * delta_.b);
}

int	set_alpha(int color, float new_alpha)
{
	int	alpha;

	if (new_alpha < 0.0f)
		new_alpha = 0.0f;
	if (new_alpha > 1.0f)
		new_alpha = 1.0f;
	new_alpha = 1 - new_alpha;
	alpha = (int)(new_alpha * 255.0f);
	color = (color & 0x00FFFFFF) | (alpha << 24);
	return (color);
}

float	get_alpha(int color)
{
	int	alpha;

	alpha = (color >> 24) & 0xFF;
	return (255.0f - (float)alpha / 255.0f);
}

void	fill_transparency(t_image *src, int color)
{
	int		*img_data;
	int		total_pixels;
	int		i;

	if (!src || !src->src)
		return ;
	if (color == -1)
		color = 0x000000;
	img_data = src->src;
	total_pixels = src->size.x * src->size.y;
	i = -1;
	while (++i < total_pixels)
	{
		if ((img_data[i] & 0xFF000000) == 0x00000000)
			continue ;
		img_data[i] = color;
	}
}
