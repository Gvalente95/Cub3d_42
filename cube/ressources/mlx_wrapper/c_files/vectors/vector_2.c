/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 01:57:35 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/07 19:06:37 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

t_vec2	v3_to_v2(t_vec3 vec)
{
	return (v2(vec.x, vec.y));
}

int	same_vec2(t_vec2 a, t_vec2 b)
{
	return (a.x == b.x && a.y == b.y);
}

t_vec2	v2(int x, int y)
{
	t_vec2	ret;

	ret.x = x;
	ret.y = y;
	return (ret);
}

int	print_vec2(t_vec2 a, const char *label)
{
	if (label)
		printf("%s%-10s%s ", PBLUE, label, PRESET);
	printf("x%d y%d\n", a.x, a.y);
	return (1);
}
