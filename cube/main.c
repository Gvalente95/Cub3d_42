/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 22:23:11 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/28 18:27:43 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

int	main(int argc, char *argv[])
{
	t_md	md;
	int		res;
	t_vec2	scrn_size;

	if (argc < 2)
		return (printf("%sError\nNo arg%s\n", PRED, PRESET));
	if (argc >= 7)
		return (printf("%sError\nunvalid arg size%s\n", PRED, PRESET), 1);
	if (!ft_strnstr(argv[1], ".cub", ft_strlen(argv[1])))
		return (printf("%sError\nunvalid format%s\n", PRED, PRESET), 1);
	res = RESOLUTION;
	scrn_size = v2(SCRN_W, SCRN_H);
	if (argc > 3 && ft_isalnum(argv[2][0]))
		scrn_size.x = minmax(200, 3000, ft_atoi(argv[3]));
	if (argc > 4 && ft_isalnum(argv[3][0]))
		scrn_size.y = minmax(200, 3000, ft_atoi(argv[4]));
	if (argc > 5)
		res = minmax(2, 30, ft_atoi(argv[5]));
	init_wrapper(&md, scrn_size, "Cube3d", res);
	init_cube(&md, argv[1], argc > 2);
	mlx_do_key_autorepeatoff(md.mlx);
	mlx_loop_hook(md.mlx, (int (*)())update_and_render, &md);
	mlx_loop(md.mlx);
	return (0);
}
