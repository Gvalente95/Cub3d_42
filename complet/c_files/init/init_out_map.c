/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_out_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:36:09 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/23 00:26:03 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	is_wall(char c)
{
	return (c == '2' || c == 'D');
}

static int	fill_out_map(char *map, int index, int width, int len)
{
	if (index < 0 || index >= len)
		return (0);
	if (map[index] == '\n')
		return (0);
	if (is_wall(map[index]) || map[index] == '*')
		return (1);
	if (map[index] == '1')
		return (0);
	map[index] = '*';
	if (!fill_out_map(map, index + 1, width, len))
		return (0);
	if (!fill_out_map(map, index - 1, width, len))
		return (0);
	if (!fill_out_map(map, index + width, width, len))
		return (0);
	if (!fill_out_map(map, index - width, width, len))
		return (0);
	return (1);
}

static void	replace_map_interiors(char *out_map)
{
	int	i;

	i = -1;
	while (out_map[++i])
	{
		if (out_map[i] == '*')
			out_map[i] = 'I';
		else if (!char_in_str(out_map[i], "21D\n"))
			out_map[i] = ' ';
	}
	printf("out_map\n%s\n", out_map);
}

char	*get_out_map(char *map, int width, int len)
{
	char	*out_map;
	char	*piece_map;
	int		i;

	out_map = ft_strdup(map);
	i = -1;
	while (map[++i])
	{
		if (map[i] == '\n' || map[i] == '1' || is_wall(map[i]) || map[i] == '*')
			continue ;
		piece_map = ft_strdup(out_map);
		if (!fill_out_map(piece_map, i, width, len))
		{
			free(piece_map);
			continue ;
		}
		free(out_map);
		out_map = piece_map;
	}
	replace_map_interiors(out_map);
	return (out_map);
}
