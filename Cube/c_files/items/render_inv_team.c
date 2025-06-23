/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_inv_team.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:28:17 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/05 14:10:20 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	draw_team_slot(t_md *md, t_txtd td, t_vec2 spacing, t_vec2 base)
{
	const t_vec2		slot_size = _v2(md->inv.sz.x / 4);
	t_ent				*pk;
	t_vec2				pos;
	t_vec2				sc;
	t_image				*img;

	sc = slot_size;
	if (md->var == md->inv.hov_indexes[1])
		sc = _v2(slot_size.x * 1.4);
	pos.x = base.x + (md->var % 2) * (slot_size.x + spacing.x);
	pos.y = base.y + (md->var / 2) * spacing.y;
	draw_sphere(md->inv.img, pos, sc, v3(md->rgb[RGB_WHITE], 2, 1));
	pk = md->inv.pokemon_team[md->var];
	if (!pk)
		return ;
	img = copy_image(md, pk->frame, _v2(sc.x), -1);
	img->pos = add_vec2(_v2(slot_size.x / 2), sub_vec2(pos, _v2(sc.x / 2)));
	draw_img(img, md->inv.img, img->pos, -1);
	free_image_data(md, img);
	draw_hp_bar(md, pk, v2(pos.x, pos.y + sc.y), v2(100, 20));
	if (md->inv.hov_indexes[1] != md->var)
		return ;
	td.x = pos.x + slot_size.x / 2 - ft_strlen(pk->label) * td.scale / 2;
	td.y = pos.y + sc.y - md->prm.txt_sc * 1.3;
	rnd_fast_txt(md, td, pk->label);
}

void	draw_pokemon_team(t_md *md, t_inventory *inv, t_txtd td, int brdsz)
{
	const t_vec2	slt_sz = _v2(inv->sz.x / 4);
	const t_vec2	spacing = v2(slt_sz.x - 10, slt_sz.y + 10);
	const t_vec2	lay = v2(2 * slt_sz.x + spacing.x, 3 * slt_sz.y + 2 * 10);
	const t_vec2	base = v2(((inv->sz.x - lay.x) / 2 + brdsz / 2), \
		(inv->sz.y - lay.y) / 2);

	md->var = -1;
	while (++md->var < 6)
		draw_team_slot(md, td, spacing, base);
	md->var = 0;
}

void	render_used_shadow(t_md *md, t_vec2 usd_p, t_vec2 u_sz, double dur)
{
	t_vec2			p;
	t_vec2			sz;
	const double	elapsed = md->timer.cur_tm - md->inv.held_used_start;

	sz = v2(u_sz.x, u_sz.y / 2);
	p.x = usd_p.x;
	p.y = usd_p.y + u_sz.y * .8f;
	p.y = maxf(usd_p.y + u_sz.y * .8f, md->win_sz.y * .9f - md->cam.rot.y * 4);
	p.y -= minf(elapsed, dur) * 400;
	draw_sphere(md->screen, p, sz, v3(_BLACK, 1, 1));
}
