/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 00:07:17 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 09:51:56 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

t_vec4	v4(int r, int g, int b, int a)
{
	t_vec4	res;

	res.r = r;
	res.g = g;
	res.b = b;
	res.a = a;
	return (res);
}

t_vec4	_v4(int scale)
{
	return ((t_vec4){scale, scale, scale, scale});
}

t_vec3	v4_to_v3(t_vec4 a)
{
	t_vec3	ret;

	ret.x = a.r;
	ret.y = a.g;
	ret.z = a.b;
	return (ret);
}

int	cmp_vec4(t_vec4 a, t_vec4 b)
{
	return (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a);
}

int	print_vec4(t_vec4 a, const char *label)
{
	if (label)
		printf("%s: ", label);
	printf("r%d g%d %db %da", a.r, a.g, a.b, a.a);
	return (1);
}
