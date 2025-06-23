/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:59:44 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/29 12:39:35 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

int	get_trgb(unsigned char t, unsigned char r, unsigned char g, unsigned char b)
{
	return ((t << 24) | (r << 16) | (g << 8) | b);
}

unsigned char	get_t(int trgb)
{
	return ((trgb >> 24) & 0xFF);
}

unsigned char	get_r(int trgb)
{
	return ((trgb >> 16) & 0xFF);
}

unsigned char	get_g(int trgb)
{
	return ((trgb >> 8) & 0xFF);
}

unsigned char	get_b(int trgb)
{
	return (trgb & 0xFF);
}
