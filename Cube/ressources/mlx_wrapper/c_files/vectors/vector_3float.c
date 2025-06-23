/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_3float.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:54:50 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/29 12:41:34 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

t_vec3	get_grid_posf(t_md *md, t_vec3f pos)
{
	return ((t_vec3){(int)(pos.x / md->t_len) * md->t_len, \
		(int)(pos.y / md->t_len) * md->t_len, 0});
}

t_vec3f	get_v3f(float x, float y, float z)
{
	t_vec3f	ret;

	ret.x = x;
	ret.y = y;
	ret.z = z;
	return (ret);
}

int	cmp_vec3f(t_vec3f a, t_vec3f b, float margin)
{
	return (
		fabsf(a.x - b.x) < margin && \
		fabsf(a.y - b.y) < margin && \
		fabsf(a.z - b.z) < margin
	);
}

int	print_vec3f(t_vec3f a, const char *label)
{
	if (label)
		printf("%s: ", label);
	printf("x%.3f y%.3f %.3fz\n", a.x, a.y, a.z);
	return (1);
}

t_vec3f	v3f(float scale)
{
	return ((t_vec3f){scale, scale, scale});
}
