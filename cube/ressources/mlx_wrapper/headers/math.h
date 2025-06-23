/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 15:40:52 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/11 12:04:27 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_H
# define MATH_H

static inline int	max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

static inline int	min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

static inline int	minmax(int min, int max, int value)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

static inline float	maxf(float a, float b)
{
	if (a > b)
		return (a);
	return (b);
}

static inline float	minf(float a, float b)
{
	if (a < b)
		return (a);
	return (b);
}

unsigned int	get_r_seed(void);

#endif