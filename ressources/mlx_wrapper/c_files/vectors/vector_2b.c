/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_2b.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:13:52 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 09:51:04 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

t_vec2	v2_center(t_vec2 a, t_vec2 b)
{
	return (v2(a.x / 2 - b.x / 2, a.y / 2 - b.y / 2));
}

int	v2_bounds(t_vec2 a, t_vec2 bnd_pos, t_vec2 bnd_size)
{
	return (a.x >= bnd_pos.x && \
		a.x <= bnd_pos.x + bnd_size.x && \
		a.y >= bnd_pos.y && \
		a.y <= bnd_pos.y + bnd_size.y);
}

t_vec2	_v2(int scale)
{
	return ((t_vec2){scale, scale});
}

int	v2_touch(t_vec2 a, t_vec2 a_size, t_vec2 b, t_vec2 b_size)
{
	return (a.x < b.x + b_size.x && \
		a.x + a_size.x > b.x && \
		a.y < b.y + b_size.y && \
		a.y + a_size.y > b.y \
	);
}

t_vec2	div_v2(t_vec2 a, int div)
{
	return ((t_vec2){a.x / div, a.y / div});
}
