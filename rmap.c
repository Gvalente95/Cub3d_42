/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rmap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 00:01:16 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 09:51:04 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

int	main(int argc, char *argv[])
{
	int			difficulty;
	t_vec2		map_size;
	char		*map;

	difficulty = 5;
	map_size = v2(20, 20);
	if (argc > 1)
		map_size.x = ft_atoi(argv[1]);
	if (argc > 2)
		map_size.y = ft_atoi(argv[2]);
	if (argc > 3)
		difficulty = ft_atoi(argv[3]);
	map = get_new_map(difficulty, &map_size, NULL);
	free(map);
	return (1);
}
