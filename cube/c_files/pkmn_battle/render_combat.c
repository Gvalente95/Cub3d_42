/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_combat.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:29:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/08 17:27:48 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	render_batt_pkmn_0(t_md *md, t_battle_d *bd, t_ent *pok, t_vec2 base_p)
{
	t_vec2			f_p;
	t_image			*img;
	int				clr;
	const t_vec2	sz = scale_vec2(bd->pk_sz, .6f);

	bd = &md->battle_d;
	f_p = base_p;
	if (bd->stored_dealt[0] > 0)
		f_p.x += r_range(-10, 10);
	if (bd->stored_dealt[1] != 0)
		f_p = add_vec2(f_p, scale_vec2(sub_vec2(bd->pk_p[1], f_p), 0.1));
	clr = -1;
	if (!pok->hp || bd->trans_lvl == 1)
		clr = _BLACK;
	else if (bd->stored_dealt[0])
		clr = md->rgb[bd->stored_dealt[0] < 0];
	if (bd->trans_lvl && bd->trainer)
		img = copy_image(md, bd->trainer->frame, sz, clr);
	else
		img = copy_image(md, pok->frame, sz, clr);
	draw_sphere(md->screen, \
		(t_vec2){f_p.x - sz.x * .1, f_p.y + sz.y * .7}, \
		(t_vec2){sz.x * 1.2, sz.y * .3}, (t_vec3){_BLACK, 5, 1});
	draw_img(img, md->screen, f_p, -1);
	free_image_data(md, img);
}

void	render_batt_pkmn_1(t_md *md, t_battle_d *bd, t_ent *pok, t_vec2 base_p)
{
	t_vec2			final_p;
	t_image			*img;
	int				clr;
	const t_vec2	sz = bd->pk_sz;

	final_p = base_p;
	if (bd->stored_dealt[1] > 0)
		final_p.x += r_range(-10, 10);
	if (bd->stored_dealt[0] != 0)
		final_p = add_vec2(final_p, \
	scale_vec2(sub_vec2(bd->pk_p[!1], final_p), 0.1));
	clr = -1;
	if (!pok->hp || bd->trans_lvl == 1)
		clr = _BLACK;
	else if (bd->stored_dealt[1])
		clr = md->rgb[bd->stored_dealt[1] < 0];
	img = copy_image(md, pok->frame, sz, clr);
	flipx_image_data(img);
	draw_img(img, md->screen, final_p, -1);
	draw_sphere(md->screen, \
		(t_vec2){final_p.x, final_p.y + sz.y * .8}, \
		(t_vec2){sz.x * 1.2, sz.y * .3}, (t_vec3){_BLACK, 4, 1});
	free_image_data(md, img);
}

void	render_sub_text(t_md *md, t_vec2 sub_p, int i, int j)
{
	t_txtd				txt;
	const t_battle_d	bd = md->battle_d;
	const char			atk_names[4][10] = \
		{"Bite", "Charge", "Griffe", "Stuff"};

	txt = (t_txtd){sub_p.x + 20, sub_p.y, -1, -1, md->screen};
	txt.y += bd.but_sz.y / 2 - md->prm.txt_sc / 2;
	if (bd.in_sub && bd.sub_i == j)
		txt.color = _RED;
	if (i == 0)
		rnd_fast_txt(md, txt, "%-9.9s %d", atk_names[j], 30);
	else if (i == 1)
		rnd_fast_txt(md, txt, \
			"%-9.9s x%d", md->txd.item_names[j], md->inv.items[j]);
	else if (i == 2 && !md->inv.pokemon_team[j])
		rnd_fast_txt(md, txt, "NULL");
	else if (i == 2)
		rnd_fast_txt(md, txt, md->inv.pokemon_team[j]->label);
}

void	draw_sub_options(t_md *md, t_battle_d *bd, int i, int elem_len)
{
	int			j;
	t_image		*but;
	t_vec2		sub_p;
	int			is_sub_sel;

	sub_p = (t_vec2){bd->but_ps[i].x + bd->but_sz.x, bd->but_ps[i].y};
	but = init_img(md, v2(bd->but_sz.x * 1.5, bd->but_sz.y), NULL, -1);
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
		draw_alpha_img(but, md->screen, sub_p, .2 + (is_sub_sel * .4));
		render_sub_text(md, sub_p, i, j);
		sub_p = add_vec2(sub_p, (t_vec2){5, but->size.y * 1.1});
	}
	free_image_data(md, but);
}

void	render_battle_buttons(t_md *md, t_battle_d *bd)
{
	int			i;
	const int	sub_len[4] = {4, PCKP_TYPE_LEN - 1, md->inv.team_size, 0};

	i = -1;
	while (++i < 4)
	{
		if (v2_touch(md->mouse.real, _v2(2), bd->but_ps[i], bd->but_sz))
		{
			bd->in_sub = 0;
			bd->opt_i = i;
		}
		if (bd->opt_i != i)
			draw_alpha_img(bd->buttons[i], md->screen, bd->but_ps[i], .3);
		else
			draw_img(bd->buttons[i], md->screen, bd->but_ps[i], -1);
		if (bd->opt_i == i && sub_len[i] != 0)
			draw_sub_options(md, bd, i, sub_len[i]);
	}
}
