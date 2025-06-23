/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_hud.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:50:32 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/23 16:44:45 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	draw_locks_icons(t_md *md, t_hud *hud, t_vec2 winsz)
{
	t_vec2	lock_pos;

	lock_pos = v2(winsz.x - hud->lock_x_icon->size.x, \
		winsz.y - hud->lock_y_icon->size.y);
	if (md->mouse.lock_rot.x)
		draw_img(hud->lock_x_icon, md->screen, lock_pos, -1);
	lock_pos.y -= hud->lock_x_icon->size.y;
	if (md->mouse.lock_rot.y)
		draw_img(hud->lock_y_icon, md->screen, lock_pos, -1);
}

void	draw_info(t_md *md, t_vec2 *screen_pos, t_image *icon, int value)
{
	t_txtd	txt_data;

	txt_data = (t_txtd){\
		screen_pos->x + icon->size.x * 2, \
		screen_pos->y, \
		-1, \
		md->prm.txt_sc, NULL};
	draw_img(icon, md->screen, *screen_pos, -1);
	rnd_fast_txt(md, txt_data, "%d", value);
	screen_pos->x -= icon->size.x * 3;
}

void	draw_game_info(t_md *md, t_hud *hud, t_vec2 winsz)
{
	t_vec2	start_pos;
	int		txt_space;

	if (md->mouse.lock_rot.x || md->mouse.lock_rot.y)
		draw_locks_icons(md, hud, winsz);
	txt_space = md->prm.txt_sc * 1.5;
	start_pos = v2(winsz.x - txt_space * 10, winsz.y - txt_space);
	draw_info(md, &start_pos, md->hud.key_icon, md->hud.keys);
	draw_info(md, &start_pos, md->hud.key_icon, md->hud.hp);
}

void	draw_hud_weapon(t_md *md, t_hud *hud, t_vec2 winsz)
{
	t_vec2			center_gun;
	t_image			*gun_image;
	t_vec2			gun_sz;
	t_vec2			pos;

	gun_image = hud->rgun;
	gun_sz = gun_image->size;
	pos.x = winsz.x - gun_sz.x * .8;
	pos.y = winsz.y - gun_sz.y / 2 - md->cam.pos.z * 3 + md->plr.pos.z * 2.8;
	draw_img(gun_image, md->screen, pos, -1);
	return ;
	center_gun = v2(winsz.x / 2 - gun_image->size.x / 2 + 30, \
		winsz.y - gun_image->size.y);
	draw_img(gun_image, md->screen, center_gun, -1);
}

void	render_hud_elements(t_md *md, t_hud *hud)
{
	t_image	*img;
	t_vec2	cross_pos;

	if (!md->inv.active && md->inv.held_i == -1)
	{
		img = hud->center;
		if (md->cam.prv_pointed_ent || md->cam.pointed_door)
			img = md->mouse.cursor;
		cross_pos = sub_vec2(div_v2(md->win_sz, 2), div_v2(img->size, 2));
		draw_img(img, md->screen, cross_pos, -1);
	}
}
