/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_3floatb.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:16:50 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/17 14:47:54 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"
#include "../../../cube.h"

t_vec3f	v3f_center(t_vec3f a, t_vec3f b)
{
	return (get_v3f(a.x / 2 - b.x / 2, a.y / 2 - b.y / 2, a.z / 2 - b.z / 2));
}

int	v3f_bounds(t_vec3f a, t_vec3f a_size, t_vec3f pos, t_vec3f b_size)
{
	return (a.x + a_size.x >= pos.x && \
		a.x <= pos.x + b_size.x && \
		a.y + a_size.y >= pos.y && \
		a.y <= pos.y + b_size.y);
}

t_vec3f	normalize_vec3f(t_vec3f v)
{
	float	length;

	length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (length == 0.0f)
		return ((t_vec3f){0, 0, 0});
	return ((t_vec3f){v.x / length, v.y / length, v.z / length});
}

t_vec3f	div_v3f(t_vec3f a, float div)
{
	return ((t_vec3f){a.x / div, a.y / div, a.z / div});
}
