/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_gen.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 23:10:51 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 09:51:32 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	set_rect_cell(char *full, t_vec3 draw_p, t_vec3 full_sz, int is_border)
{
	int			i;
	const int	is_house = full_sz.x * full_sz.y < ft_strlen(full) / 5;
	int			brd_c;

	brd_c = '1';
	if (is_house == 0)
		brd_c = '2';
	i = (full_sz.x * draw_p.y + draw_p.x);
	if (i >= full_sz.z - 1 || i < 0 || full[i] == '\n')
		return (0);
	if (draw_p.x <= 1 || draw_p.x == full_sz.x || \
		draw_p.y == 0 || draw_p.y == full_sz.y)
		return (full[i] = '1', 0);
	if (is_border && ((is_house || (full[i] == ' ' || r_range(0, 25) > 3)) || \
		r_range(0, 100) == 0))
		full[i] = brd_c;
	else
		full[i] = '0';
	if (is_border && \
		r_range(0, 50) == 0 && \
		full[i - 1] == brd_c && \
		full[i - 2] == brd_c && full[i - full_sz.x - 1] == '0' && \
		full[i + full_sz.x - 1] == '0')
		full[i - 1] = 'D';
	return (1);
}

int	set_rect_home(char *full, t_vec3 draw_p, t_vec3 full_sz, t_vec3 rect_pos)
{
	int		i;
	int		is_border;
	t_dir	dir;
	int		neigh_index;

	dir = draw_p.z;
	is_border = rect_pos.z;
	i = (full_sz.x * draw_p.y + draw_p.x);
	if (i >= full_sz.z - 1 || i < 0 || full[i] == '\n')
		return (0);
	if (draw_p.x <= 1 || draw_p.x == full_sz.x || \
		draw_p.y == 0 || draw_p.y == full_sz.y)
		return (full[i] = '2', 0);
	if (is_border && rect_pos.x == 3 && rect_pos.y == 0 + ((dir == down) * 6))
	{
		neigh_index = i - (full_sz.x * (1 - ((dir == down) * 2)));
		if (full[neigh_index] == '2')
			full[neigh_index] = '0';
		return (full[i] = 'D', 1);
	}
	else if (is_border)
		return (full[i] = '2', 1);
	if (dir == up && rect_pos.x == 3 && rect_pos.y == 3)
		return (full[i] = 'N', full[i - full_sz.x] = 'G', 1);
	return (full[i] = 'P', 1);
}

void	join_rect(char *full, t_vec2 rect_size, t_vec3 rect_pos, t_vec3 full_sz)
{
	t_vec3	draw_p;
	t_vec2	end_draw;
	int		bdr;
	t_vec3	data;

	draw_p = v3(rect_pos.x - 1, rect_pos.y - 1, rect_pos.z);
	end_draw = v2(rect_pos.x + rect_size.x, rect_pos.y + rect_size.y);
	while (++draw_p.y < end_draw.y)
	{
		if (draw_p.y > full_sz.y)
			break ;
		draw_p.x = rect_pos.x - 1;
		while (++draw_p.x < end_draw.x)
		{
			if (draw_p.x > full_sz.x)
				break ;
			bdr = (draw_p.x == rect_pos.x || draw_p.x == end_draw.x - 1 || \
					draw_p.y == rect_pos.y || draw_p.y == end_draw.y - 1);
			data = v3(draw_p.x - rect_pos.x, draw_p.y - rect_pos.y, bdr);
			if (rect_pos.z > -1)
				set_rect_home(full, draw_p, full_sz, data);
			else
				set_rect_cell(full, draw_p, full_sz, bdr);
		}
	}
}

char	*get_cmps_map(t_vec2 size, int rects_amount, int i)
{
	char	*map;
	t_vec2	rect_size;
	t_vec3	rect_pos;
	t_vec3	map_sz;

	map_sz = v3(size.x, size.y, (size.x * size.y) + 1);
	map = malloc(map_sz.z);
	while (++i < map_sz.z - 1)
	{
		map[i] = ' ';
		if (i % size.x == 0)
			map[i] = '\n';
	}
	map[i] = '\0';
	rect_pos.z = -1;
	while (rects_amount--)
	{
		rect_size = v2(r_range(5, size.x * .75), r_range(5, size.y * .75));
		rect_pos.x = r_range(0, size.x - rect_size.x - 1);
		rect_pos.y = r_range(3, size.y - rect_size.y - 3);
		join_rect(map, rect_size, rect_pos, map_sz);
	}
	join_rect(map, _v2(7), v3(size.x / 2 - 4, size.y - 7, up), map_sz);
	join_rect(map, _v2(7), v3(size.x / 2 - 4, 0, down), map_sz);
	return (map);
}

char	*get_new_map(int difficulty, t_vec2 *size, char *data_info)
{
	const char		map_name[] = "Randomap.cub";
	char			*map;
	char			*map_data;
	int				fd;
	unsigned int	seed;

	seed = get_r_seed();
	size->x = r_range_seed(&seed, size->x * .8, size->x * 1.2);
	size->y = r_range_seed(&seed, size->y * .8, size->y * 1.2);
	map = get_cmps_map(*size, 10, -1);
	set_characters(map, difficulty);
	close_map(map, *size, ft_strlen(map));
	if (!data_info)
		map_data = set_map_with_base(map);
	else
		map_data = ft_megajoin(map, "\n", data_info, NULL);
	free(map);
	fd = open(map_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (printf("unable to create map"), free(map_data), NULL);
	write(fd, map_data, ft_strlen(map_data));
	printf("map generated: \n%s\n", map_data);
	close(fd);
	return (map_data);
}
