/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_gen_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 21:15:25 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 09:51:32 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

char	*get_rectangle(t_vec2 size)
{
	char	*rect;
	t_vec2	pos;
	int		i;

	rect = malloc((size.x * size.y) + 1);
	pos = v2(-1, -1);
	i = -1;
	while (++pos.y < size.y)
	{
		pos.x = -1;
		while (++pos.x < size.x)
		{
			if (pos.x == 0 || pos.x == size.x || pos.y == 0 || pos.y == size.y)
				rect[i++] = '0';
			else
				rect[i++] = '1';
		}
		rect[i++] = '\n';
	}
	rect[i] = '\0';
	return (rect);
}

int	get_char_amount(char *buffer, char c)
{
	int	i;
	int	amount;

	amount = 0;
	i = -1;
	while (buffer[++i])
		if (buffer[i] == c)
			amount++;
	return (amount);
}

void	close_map(char *map, t_vec2 size, int len)
{
	int	i;

	i = -1;
	while (map[++i])
	{
		if (map[i] != '0' && map[i] != 'D')
			continue ;
		if (i > 0 && map[i - 1] == ' ')
			map[i] = '1';
		else if (i < len && map[i + 1] == ' ')
			map[i] = '1';
		else if (i - size.x > 0 && map[i - size.x] == ' ')
			map[i] = '1';
		else if (i + size.x < len && map[i + size.x] == ' ')
			map[i] = '1';
	}
}

int	replace_window(t_md *md, int new_w, int new_h)
{
	mlx_destroy_window(md->mlx, md->win);
	md->win_sz = v2(new_w, new_h);
	md->win = mlx_new_window(md->mlx, new_w, new_h, "Cube3D");
	if (md->is_linux)
	{
		mlx_hook(md->win, KeyPress, KeyPressMask, handle_key_press, md);
		mlx_hook(md->win, KeyRelease, KeyReleaseMask, handle_key_release, md);
		mlx_hook(md->win, DestroyNotify, StructureNotifyMask, close_window, md);
	}
	else
	{
		mlx_hook(md->win, 2, 0, handle_key_press, md);
		mlx_hook(md->win, 3, 0, handle_key_release, md);
		mlx_hook(md->win, 17, 0, close_window, md);
	}
	mlx_mouse_hook(md->win, mouse_event_handler, md);
	mlx_hook(md->win, 5, ButtonReleaseMask, mouse_release_handler, md);
	mlx_hook(md->win, 6, PointerMotionMask, mouse_motion_handler, md);
	set_menu_pos(md, &md->menu, v3(-200, -200, 1), v3(300, -200, 5));
	render(md);
	md->menu.selected_slider = NULL;
	md->mouse.click = MOUSE_NOPRESS;
	md->mouse.pressed = MOUSE_NOPRESS;
	return (md->menu.refresh_bg = 1, md->menu.refresh_ui = 1, -1);
}
