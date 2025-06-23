/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_math_scale.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 15:10:03 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/31 15:15:15 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

t_vec3f	scale_vec3f(t_vec3f v, float s)
{
	return ((t_vec3f){v.x * s, v.y * s, v.z * s});
}

t_vec3	scale_vec3(t_vec3 v, float s)
{
	return ((t_vec3){v.x * s, v.y * s, v.z * s});
}

t_vec2f	scale_vec2f(t_vec2f v, float s)
{
	return ((t_vec2f){v.x * s, v.y * s});
}

t_vec2	scale_vec2(t_vec2 v, float s)
{
	return ((t_vec2){v.x * s, v.y * s});
}
