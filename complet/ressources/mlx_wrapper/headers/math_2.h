/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_2.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 15:43:06 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/25 00:18:03 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_2_H
# define MATH_2_H

static inline float	minmaxf(float min, float max, float value)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

static inline int	ft_sign(float a)
{
	if (a > EPSILON)
		return (1);
	if (a < EPSILON)
		return (-1);
	return (0);
}

static inline int	safe_mod(int a, int b)
{
	return (((a % b) + b) % b);
}

static inline float	lerp(float a, float b, float t)
{
	return (a + (b - a) * t);
}

#endif