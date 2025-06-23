/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector4float.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:04:38 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/29 12:39:35 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

t_vec4f	get_v4f(float r, float g, float b, float a)
{
	t_vec4f	res;

	res.r = r;
	res.g = g;
	res.b = b;
	res.a = a;
	return (res);
}

t_vec3f	v4f_to_v3f(t_vec4f a)
{
	t_vec3f	ret;

	ret.x = (int)a.r;
	ret.y = (int)a.g;
	ret.z = (int)a.b;
	return (ret);
}

int	cmp_vec4f(t_vec4f a, t_vec4f b)
{
	return (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a);
}

int	print_vec4f(t_vec4f a, const char *label)
{
	if (label)
		printf("%s: ", label);
	printf("r%f g%f %fb %fa", a.r, a.g, a.b, a.a);
	return (1);
}

t_vec4f	v4f(float scale)
{
	return (get_v4f(scale, scale, scale, scale));
}
