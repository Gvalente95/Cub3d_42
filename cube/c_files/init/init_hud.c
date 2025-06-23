/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_hud.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:44:55 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/11 19:42:58 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	init_fx(t_md *md, t_fx_data *fx)
{
	fx->vignette = init_img(md, md->win_sz, NULL, -1);
	flush_img(fx->vignette, 0xFFFFFFFF, .2, 0);
	apply_vignette(fx->vignette, 1, _WHITE);
	fx->fog = .5;
	fx->noise = 0.5;
}

t_image	*init_center_cross(t_md *md)
{
	const t_vec2		sz = _v2(md->win_sz.x * .015f);
	const int			thick = sz.x / 8;
	const int			clr = _WHITE;
	t_vec2				line;
	void				*img;

	img = init_img(md, sz, NULL, _NULL);
	flush_img(img, v4_to_color(0, 0, 0, 0), -1, 0);
	line.x = sz.x / 2 - thick / 2;
	line.y = 0;
	draw_pixels(img, line, v2(thick, sz.y), clr);
	line.x = 0;
	line.y = sz.y / 2 - thick / 2;
	draw_pixels(img, line, v2(sz.x, thick), clr);
	return (img);
}

void	init_hud(t_md *md, t_hud *hud)
{
	md->hud.hp = 100;
	md->hud.keys = 1;
	hud->bgr_color = md->hud.ceiling_color;
	md->screen = init_img(md, md->win_sz, NULL, hud->bgr_color);
	hud->overlay = init_img(md, md->win_sz, NULL, -1);
	hud->lock_x_icon = init_img(md, _v2(20), "hud/icons/lock_x.xpm", -1);
	hud->lock_y_icon = init_img(md, _v2(20), "hud/icons/lock_y.xpm", -1);
	hud->amm_icon = init_img(md, _v2(20), "hud/icons/amm.xpm", -1);
	hud->amm2_icon = init_img(md, _v2(20), "hud/icons/amm2.xpm", -1);
	hud->key_icon = init_img(md, _v2(20), "hud/icons/key.xpm", -1);
	hud->key2_icon = init_img(md, _v2(20), "hud/icons/key2.xpm", -1);
	hud->center = init_center_cross(md);
	hud->rgun = init_img(md, v2(400, 400), "hud/rgun.xpm", -1);
	init_background(md, hud, md->win_sz);
	init_fx(md, &md->fx);
}
