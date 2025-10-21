/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_inventory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:30:56 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/06 12:15:00 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	refresh_inv_opt_bgr(t_md *md, t_inventory *inv)
{
	t_image			*img;
	const char		labels[2][3][6] = \
		{{"USE", "THROW", "BACK"}, \
		{"INFO", "ABIL", "FREE"}};
	t_txtd			txd;
	int				i;
	int				is_sel;

	is_sel = minmax(0, 2, inv->opt_i);
	img = inv->opt_img;
	flush_img(img, _BLACK, 10, 0);
	txd = (t_txtd){0, 0, _WHITE, md->prm.txt_sc, img};
	i = -1;
	while (++i < 3)
	{
		if (i == is_sel)
			draw_pixels(img, v2(txd.x, txd.y), \
			v2(img->size.x, md->prm.txt_sc), _WHITE);
		rnd_fast_txt(md, txd, labels[inv->page][i]);
		txd.y += md->inv.line_height;
	}
	inv->update_opt = 0;
}

static void	draw_inv_labels(t_md *md, t_inventory *inv, t_txtd td, int brdsz)
{
	int	i;

	td.y += md->prm.txt_sc;
	i = -1;
	while (++i < PCKP_TYPE_LEN)
	{
		td.y += inv->line_height;
		td.color = _BLACK;
		if (inv->items[i] > 0)
			td.color = -1;
		if (i == inv->hov_indexes[inv->page])
			draw_pixels(inv->img, v2(td.x, td.y - 2), \
		v2(inv->sz.x * .75f - brdsz, md->prm.txt_sc * 1.4), _WHITE);
		rnd_fast_txt(md, td, "%-15s x%d", md->txd.item_names[i], \
			inv->items[i]);
	}
}

void	refresh_inv_bgr(t_md *md, t_inventory *inv)
{
	const char	title[3][10] = {"INVENTORY", "TEAM", "MINIMAP"};
	const int	brdsz = 4;
	const int	brd2 = brdsz * 2;
	t_image		*img;
	t_txtd		td;

	img = inv->img;
	td = (t_txtd){brd2, brd2, _MAGENT, md->prm.txt_sc, img};
	flush_img(img, _WHITE, 10, 0);
	draw_pixels(img, _v2(brdsz), sub_vec2(inv->sz, _v2(brd2)), _BLACK);
	if (inv->page == 0)
		draw_pixels(img, v2(inv->sz.x * .75f, td.y + 25), \
			v2(brdsz, inv->sz.y - (td.y + 25)), _WHITE);
	draw_pixels(img, v2(0, td.y + 25), v2(inv->sz.x, brdsz), _WHITE);
	td.x = inv->sz.x / 2 - (ft_strlen(title[inv->page]) * td.scale) / 2;
	rnd_fast_txt(md, td, title[inv->page]);
	td.x = brd2;
	if (inv->page == 0)
		draw_inv_labels(md, inv, td, brdsz);
	else if (inv->page == 1)
		draw_pokemon_team(md, inv, td, brdsz);
	inv->update_img = 0;
}

void	refresh_inv_map(t_md *md, t_inventory *inv, t_image *map_bg)
{
	t_image			*cp_map;
	const int		y_start = md->prm.txt_sc * 2;
	const t_vec2	sz = (t_vec2){inv->sz.x - 8, inv->sz.y - y_start};

	cp_map = copy_image(md, map_bg, sz, -1);
	if (inv->inv_map)
		free_image_data(md, inv->inv_map);
	inv->inv_map = cp_map;
	inv->update_map = 0;
}

void	render_inventory(t_md *md, t_inventory *inv)
{
	t_vec2	map_pos;

	draw_alpha_img(inv->img, md->screen, inv->p, .8f);
	if (inv->page == 2)
	{
		map_pos.x = inv->p.x + 4;
		map_pos.y = inv->p.y + md->prm.txt_sc * 2 + inv->inv_map->size.y / 2;
		draw_img(inv->inv_map, md->screen, map_pos, -1);
	}
	if (inv->sel_i != -1)
		draw_alpha_img(inv->opt_img, md->screen, inv->opt_p, .75);
}
