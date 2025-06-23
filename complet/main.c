/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 22:23:11 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/16 18:26:36 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

int	main(int argc, char *argv[])
{
	t_md	md;
	int		res;
	t_vec2	scrn_size;

	if (argc >= 6)
		return (printf("unvalid arg size\n"), 1);
	res = RESOLUTION;
	scrn_size = v2(SCRN_W, SCRN_H);
	if (argc >= 3)
		res = ft_atoi(argv[2]);
	if (argc >= 4)
		scrn_size.x = ft_atoi(argv[3]);
	if (argc >= 5)
		scrn_size.y = ft_atoi(argv[4]);
	init_wrapper(&md, scrn_size, "Cube3d", res);
	if (argc <= 1)
		init_cube(&md, md.base_map_path, argc > 2);
	else
		init_cube(&md, argv[1], argc > 2);
	mlx_do_key_autorepeatoff(md.mlx);
	mlx_loop_hook(md.mlx, (int (*)())update_and_render, &md);
	mlx_loop(md.mlx);
	return (0);
}
