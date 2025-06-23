/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 02:51:18 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/25 00:55:44 by giuliovalen      ###   ########.fr       */
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

void	remove_chars(t_md *md, char **txt, const char *to_remove)
{
	char	*new_str;
	int		new_len;
	int		i;
	int		j;

	new_len = 0;
	i = -1;
	while ((*txt)[++i])
		if (!char_in_str((*txt)[i], to_remove))
			new_len++;
	if (new_len == ft_strlen(*txt))
		return ;
	new_str = malloc(new_len + 1);
	if (!new_str)
		free_and_quit(md, "alloc in remove_chars: ", *txt);
	j = -1;
	i = -1;
	while ((*txt)[++i])
		if (!char_in_str((*txt)[i], to_remove))
			new_str[++j] = (*txt)[i];
	new_str[++j] = '\0';
	free(*txt);
	*txt = new_str;
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
