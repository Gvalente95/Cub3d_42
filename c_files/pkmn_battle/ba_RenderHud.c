/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ba_RenderHud.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:29:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/10 01:09:45 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	render_hp_bars(t_md* md, t_BA_d* bd) {
	for (int i = 0; i < 2; i++) {
		t_ent* pk = bd->pk[i];
		if (i == 0 && !bd->opponent->is_trainer) pk = bd->opponent;
		if (!pk)
			continue;
		bd->hp_pos[i] = (t_vec2){ bd->pk_p[i].x, bd->pk_p[i].y - 40 };
		t_vec2 size = (t_vec2){ bd->pk_sz.x, 30 };
		draw_hp_bar(md, pk, bd->hp_pos[i], size);
		if (pk->pkStatus.active) {
			rnd_fast_txt(md,
			(t_txtd) {bd->hp_pos[i].x, bd->hp_pos[i].y + size.y, pk->pkStatus.color, -1, md->screen},
			"%s", pk->pkStatus.abbrev);
		}
	}
}

void	render_team_logos(t_md *md, t_BA_d *bd, int side, int team_size)
{
	int		i;
	t_vec2	pos;
	t_ent	**pokemons;
	t_image	*img;
	int		clr;

	pos = v2(bd->hp_pos[side].x, bd->hp_pos[side].y - 40);
	pokemons = side == BME ? md->inv.pokemon_team : bd->opponent->pk_team;
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

void	render_sub_text(t_md *md, t_vec2 sub_p, int i, int j)
{
	t_txtd				txt;
	const t_BA_d	bd = md->BA_d;

	txt = (t_txtd){ sub_p.x + 20, sub_p.y, -1, -1, md->screen };
	txt.y += bd.but_sz.y / 2 - md->prm.txt_sc / 2;
	if (bd.in_sub && bd.sub_i == j)
		txt.color = _SILVER;
	if (i == 0) {
		t_ent* pk = md->BA_d.pk[BME];
		if (!pk) return;
		t_move* moves = pk->poke_moves;
		rnd_fast_txt(md, txt, "%-9.9s %d/%d", moves[j].name, moves[j].cc, moves[j].ccMax);
	}
	else if (i == 1)
		rnd_fast_txt(md, txt, \
			"%-9.9s x%d", md->txd.item_names[j], md->inv.items[j]);
	else if (i == 2 && !md->inv.pokemon_team[j])
		rnd_fast_txt(md, txt, "NULL");
	else if (i == 2)
		rnd_fast_txt(md, txt, md->inv.pokemon_team[j]->label);
}

void	render_sub_options(t_md *md, t_BA_d *bd, int i, int elem_len)
{
	int			j;
	t_image		*but;
	t_vec2		sub_p;
	int			is_sub_sel;

	if (i == 0 && !bd->pk[BME])
		return;
	sub_p = (t_vec2){ bd->but_ps[i].x + bd->but_sz.x, bd->but_ps[i].y };
	but = init_img(md, v2(bd->but_sz.x * 1.5, bd->but_sz.y), NULL, -1);
	if (i == 1)
		flush_gradient(but, _GREEN, _MAGENT, 10);
	j = -1;
	while (++j < elem_len)
	{
		if (v2_touch(md->mouse.real, _v2(2), sub_p, but->size))
		{
			bd->in_sub = 1;
			bd->sub_i = j;
		}
		is_sub_sel = (bd->in_sub && bd->sub_i == j);
		if (is_sub_sel && i == 2 && j < md->inv.team_size && md->inv.pokemon_team[j]->hp <= 0) is_sub_sel = 0;
		if (i == 0) {
			int color = getPkElColor(md, bd->pk[1]->poke_moves[j].elementType);
			flush_img(but, color, 10, -1);
		}
		if (i == 2) {
			int color = getPkElColor(md, md->inv.pokemon_team[j]->elemType);
			flush_img(but, color, 10, -1);
		}
		draw_alpha_img(but, md->screen, sub_p, .5 + (is_sub_sel * .4));
		render_sub_text(md, sub_p, i, j);
		sub_p = add_vec2(sub_p, (t_vec2){5, but->size.y * 1.1});
	}
	free_image_data(md, but);
}

void	render_BA_buttons(t_md *md, t_BA_d *bd)
{
	int			i;
	const int	sub_len[4] = { 4, PCKP_TYPE_LEN - 1, md->inv.team_size, 0 };
	const int	displayAttackMenu = bd->pk[1] != NULL;

	i = -1;
	while (++i < 4)
	{
		if (v2_touch(md->mouse.real, _v2(2), bd->but_ps[i], bd->but_sz))
		{
			bd->in_sub = 0;
			bd->opt_i = i;
		}
		if (bd->opt_i != i || (i == 0 && !displayAttackMenu))
			draw_alpha_img(bd->buttons[i], md->screen, bd->but_ps[i], .3);
		else
			draw_img(bd->buttons[i], md->screen, bd->but_ps[i], -1);
		if (bd->opt_i == i && sub_len[i] != 0 && (i != 0 || displayAttackMenu))
			render_sub_options(md, bd, i, sub_len[i]);
	}
}

void	render_BA_hud(t_md* md, t_BA_d* bd) {
	t_vec2	txt_p;

	if (!bd->action_type && bd->bstate == BT_ON && bd->turn)
		render_BA_buttons(md, bd);
	if (bd->opponent->is_trainer)
		render_team_logos(md, bd, 0, bd->opponent->team_sz);
	render_team_logos(md, bd, 1, md->inv.team_size);
	render_hp_bars(md, bd);
	txt_p = bd->pk_basep[bd->turn];
	txt_p.y += bd->pk_sz.y * 1.2f;
	rnd_txt_simple(md, txt_p, bd->log_message);
}
