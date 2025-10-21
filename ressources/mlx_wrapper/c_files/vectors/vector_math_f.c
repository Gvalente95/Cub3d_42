/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_math_f.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:55:28 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 09:51:04 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

void	swap_vec2(t_vec2 *a, t_vec2 *b)
{
	t_vec2	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

t_vec3f	cross_vec3f(t_vec3f a, t_vec3f b)
{
	t_vec3f	r;

	r.x = a.y * b.z - a.z * b.y;
	r.y = a.z * b.x - a.x * b.z;
	r.z = a.x * b.y - a.y * b.x;
	return (r);
}

t_vec2	project(t_vec3f p, t_vec2 origin)
{
	const float	fov = 360.0f;
	float		scale;

	if (p.z > -1.0f)
		p.z = -1.0f;
	scale = fov / (fov - p.z);
	return (v2(origin.x + p.x * scale, origin.y + p.y * scale));
}
