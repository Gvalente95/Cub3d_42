/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:54:50 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 09:51:32 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

t_vec3	get_grid_pos(t_md *md, t_vec3 pos)
{
	return ((t_vec3){(int)(pos.x / md->t_len) * md->t_len, \
		(int)(pos.y / md->t_len) * md->t_len, 0});
}

t_vec3	v2_to_v3(t_vec2 vec)
{
	return (v3(vec.x, vec.y, 0));
}

t_vec3	v3(int x, int y, int z)
{
	t_vec3	ret;

	ret.x = x;
	ret.y = y;
	ret.z = z;
	return (ret);
}

int	cmp_vec3(t_vec3 a, t_vec3 b)
{
	return (a.x == b.x && a.y == b.y && a.z == b.z);
}

int	print_vec3(t_vec3 a, const char *label)
{
	if (label)
		printf("%s: ", label);
	printf("x%d y%d %dz\n", a.x, a.y, a.z);
	return (1);
}
