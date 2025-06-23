/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 23:46:39 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/01 15:33:47 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	show_fps(t_md *md, t_vec2 pos)
{
	const int	colors[5] = {RGB_RED, RGB_ORANGE, RGB_YELLOW, \
		RGB_GREEN, RGB_BLUE};
	const int	color_index = minmaxf(0, 4, md->timer.prv_fps / 10);
	const int	color = md->rgb[colors[color_index]];
	t_txtd		txt_data;
	float		fps_gain;

	txt_data = (t_txtd){pos.x, pos.y, color, md->prm.txt_sc, NULL};
	rnd_fast_txt(md, txt_data, "fps %d", md->timer.prv_fps);
	txt_data.y -= md->prm.txt_sc * 1.5;
	fps_gain = md->timer.avrg_fps_prev - md->timer.avrg_fps;
	if (isinf(fps_gain) || isnan(fps_gain))
		fps_gain = 0;
	txt_data.color = md->rgb[colors[(fps_gain < 0) * 3]];
	if (fps_gain > 0)
		rnd_fast_txt(md, txt_data, "+%.1f", fps_gain);
	else
		rnd_fast_txt(md, txt_data, "%.1f", fps_gain);
	txt_data.color = -1;
	txt_data.y -= md->prm.txt_sc * 1.5;
	if (md->prm.use_thrd)
		rnd_fast_txt(md, txt_data, "T-on");
	if (md->prm.debug_mode)
		show_debug_time(md, txt_data);
}

void	render_2d_ent(t_md *md, t_ent *e, t_vec2 centr)
{
	t_vec2					ent_p;
	const t_texture_data	*td = &md->txd;
	t_image					*img;

	ent_p.x = centr.x + (e->pos.x / md->t_len) * td->size_2d;
	ent_p.y = centr.y + (e->pos.y / md->t_len) * td->size_2d;
	img = td->wall_img2d[0];
	if (e->type == nt_mob)
		img = td->mobs_txtrs_mini[e->mob_type][e->action][e->frame_index];
	else if (e->type == nt_item)
		img = td->item_txtr_mini[e->pckp_type][0];
	else if (e->type == nt_door)
		img = td->door_txtr_mini;
	else if (e->type == nt_pokemon)
		img = td->pkmns_mini[e->mob_type];
	else if (e->type == nt_ext_wall)
		img = td->ext_wall_mini;
	else if (e->type == nt_grass)
		img = td->grass_mini;
	else if (e->type == nt_plr)
	{
		ent_p = sub_vec2(ent_p, div_v2(td->e_sizes2d[nt_mob], 2));
		img = td->mobs_txtrs_mini[1][md->plr.action][md->plr.frame_index];
	}
	draw_img(img, md->screen, ent_p, -1);
}

void	render_2d_entities(t_md *md)
{
	t_dblst	*node;
	t_ent	*e;
	t_vec2	centr;

	centr = v2((md->win_sz.x * .5 - md->txd.size_2d * 2 - \
		(md->cam.ofst.x / md->t_len * md->txd.size_2d)), \
		md->win_sz.y * .5 - md->txd.size_2d * 2 - \
		((md->cam.ofst.y / md->t_len) * md->txd.size_2d));
	node = md->entities;
	while (node)
	{
		e = (t_ent *)node->content;
		if (e->is_active && e->type != nt_empty)
			render_2d_ent(md, e, centr);
		node = node->next;
	}
	render_2d_ent(md, &md->plr, centr);
}

void	apply_fx(t_md *md, t_image *screen, t_fx_data *fx)
{
	if (fx->barrel_amount > 0)
		apply_barrel_fx(screen, screen->size, \
			div_v2(md->win_sz, 2), fx->barrel_amount);
	if (fx->scanlines > 0)
		apply_scanlines(screen, fx->scanlines);
	if (fx->hue.r != 1 || fx->hue.g != 1 || fx->hue.b != 1)
		set_hue(screen, fx->hue);
	if (fx->bloom_threshold > 0)
		apply_bloom(screen, fx->bloom_threshold);
	if (fx->color_band > 0)
		apply_color_banding(screen, fx->color_band);
	if (fx->noise > 0)
		apply_noise(md, screen, fx->noise, 1);
	if (fx->contrast != 0.0f)
		apply_contrast(screen, fx->contrast);
	return ;
}

void	render(t_md *md)
{
	if (md->prm.show_sky && !cmp_vec2(md->mouse.delta_raw, _v2(0)))
		render_sky(md);
	cast_ray_threads_lp(md);
	if (md->prm.view_2d)
		render_2d_entities(md);
	if (md->prm.show_hud)
		render_hud_elements(md, &md->hud);
	if (md->inv.held_i > -1)
		render_held_item(md, &md->inv, md->inv.held_i);
	if (md->inv.active)
		render_inventory(md, &md->inv);
	render_minimap(md, &md->mmap);
	render_time_logs(md, &md->timer);
	if (md->prm.debug_mode)
		show_update_information(md);
	if (md->prm.show_fps)
		show_fps(md, v2(0, md->win_sz.y - (md->prm.txt_sc * 1.5)));
	apply_fx(md, md->screen, &md->fx);
	if (md->menu.active)
		draw_smooth_rec(md->screen, _v2(100), \
		sub_vec2(md->win_sz, _v2(200)), 0xEE000000);
	else
		mlx_put_image_to_window(md->mlx, md->win, md->screen->img, 0, 0);
}
