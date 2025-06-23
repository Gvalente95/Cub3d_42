/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_2f_b.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:51:44 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/17 14:48:19 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

t_vec2f	v2f_center(t_vec2f a, t_vec2f b)
{
	return (get_v2f(a.x / 2 - b.x / 2, a.y / 2 - b.y / 2));
}

int	v2f_bounds(t_vec2f a, t_vec2f bnd_pos, t_vec2f bnd_size)
{
	return (a.x >= bnd_pos.x && \
		a.x <= bnd_pos.x + bnd_size.x && \
		a.y >= bnd_pos.y && \
		a.y <= bnd_pos.y + bnd_size.y);
}

t_vec2f	v2f(int scale)
{
	return ((t_vec2f){scale, scale});
}

int	v2f_touch(t_vec2f a, t_vec2f a_size, t_vec2f b, t_vec2f b_size)
{
	return (a.x < b.x + b_size.x && \
		a.x + a_size.x > b.x && \
		a.y < b.y + b_size.y && \
		a.y + a_size.y > b.y \
	);
}

t_vec2f	div_v2f(t_vec2f a, float div)
{
	return ((t_vec2f){a.x / div, a.y / div});
}
