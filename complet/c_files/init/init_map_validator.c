/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map_validator.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 02:32:22 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/11 19:24:58 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	find_unvalid_char(char *str, char *must_contain)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (!char_in_str(str[i], must_contain))
			return (str[i]);
	}
	return ('\0');
}

static int	flood_fill(t_map *map, char *str, int index, int depth)
{
	if (depth > 10)
		return (1);
	if (index < 0 || index >= map->len)
		return (1);
	if (char_in_str(str[index], "!12D*\n\0"))
		return (1);
	if (str[index] == ' ' || str[index] == '!' || str[index] == '\n')
		return (str[index] = '!', 0);
	str[index] = '*';
	if (!flood_fill(map, str, index - 1, depth + 1))
		return (0);
	if (!flood_fill(map, str, index + 1, depth + 1))
		return (0);
	if (!flood_fill(map, str, index - map->size.x, depth + 1))
		return (0);
	if (!flood_fill(map, str, index + map->size.x, depth + 1))
		return (0);
	return (1);
}

static void	print_unvalid_flood(char *flooded_map)
{
	int	i;

	i = -1;
	while (flooded_map[++i])
	{
		if (flooded_map[i] == '\n')
			printf("n\n");
		else if (flooded_map[i] == '!')
			printf("%s!%s", COLOR_TXT_RED, COLOR_TXT_RESET);
		else if (flooded_map[i] == '*')
			printf("%s*%s", COLOR_TXT_GREEN, COLOR_TXT_RESET);
		else
			printf("%c", flooded_map[i]);
	}
	printf("\n");
}

int	find_breach(t_md *md, t_map *map, int plr_index)
{
	int			space_index;
	char		*flooded_map;
	int			found_breach;

	if (map->len > 20000)
		free_and_quit(md, "Map too large for this version, sorry :|", NULL);
	flooded_map = ft_strdup(map->buffer);
	flood_fill(map, flooded_map, plr_index, 0);
	if (contains(flooded_map, '!'))
		return (print_unvalid_flood(flooded_map), free(flooded_map), 1);
	space_index = get_char_index(flooded_map, '0');
	while (space_index != -1)
	{
		flood_fill(map, flooded_map, space_index, 0);
		space_index = get_char_index(flooded_map, '0');
	}
	found_breach = contains(flooded_map, '!');
	if (found_breach)
		return (print_unvalid_flood(flooded_map), free(flooded_map), 1);
	free(flooded_map);
	return (0);
}

int	validate_map(t_md *md, char *map)
{
	char	unvalid_char;
	char	*valid_chars;

	valid_chars = md_strjoin(md, md->txd.ents_tp_map[0], " 10NSEW\n");
	unvalid_char = find_unvalid_char(map, valid_chars);
	free(valid_chars);
	if (unvalid_char != '\0')
		return (printf("Error\nUnvalid char: %c\n", unvalid_char), 0);
	return (1);
}
