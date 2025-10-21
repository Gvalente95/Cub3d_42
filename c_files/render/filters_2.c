/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filters_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 23:17:47 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/22 23:11:57 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	set_hue(t_image *img, t_vec4f rgb_factors)
{
	int				pixels;
	t_vec4			rgba;
	unsigned int	color;
	int				i;

	pixels = img->size.x * img->size.y;
	i = -1;
	while (++i < pixels)
	{
		color = img->src[i];
		rgba.r = ((color >> 16) & 0xFF) * rgb_factors.r;
		rgba.g = ((color >> 8) & 0xFF) * rgb_factors.g;
		rgba.b = (color & 0xFF) * rgb_factors.b;
		rgba.a = (color >> 24) & 0xFF;
		rgba.r = minmaxf(0, 255, rgba.r);
		rgba.g = minmaxf(0, 255, rgba.g);
		rgba.b = minmaxf(0, 255, rgba.b);
		color = (rgba.a << 24) | (rgba.r << 16) | (rgba.g << 8) | rgba.b;
		img->src[i] = color;
	}
}

void	apply_contrast(t_image *img, float contrast)
{
	int				pixels;
	t_vec4			rgba;
	unsigned int	color;
	int				i;
	const int		increment = contrast * 255;

	pixels = img->size.x * img->size.y;
	i = -1;
	while (++i < pixels)
	{
		color = img->src[i];
		rgba.r = ((color >> 16) & 0xFF) + increment;
		rgba.g = ((color >> 8) & 0xFF) + increment;
		rgba.b = (color & 0xFF) + increment;
		rgba.a = (color >> 24) & 0xFF;
		rgba.r = minmaxf(0, 255, rgba.r);
		rgba.g = minmaxf(0, 255, rgba.g);
		rgba.b = minmaxf(0, 255, rgba.b);
		color = (rgba.a << 24) | (rgba.r << 16) | (rgba.g << 8) | rgba.b;
		img->src[i] = color;
	}
}
