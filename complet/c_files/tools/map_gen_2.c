/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_gen_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 05:17:00 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/23 18:03:59 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	set_character(char c, int amount, char *buffer, int len)
{
	int	*free_indexes;
	int	free_len;
	int	random_index;
	int	set_amount;
	int	i;

	free_len = 0;
	free_indexes = malloc(sizeof(int) * len);
	i = -1;
	while (buffer[++i])
		if (buffer[i] == '0')
			free_indexes[free_len++] = i;
	if (free_len < 5)
		return (0);
	if (amount > free_len)
		amount = free_len;
	set_amount = amount;
	i = -1;
	while (amount--)
	{
		random_index = r_range(0, free_len - 1);
		if (buffer[free_indexes[random_index]] == '0')
			buffer[free_indexes[random_index]] = c;
	}
	return (free(free_indexes), free_len - set_amount);
}

void	set_doors(char *map, int doors_amount)
{
	int	i;

	i = 0;
	while (map[++i] && doors_amount)
	{
		if (map[i] != '0')
			continue ;
		if (map[i - 1] == '1' && map[i + 1] == '1' && r_range(0, 50))
		{
			map[i] = 'D';
			doors_amount--;
			continue ;
		}
	}
}

void	set_characters(char *map, int difficulty)
{
	int	mobs_amount;
	int	item_amount;
	int	door_amount;
	int	space;
	int	len;

	(void)difficulty;
	len = ft_strlen(map);
	if (!contains(map, 'N'))
		set_character('N', 1, map, len);
	space = get_char_amount(map, '0');
	mobs_amount = minmax(1, 30, space / 10);
	space = get_char_amount(map, '0');
	item_amount = minmax(1, 30, space / 10);
	set_character('P', item_amount, map, len);
	set_character('K', mobs_amount / 3, map, len);
	set_character('M', mobs_amount / 3, map, len);
	door_amount = len / 20;
	set_doors(map, door_amount);
}

char	*set_map_with_base(char *map)
{
	const char	data_info[6][50] = {
		"NO ressources/xpm/utils/wall.xpm\n", \
		"SO ressources/xpm/utils/wall.xpm\n", \
		"WE ressources/xpm/utils/wall.xpm\n", \
		"EA ressources/xpm/utils/wall.xpm\n", \
		"F 96,64,32\n", "C 120,120,255\n"
	};
	int			i;
	char		*full_data;
	char		*tmp;

	full_data = ft_strdup("");
	i = -1;
	while (++i < 6)
	{
		tmp = full_data;
		full_data = ft_strjoin(tmp, data_info[i]);
		free(tmp);
		if (!full_data)
			return (printf("Error\nAlloc of texture\n"), NULL);
	}
	tmp = full_data;
	full_data = ft_strjoin(tmp, map);
	return (free(tmp), full_data);
}
