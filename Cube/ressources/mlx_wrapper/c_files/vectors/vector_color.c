/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_color.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 12:10:30 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 09:51:56 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

int	v4_to_color(int r, int g, int b, int a)
{
	t_vec4		rgba;

	a = 255 - a;
	rgba = v4(r, g, b, a);
	return ((rgba.a << 24) | (rgba.r << 16) | (rgba.g << 8) | rgba.b);
}

t_vec4	color_to_v4(int color)
{
	t_vec4	clr;

	clr.a = (color >> 24) & 0xFF;
	clr.r = (color >> 16) & 0xFF;
	clr.g = (color >> 8) & 0xFF;
	clr.b = color & 0xFF;
	return (clr);
}

int	color_compare(int color, t_vec4 cmp)
{
	t_vec4	clr_rgb;

	clr_rgb = color_to_v4(color);
	if (clr_rgb.r == cmp.r && clr_rgb.b == cmp.b && clr_rgb.b == cmp.b)
		return (1);
	return (0);
}
