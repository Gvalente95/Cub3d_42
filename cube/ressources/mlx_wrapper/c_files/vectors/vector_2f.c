/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_2f.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:50:12 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/17 14:48:12 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

t_vec2f	v3_to_v2f(t_vec3 vec)
{
	return (get_v2f(vec.x, vec.y));
}

int	cmp_vec2f(t_vec2f a, t_vec2f b, float margin)
{
	return (
		fabsf(a.x - b.x) < margin && \
		fabsf(a.y - b.y) < margin
	);
}

t_vec2f	get_v2f(float x, float y)
{
	t_vec2f	ret;

	ret.x = x;
	ret.y = y;
	return (ret);
}

int	print_vec2f(t_vec2f a, const char *label)
{
	if (label)
		printf("%s: ", label);
	printf("x%f y%f\n", a.x, a.y);
	return (1);
}
