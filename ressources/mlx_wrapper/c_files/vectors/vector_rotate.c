/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_rotate.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 21:32:19 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/11 12:10:06 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

t_vec3f	rotate_y(t_vec3f v, float angle)
{
	const float	s = sinf(angle);
	const float	c = cosf(angle);

	return ((t_vec3f){v.x, v.y * c - v.z * s, v.y * s + v.z * c});
}

t_vec3f	rotate_x(t_vec3f v, float angle)
{
	const float	s = sinf(angle);
	const float	c = cosf(angle);

	return (((t_vec3f){v.x * c + v.z * s, v.y, -v.x * s + v.z * c}));
}

t_vec3f	rotate_z(t_vec3f v, float angle)
{
	const float	s = sinf(angle);
	const float	c = cosf(angle);

	return (((t_vec3f){v.x * c - v.y * s, v.x * s + v.y * c, v.z}));
}

t_vec3f	rotate_vec3f(t_vec3f v, t_vec3f rot)
{
	v = rotate_x(v, rot.x);
	v = rotate_y(v, rot.y);
	v = rotate_z(v, rot.z);
	return (v);
}

t_vec2f	rotate_vec2f(t_vec2f v, float angle)
{
	t_vec2f	r;

	r.x = v.x * cosf(angle) - v.y * sinf(angle);
	r.y = v.x * sinf(angle) + v.y * cosf(angle);
	return (r);
}
