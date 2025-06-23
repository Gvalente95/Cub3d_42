/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_math_sub.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 15:10:12 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/31 15:14:52 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

t_vec3f	sub_vec3f(t_vec3f a, t_vec3f b)
{
	return ((t_vec3f){a.x - b.x, a.y - b.y, a.z - b.z});
}

t_vec3	sub_vec3(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){a.x - b.x, a.y - b.y, a.z - b.z});
}

t_vec2f	sub_vec2f(t_vec2f a, t_vec2f b)
{
	return ((t_vec2f){a.x - b.x, a.y - b.y});
}

t_vec2	sub_vec2(t_vec2 a, t_vec2 b)
{
	return ((t_vec2){a.x - b.x, a.y - b.y});
}
