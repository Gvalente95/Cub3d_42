/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:02:48 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 09:51:47 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

//	d = (start.x, start.y, end.x, end.y)
void	color_img(void *frame, t_vec2 size, int col, t_vec4 d)
{
	t_image	q;
	int		pixel_index;

	q.src = \
	(int *)mlx_get_data_addr(frame, &q.bpp, &q.size_line, &q.endian);
	q.pos.y = -1;
	while (++q.pos.y < size.y)
	{
		q.pos.x = -1;
		while (++q.pos.x < size.x)
		{
			if (q.pos.x < d.r || q.pos.x > d.b || \
				q.pos.y < d.g || q.pos.y > d.a)
				continue ;
			pixel_index = (q.pos.y * q.size_line / 4) + q.pos.x;
			q.src[pixel_index] = col;
		}
	}
}

int	str_to_color(const char *line)
{
	t_vec4		rgba;
	const char	*values;
	char		**splits;

	rgba = _v4(255);
	if (!line || ft_strlen(line) <= 3)
		return (str_to_color("255,255,255"));
	values = line;
	while (values && !ft_isdigit(*values))
		values++;
	if (!*values)
		return (-1);
	if (!contains(line, ','))
		splits = ft_split(values, ' ');
	else
		splits = ft_split(values, ',');
	if (!splits)
		return (str_to_color("255,255,255"));
	rgba.r = minmax(0, 255, ft_atoi(splits[0]));
	if (splits[1])
		rgba.g = minmax(0, 255, ft_atoi(splits[1]));
	if (splits[1] && splits[2])
		rgba.b = minmax(0, 255, ft_atoi(splits[2]));
	free_void_array((void **)splits);
	return ((rgba.r << 16) | (rgba.g << 8) | rgba.b);
}

int	ent_sort_cmp(void *a, void *b)
{
	const t_ent	*ea = *(t_ent **)a;
	const t_ent	*eb = *(t_ent **)b;

	if (ea->hit_dist < eb->hit_dist)
		return (1);
	if (ea->hit_dist > eb->hit_dist)
		return (-1);
	return (0);
}
