/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 22:23:11 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/23 15:24:41 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

int	main(int argc, char *argv[])
{
	t_md	md;
	int		res;
	t_vec2	scrn_size;

	if (argc < 2)
		return (printf("Error\nNo arg\n"));
	if (argc >= 6)
		return (printf("Error\nunvalid arg size\n"), 1);
	if (!ft_strnstr(argv[1], ".cub", ft_strlen(argv[1])))
		return (printf("Error\nunvalid format\n"), 1);
	res = RESOLUTION;
	scrn_size = v2(SCRN_W, SCRN_H);
	init_wrapper(&md, scrn_size, "Cube3d", res);
	init_cube(&md, argv[1], argc > 2);
	mlx_do_key_autorepeatoff(md.mlx);
	mlx_loop_hook(md.mlx, (int (*)())update_and_render, &md);
	mlx_loop(md.mlx);
	return (0);
}
