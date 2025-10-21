/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 02:51:18 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/30 01:49:15 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	trim_excess_spaces(char **line)
{
	int		len;
	int		new_len;
	char	*trimmed_line;

	if (!*line)
		return ;
	len = ft_strlen(*line);
	if (len <= 0)
		return ;
	new_len = len;
	while (new_len > 0 && (*line)[new_len - 1] == ' ')
		new_len--;
	if (new_len == len)
		return ;
	trimmed_line = ft_strndup(*line, new_len);
	free(*line);
	*line = ft_strjoin(trimmed_line, "\n");
	free(trimmed_line);
}

int	trim_excess_newlines(char **map, int len)
{
	int		i;

	i = len - 1;
	while (i > 0 && char_in_str((*map)[i], "\n 0"))
		(*map)[i--] = '\0';
	if (i < len - 1)
		return (1);
	return (0);
}

int	get_to_find_index(char *str, char *to_find)
{
	int	i;

	i = -1;
	while (str[++i])
		if (char_in_str(str[i], to_find))
			return (i);
	return (-1);
}

int	contains_valid_character(char *line, const char *valid_characters)
{
	int	i;

	i = -1;
	while (line[++i])
		if (line[i] != ' ' && char_in_str(line[i], valid_characters))
			return (1);
	return (0);
}

int	are_bounds_valid(char *map, t_vec2 size)
{
	t_vec2	p;

	p = _v2(-1);
	while (++p.y < size.y)
	{
		p.x = -1;
		while (++p.x < size.x)
		{
			if (p.x != 0 && p.x != size.x - 2 && p.y != 0 && p.y != size.y -1)
				continue ;
			if (map[p.x + ((size.x) * p.y)] != '0')
				continue ;
			p = v2(-1, p.x + ((size.x) * p.y));
			printf("\n");
			while (map[++p.x])
			{
				if (p.x == p.y)
					printf("%s%c%s", PRED, map[p.x], PRESET);
				else
					printf("%c", map[p.x]);
			}
			return (0);
		}
	}
	return (1);
}
