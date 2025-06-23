/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:34:06 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 13:24:57 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	refresh_battleground(t_md *md, t_battle_d *bd)
{
	const t_parameters	prv_prm = md->prm;
	const t_vec3f		prv_cam = md->cam.rot;
	const int			prv_map_active = md->mmap.active;
	const t_vec3f		prv_plr_p = md->plr.pos;
	const t_vec3f		prv_plr_dir = md->plr.dir;

	md->mmap.active = 0;
	md->prm.show_hud = 0;
	md->prm.show_fps = 0;
	md->prm.alternate_draw = 0;
	md->plr.pos = (t_vec3f){md->map.size.x / 2 * md->t_len, -md->t_len * 5, 0};
	md->plr.dir.x = 90;
	md->cam.rot.x = 90;
	md->cam.rot.y = 25;
	md->plr.dir.y = 25;
	md->prm.ray_mod = 0;
	update_player(md, &md->plr);
	render(md);
	draw_img(md->screen, bd->overlay, _v2(0), _WHITE);
	md->prm = prv_prm;
	md->plr.pos = prv_plr_p;
	md->plr.dir = prv_plr_dir;
	md->mmap.active = prv_map_active;
	md->cam.rot = prv_cam;
}

void	refresh_combat_screen(t_md *md, t_battle_d *bd)
{
	t_vec2	txt_p;

	draw_img(bd->overlay, md->screen, _v2(0), -1);
	render_batt_pkmn_0(md, bd, bd->pk[0], bd->pk_basep[0]);
	render_batt_pkmn_1(md, bd, bd->pk[1], bd->pk_basep[1]);
	if (bd->trans_lvl == 1)
		return ;
	if (bd->my_turn && !bd->stored_dealt[0] && \
		!bd->stored_dealt[1] && !bd->trans_lvl)
		render_battle_buttons(md, bd);
	if (bd->trainer)
		render_team_logos(md, bd, 0, bd->trainer->team_sz);
	render_team_logos(md, bd, 1, md->inv.team_size);
	if (bd->trans_lvl == 0)
	{
		bd->hp_pos[0] = (t_vec2){bd->pk_p[0].x, bd->pk_p[0].y - 40};
		bd->hp_pos[1] = (t_vec2){bd->pk_p[1].x, bd->pk_p[1].y - 40};
		draw_hp_bar(md, bd->pk[0], bd->hp_pos[0], (t_vec2){bd->pk_sz.x, 30});
		draw_hp_bar(md, bd->pk[1], bd->hp_pos[1], (t_vec2){bd->pk_sz.x, 30});
	}
	txt_p = bd->pk_basep[!bd->my_turn];
	txt_p.y += bd->pk_sz.y * 1.2f;
	rnd_txt_simple(md, txt_p, bd->log_message);
}

void	refresh_battle_screen(t_md *md, t_battle_d *bd)
{
	refresh_combat_screen(md, bd);
	mlx_put_image_to_window(md->mlx, md->win, md->screen->img, 0, 0);
}

void	render_trans_screen(t_md *md, t_battle_d *bd, double elapsed)
{
	int				offst;
	int				i;
	t_vec2			s;
	const int		slh = md->win_sz.y / 50;
	const double	t = minf(1.0, elapsed / bd->trans_dur);

	update_trans_lvl(bd, elapsed);
	bd->pk_basep[0].x = max(bd->pk_p[0].x, 0 + (bd->pk_p[0].x - 0) * t);
	bd->pk_basep[1].x = \
		min(bd->pk_p[1].x, md->win_sz.x + (bd->pk_p[1].x - md->win_sz.x) * t);
	refresh_battle_screen(md, bd);
	offst = minmax(0, md->win_sz.x, (elapsed * md->win_sz.x) / bd->trans_dur);
	s = (t_vec2){md->win_sz.x - offst, slh};
	i = -1;
	while (++i < 50)
	{
		if (i % 2 == 0)
			draw_pixels(md->screen, (t_vec2){0, slh * i}, s, _BLACK);
		else
			draw_pixels(md->screen, \
				(t_vec2){md->win_sz.x - s.x, slh * i}, s, _BLACK);
	}
	mlx_put_image_to_window(md->mlx, md->win, md->screen->img, 0, 0);
	reset_mlx_values(md);
}

void	render_team_logos(t_md *md, t_battle_d *bd, int side, int team_size)
{
	int		i;
	t_vec2	pos;
	t_ent	**pokemons;
	t_image	*img;
	int		clr;

	if (side == 0 && !bd->trainer)
		return ;
	pos = v2(bd->hp_pos[side].x, bd->hp_pos[side].y - 40);
	pokemons = md->inv.pokemon_team;
	if (side == 0)
		pokemons = bd->trainer->pk_team;
	i = -1;
	while (++i < 6)
	{
		clr = -1;
		if (i >= team_size || !pokemons[i])
			clr = _BLACK;
		else if (pokemons[i]->hp <= 0)
			clr = v4_to_color(80, 80, 80, 80);
		img = copy_image(md, md->txd.item_txtr[Pokeball][0], _v2(40), clr);
		draw_img(img, md->screen, pos, -1);
		free_image_data(md, img);
		pos.x += 40;
	}
}
