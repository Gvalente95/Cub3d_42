/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minimap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:48:25 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/24 13:26:09 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	set_base_mmap_bg(t_md *md, t_mmap *mmap, int ic_len)
{
	t_vec2		pos;
	int			i;
	char		c;

	i = -1;
	while (md->map.buffer[++i])
	{
		c = md->map.buffer[i];
		if (c == ' ' || c == '1')
			continue ;
		pos = v2(i % (md->map.size.x + 1), i / (md->map.size.x + 1));
		draw_pixels(mmap->bg, v2(ic_len * pos.x, ic_len * pos.y), \
			_v2(ic_len), _YELLOW);
		if (c != '1')
			continue ;
		draw_pixels(mmap->bg, v2(ic_len * pos.x - 1, ic_len * pos.y - 1), \
		_v2(ic_len - 1), _BLACK);
	}
}

static t_image	*init_full_map(t_md *md, t_vec2 win_sz, t_vec2 map_sz)
{
	t_image			*img;
	const t_vec2	t_sz = v2(win_sz.x / map_sz.x, win_sz.y / map_sz.y);
	int				i;
	char			c;
	t_vec2			map_pos;

	img = init_img(md, win_sz, NULL, _NULL);
	if (!img)
		return (NULL);
	i = -1;
	while (md->map.buffer[++i])
	{
		c = md->map.buffer[i];
		if (c == '\n')
			continue ;
		map_pos = v2(i % (map_sz.x + 1), i / (map_sz.x + 1));
		draw_pixels(img, v2(t_sz.x * map_pos.x, t_sz.y * map_pos.y), \
		t_sz, md->rgb[get_char_index(md->txd.ents_tp_map[0], c)]);
	}
	return (img);
}

void	init_minimap(t_md *md, t_mmap *mmap)
{
	int		ic_len;
	t_vec2	plr_tile;

	ic_len = minmax(6, 32, (md->win_sz.x / 2) / md->map.size.x);
	mmap->ic_scl = ic_len;
	mmap->comps_scl = minmax(6, 8, (md->win_sz.x / 4) / md->map.size.x);
	mmap->cmps = 1;
	mmap->mray_len = 0;
	mmap->revealed_cur = 0;
	mmap->active = 1;
	mmap->size = v2(md->map.size.x * ic_len, md->map.size.y * ic_len);
	mmap->full_map = init_full_map(md, md->win_sz, md->map.size);
	mmap->img = init_img(md, mmap->size, NULL, _NULL);
	mmap->bg = init_img(md, mmap->size, NULL, v4_to_color(0, 0, 0, 50));
	plr_tile.x = 1 + md->plr.coord.x * ic_len;
	plr_tile.y = 1 + md->plr.coord.y * ic_len;
	draw_pixels(mmap->bg, plr_tile, _v2(ic_len - 1), _WHITE);
}
