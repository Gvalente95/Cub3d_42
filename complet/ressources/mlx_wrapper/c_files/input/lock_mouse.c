/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lock_mouse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 12:11:44 by gvalente          #+#    #+#             */
/*   Updated: 2025/06/23 14:41:50 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"
#include "../../mlx_linux/mlx_int.h"
#include <X11/Xlib.h>
#include <X11/extensions/Xfixes.h>

Display	*mlx_get_display(void *mlx_ptr)
{
	return (((t_xvar *) mlx_ptr)->display);
}

Window	mlx_get_window(void *mlx_ptr, void *win_ptr)
{
	return (((t_win_list *) win_ptr)->window);
}

void	show_cursor(t_md *md)
{
	XUndefineCursor(mlx_get_display(md->mlx), mlx_get_window(md->mlx, md->win));
}

void	hide_cursor(t_md *md)
{
	Pixmap	bl;
	XColor	dum;
	char	data[1];
	Display	*dsp;
	Window	win;

	memset(&dum, 0, sizeof(XColor));
	dsp = mlx_get_display(md->mlx);
	win = mlx_get_window(md->mlx, md->win);
	data[0] = 0;
	bl = XCreateBitmapFromData(dsp, win, data, 1, 1);
	XDefineCursor(dsp, win, XCreatePixmapCursor(dsp, bl, bl, &dum, &dum, 0, 0));
	XFreePixmap(dsp, bl);
}

void	lock_mouse_center(t_md *md)
{
	Display			*display;
	Window			win;
	const t_vec2	mouse_p = div_v2(md->win_sz, 2);

	display = mlx_get_display(md->mlx);
	win = (Window)mlx_get_window(md->mlx, md->win);
	hide_cursor(md);
	XWarpPointer(display, None, win, 0, 0, 0, 0, mouse_p.x, mouse_p.y);
	XFlush(display);
}
