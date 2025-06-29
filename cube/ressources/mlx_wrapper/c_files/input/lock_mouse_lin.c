/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lock_mouse_lin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 12:11:44 by gvalente          #+#    #+#             */
/*   Updated: 2025/06/28 14:02:12 by giuliovalen      ###   ########.fr       */
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
	return ;
	XUndefineCursor(mlx_get_display(md->mlx), mlx_get_window(md->mlx, md->win));
}

void	hide_cursor(t_md *md)
{
	Pixmap	bl;
	XColor	dum;
	char	data[1];
	Display	*dsp;
	Window	win;

	return ;
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

	return ;
	display = mlx_get_display(md->mlx);
	win = (Window)mlx_get_window(md->mlx, md->win);
	hide_cursor(md);
	XWarpPointer(display, None, win, 0, 0, 0, 0, mouse_p.x, mouse_p.y);
	XFlush(display);
}
