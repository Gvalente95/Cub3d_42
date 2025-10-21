/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_inventory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:45:16 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 09:50:52 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	update_inv_input(t_md *md, t_inventory *inv, unsigned int c)
{
	const int	hor_input = (c == D_KEY) - (c == A_KEY);
	const int	ver_input = (c == S_KEY) - (c == W_KEY);
	const int	has_selected = c == ENTER_KEY || md->mouse.click == MOUSE_PRESS;
	const int	is_in_option = inv->sel_i != -1;

	if (has_selected)
		return (update_inv_selection(md, inv));
	if (hor_input)
	{
		if (is_in_option)
			return (close_option_pannel(inv), 1);
		wrap_int(&inv->page, 0, inv->num_pages - 1, hor_input);
		return (inv->update_img = 1, play_sound(md, AU_TUK), 1);
	}
	if (!ver_input)
		return (0);
	inv->update_img = 1;
	inv->update_opt = 1;
	if (inv->sel_i < 0)
		wrap_int(&inv->hov_indexes[inv->page], 0, \
			inv->num_items[inv->page], ver_input);
	else
		wrap_int(&inv->opt_i, 0, 2, ver_input);
	play_sound(md, AU_BOP);
	return (1);
}

void	handle_inv_mouse(t_md *md, t_inventory *inv, t_image *img)
{
	const t_mouse	ms = md->mouse;
	const int		txsc = md->prm.txt_sc;
	float			relative_y;
	int				hov;
	const float		line_h = inv->line_height;

	if (ms.pressed == MOUSE_PRESS)
	{
		inv->p.x = minmax(0, md->win_sz.x - img->size.x, inv->p.x + ms.delta.x);
		inv->p.y = minmax(0, md->win_sz.y - img->size.y, inv->p.y + ms.delta.y);
	}
	if (ms.real.y <= inv->p.y + 8 + txsc + line_h)
		return ;
	relative_y = ms.real.y - (inv->p.y + 8 + txsc + line_h);
	hov = minmax(0, PCKP_TYPE_LEN - 1, (int)(relative_y / line_h));
	if (inv->hov_indexes[inv->page] != hov && (inv->sel_i < 0 || ms.click))
	{
		inv->hov_indexes[inv->page] = hov;
		inv->update_img = 1;
	}
	if (ms.click != MOUSE_PRESS || inv->sel_i == hov || inv->items[hov] <= 0)
		return ;
	inv->sel_i = hov;
	inv->update_img = 1;
	play_sound(md, AU_MOUSE_CLICK);
}

void	handle_opt_mouse(t_md *md, t_inventory *inv)
{
	const t_mouse	ms = md->mouse;
	float			relative_y;
	int				hov;
	const float		line_h = inv->line_height;

	relative_y = ms.real.y - inv->opt_p.y;
	hov = minmax(0, 2, (int)(relative_y / line_h));
	if (inv->opt_i != hov)
	{
		inv->opt_i = hov;
		inv->update_opt = 1;
	}
	if (ms.click != MOUSE_PRESS)
		return ;
	if (!grab_item(md, inv, inv->opt_i))
		close_option_pannel(inv);
}

int	handle_mouse_interraction(t_md *md, t_inventory *inv)
{
	const t_vec2	mp = md->mouse.real;
	int				delta_i;

	if (!md->mouse.delta.y)
		return (0);
	if (v2_touch(mp, _v2(1), inv->p, inv->sz))
		return (handle_inv_mouse(md, inv, inv->img), 1);
	else if (v2_touch(mp, _v2(1), inv->opt_p, inv->opt_sz))
		return (handle_opt_mouse(md, inv), 1);
	if (md->timer.time % 8 != 0 || !md->mouse.delta.y)
		return (0);
	delta_i = ft_sign(md->mouse.delta.y);
	if (inv->sel_i != -1)
	{
		inv->opt_i = minmax(0, 2, inv->opt_i + delta_i);
		inv->update_opt = 1;
		return (1);
	}
	inv->hov_indexes[inv->page] = \
		minmax(0, PCKP_TYPE_LEN - 1, inv->hov_indexes[inv->page] + delta_i);
	inv->update_img = 1;
	return (1);
}

void	update_inventory(t_md *md, t_inventory *inv)
{
	const int	prv_hov = inv->hov_indexes[inv->page];

	if (md->timer.time % 2 == 0 && md->mouse.scroll_raw.y)
	{
		inv->hov_indexes[inv->page] = minmax(0, PCKP_TYPE_LEN - 1, \
			prv_hov - md->mouse.scroll_raw.y);
		inv->update_img = 1;
	}
	handle_mouse_interraction(md, inv);
	if (prv_hov != md->inv.hov_indexes[inv->page])
		play_sound(md, AU_BOP);
	if (inv->update_img)
		refresh_inv_bgr(md, inv);
	if (inv->update_opt)
		refresh_inv_opt_bgr(md, inv);
	if (inv->update_map)
		refresh_inv_map(md, inv, md->mmap.full_map);
	if (inv->sel_i == -1)
		return ;
	inv->opt_p.x = inv->p.x - inv->opt_sz.x;
	inv->opt_p.y = inv->p.y + 8 + inv->line_height * (inv->sel_i + 2);
}
