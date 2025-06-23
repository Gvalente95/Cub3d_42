/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_wrapper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 20:39:27 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/11 18:44:27 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

static void	init_cursor(t_md *md)
{
	t_mouse			*msd;
	const t_vec2	cursor_sz = _v2(30);

	msd = &md->mouse;
	msd->cursor = init_img(md, cursor_sz, "utils/cursor/default.xpm", -1);
	msd->curs_dtc = init_img(md, cursor_sz, "utils/cursor/hand_open.xpm", -1);
	msd->curs_grb = init_img(md, cursor_sz, "utils/cursor/hand_closed.xpm", -1);
	msd->pos = v3f(0);
	msd->world = _v2(0);
	msd->real = _v2(0);
	md->cam.input_offst = _v2(0);
	msd->prev = _v2(0);
	msd->grid_pos = _v2(0);
	msd->scroll_delta = v2f(0);
	msd->scroll_raw = _v2(0);
	msd->delta = v2f(0);
	msd->focus = 0;
	msd->pressed = 0;
	msd->click = 0;
	mlx_mouse_hook(md->win, mouse_event_handler, md);
	mlx_hook(md->win, 5, ButtonReleaseMask, mouse_release_handler, md);
	mlx_hook(md->win, 6, PointerMotionMask, mouse_motion_handler, md);
}

static int	init_screen(t_md *md, t_vec2 win_sz, int resolution, char *win_name)
{
	void	*screen_image;

	md->win = mlx_new_window(md->mlx, win_sz.x, win_sz.y, win_name);
	md->win_sz = v2(win_sz.x, win_sz.y);
	md->t_len = win_sz.x / resolution;
	md->prm.resolution = resolution;
	return (1);
}

static int	init_md(t_md *md)
{
	memset(md, 0, sizeof(t_md));
	md->mlx = mlx_init();
	md->win = NULL;
	md->txd.wall_img = NULL;
	md->mouse.locked = 1;
	md->key_click = -1;
	md->t_len = 60;
	md->prm.txt_sc = max(20, md->win_sz.x / 150);
	md->mouse.hide = 1;
	return (1);
}

static void	init_os_params(t_md *md)
{
	md->mlx_put = mlx_put_image_to_window;
	md->mlx_make = mlx_xpm_file_to_image;
	md->is_linux = LIN;
	ft_strlcpy(md->base_map_path, "Randomap.cub", 20);
	printf("wrapper LIN = %d\n", LIN);
}

void	init_wrapper(t_md *md, t_vec2 win_sz, char *win_name, int resolution)
{
	init_md(md);
	init_os_params(md);
	init_fonts(md);
	start_timer(&md->timer.game_start);
	init_screen(md, win_sz, resolution, win_name);
	md->prm.txt_sc = 20;
	init_cursor(md);
	init_timer(md, &md->timer);
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
	md->r_seed = get_r_seed();
}
