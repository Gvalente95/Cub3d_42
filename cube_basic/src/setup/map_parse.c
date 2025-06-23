/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dyodlm <dyodlm@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:29:23 by dyodlm            #+#    #+#             */
/*   Updated: 2025/06/23 07:02:51 by dyodlm           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	format_object(
	t_data *data,
	t_point *original,
	int *object,
	char current
	)
{
	if (!data || !object)
		return ;
	if (ft_isspace(current))
		*object = -1;
	else if (current >= '0' && current <= '9')
		*object = current - '0';
	else if (current == 'N' || current == 'S'
		|| current == 'E' || current == 'W')
	{
		*object = PLAYER_POS;
		if (data)
			data->run.player.start = current;
	}
	else
		*object = -1;
	(void)original;
}

void	map_scale_object(
	int ***scaled_map,
	char **map,
	t_point *original,
	t_data *data
	)
{
	char	current;
	int		object;
	t_point	scale;
	t_point	iter;

	ft_bzero(&iter, sizeof(t_point));
	if (!scaled_map || !*scaled_map || !map || !original || !data)
		return ;
	if (map && original->x < data->run.map.max.x
		&& original->y < data->run.map.max.y)
		current = map[original->y][original->x];
	else
		current = -1;
	scale = (t_point){original->x * SCALE_MAP, original->y * SCALE_MAP};
	format_object(data, original, &object, current);
	while (iter.y < SCALE_MAP)
	{
		iter.x = 0;
		while (iter.x < SCALE_MAP)
			(*scaled_map)[scale.y + iter.y][scale.x + iter.x++] = object;
		iter.y++;
	}
	original->x++;
}

int	**scale_map(char **map, t_data *data)
{
	t_point	max;
	t_point	iter;
	int		**scaled_map;

	if (!map || !data)
		return (NULL);
	ft_bzero(&iter, sizeof(t_point));
	ft_bzero(&max, sizeof(t_point));
	map_get_format(map, data);
	max = data->run.map.max;
	scaled_map = allocate_scaled_map(max);
	if (!scaled_map)
		return (NULL);
	while (iter.y < max.y)
	{
		iter.x = 0;
		while (iter.x < max.x)
			map_scale_object(&scaled_map, map, &iter, data);
		iter.y++;
	}
	return (scaled_map);
}

static char	**alloc_map(char *doc, int line_count)
{
	int		i;
	int		k;
	int		j;
	int		y;
	char	**map;

	map = malloc(sizeof(char *) * (line_count + 1));
	i = 1;
	k = 0;
	while (doc[i] && k < line_count)
	{
		j = 0;
		while (doc[i + j] && doc[i + j] != '\n')
			j++;
		if (no_char_in_line(doc, i, i + j))
			break ;
		map[k] = malloc(j + 1);
		y = 0;
		while (j-- > 0 && doc[i])
			map[k][y++] = doc[i++];
		map[k++][y] = '\0';
		i++;
	}
	map[k] = NULL;
	return (map);
}

char	**parse_map(char *doc)
{
	int		line_count;
	char	**map;

	if (!doc)
		return (NULL);
	line_count = count_char(doc, '\n') - 1;
	map = alloc_map(doc, line_count);
	if (!map)
		return (NULL);
	print_map(map);
	free(doc);
	return (map);
}
