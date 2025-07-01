/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:36:33 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/07/01 14:59:13 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	init_colors(t_md *md)
{
	md->rgb[RGB_RED] = _RED;
	md->rgb[RGB_GREEN] = _GREEN;
	md->rgb[RGB_BLUE] = _BLUE;
	md->rgb[RGB_CYAN] = _CYAN;
	md->rgb[RGB_MAGENT] = _MAGENT;
	md->rgb[RGB_GRAY] = _GRAY;
	md->rgb[RGB_BROWN] = _BROWN;
	md->rgb[RGB_TEAL] = _TEAL;
	md->rgb[RGB_LIME] = _LIME;
	md->rgb[RGB_GOLD] = _GOLD;
	md->rgb[RGB_SILVER] = _SILVER;
	md->rgb[RGB_TURQ] = _TURQ;
	md->rgb[RGB_INDIGO] = _INDIGO;
	md->rgb[RGB_VIOLET] = _VIOLET;
	md->rgb[RGB_CORAL] = _CORAL;
	md->rgb[RGB_WHITE] = _WHITE;
	md->rgb[RGB_BLACK] = _BLACK;
	md->rgb[RGB_YELLOW] = _YELLOW;
	md->rgb[RGB_ORANGE] = _ORANGE;
	md->rgb[RGB_NULL] = _NULL;
}

static void	init_portal_data(t_md *md)
{
	md->portal.ends[0].e = NULL;
	md->portal.ends[1].e = NULL;
	md->portal.found = NULL;
	md->portal.last_shot_index = 0;
	md->portal.out_pos = _v2(90);
}

static void	init_game_params(t_md *md, t_parameters *prm, int start_debug)
{
	prm->debug_mode = start_debug;
	prm->show_rays = prm->debug_mode;
	prm->ray_depth = md->t_len * RAY_DEPTH;
	prm->resolution = RESOLUTION;
	prm->fly_cam = 0;
	prm->ent_mode = 0;
	prm->use_thrd = 1;
	prm->show_grass = 0;
	prm->view_2d = 0;
	prm->show_ceiling = 1;
	prm->show_floor = 1;
	prm->show_walls = 1;
	prm->show_sky = 0;
	prm->alternate_draw = 1;
	prm->ray_mod = 1;
	prm->show_fps = 1;
	prm->show_hud = 1;
	prm->super_view = 1;
	prm->max_view_sprite = MAX_RAY_SPRITE;
	prm->au_on = !md->is_linux;
	update_phys(md);
}

static void	init_var(t_md *md)
{
	const int		alrt_scl = md->prm.txt_sc;
	const t_vec2	alrt_p = (t_vec2){-1, md->win_sz.y * .7f};

	md->alert.txt_d = (t_txtd){alrt_p.x, alrt_p.y, _RED, alrt_scl, md->screen};
	md->alert.duration = -1;
	md->plr.was_hit = 0;
	md->plr.grounded = 0;
	md->plr_in_house = 1;
	md->cam.is_moving = 0;
	md->cam.bob_time = 0.0f;
	md->cam.pointed = NULL;
	md->cam.prv_pointed = NULL;
	md->cam.prv_door = NULL;
	md->cam.pointed_ent = NULL;
	md->cam.prv_pointed_ent = NULL;
	md->cam.last_pointed_ent_pos = _v2(-1);
	md->au.mus_pid = 0;
	md->au.wind_pid = 0;
	md->au.walk_index = 0;
	md->txd.size_2d = SCRN_H / 20;
	md->switch_interior = 0;
	md->inv.active = 0;
	md->prm.alternate_draw = 0;
	md->autocam.active = 1;
	md->map.accept_unvalid = 0;
}

int	init_cube(t_md *md, char *file_arg, int strictMode)
{
	md->strict_mode = !strictMode;
	init_colors(md);
	init_game_params(md, &md->prm, 0);
	init_var(md);
	md->hud.fog_color = _WHITE;
	init_portal_data(md);
	init_ents_data(md, &md->txd);
	init_map(md, file_arg);
	init_inventory(md, &md->inv);
	init_battle_data(md, &md->battle_d);
	md->init_steps++;
	init_hud(md, &md->hud);
	init_entities(md, v2(0, 0));
	init_minimap(md, &md->mmap);
	init_menu(md, &md->menu);
	init_fes(md, &md->env, md->t_len);
	md->out_map = get_out_map(md->map.buffer, md->map.size.x + 1, md->map.len);
	init_thread_pool(md, THREADS_BATCH);
	md->timer.game_start = get_time_in_seconds();
	md->timer.elapsed_pause = md->timer.game_start;
	md->init_steps++;
	ft_strlcpy(md->plr_name, " ", 15);
	md->plr_name_indx = 0;
	show_init_information(md);
	return (0);
}
