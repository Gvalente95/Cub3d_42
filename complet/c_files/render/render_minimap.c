/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_minimap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:59:55 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/24 13:27:51 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	show_revl_done(t_md *md, t_vec2 pos)
{
	float		revealed_perc_;
	int			color;
	t_txtd		data;

	revealed_perc_ = (100.0f / md->mmap.revealed_len) * md->mmap.revealed_cur;
	color = -1;
	data = (t_txtd){pos.x, pos.y, color, md->prm.txt_sc, NULL};
	rnd_fast_txt(md, data, "%.1f/", revealed_perc_);
}

void	render_minimap_ray(t_md *md)
{
	int				i;
	t_vec2			ray_start;
	t_vec2			ray_end;
	t_ray			*ray;
	const t_mmap	*mp = &md->mmap;

	i = -1;
	while (++i < md->win_sz.x)
	{
		if (!md->rays[i].active)
			continue ;
		ray = &md->rays[i];
		ray_start.x = (ray->start.x / md->t_len) * mp->ic_scl;
		ray_start.y = (ray->start.y / md->t_len) * mp->ic_scl;
		ray_end.x = (ray->pos.x / md->t_len) * mp->ic_scl;
		ray_end.y = (ray->pos.y / md->t_len) * mp->ic_scl;
		draw_line(mp->img, ray_start, ray_end, v2(0x00FFAA, 1));
	}
	ray = &md->rays[md->win_sz.x / 2];
	ray_start.x = (ray->start.x / md->t_len) * mp->ic_scl;
	ray_start.y = (ray->start.y / md->t_len) * mp->ic_scl;
	ray_end.x = (ray->pos.x / md->t_len) * mp->ic_scl;
	ray_end.y = (ray->pos.y / md->t_len) * mp->ic_scl;
	draw_line(mp->img, ray_start, ray_end, v2(_BLUE, 1));
}

void	show_if_interior(t_md *md, t_ent *e, t_vec2 draw_p, int scale)
{
	t_vec2	crd;
	t_txtd	txd;
	int		map_i;
	char	c;

	draw_p.x += 10;
	crd.x = minmax(0, md->map.size.x, (int)(e->pos.x / md->t_len));
	crd.y = minmax(0, md->map.size.y, (int)(e->pos.y / md->t_len));
	map_i = crd.y * (md->map.size.x + 1) + crd.x;
	if (map_i < 0 || map_i >= md->map.len)
		return ;
	c = md->out_map[map_i];
	if (c == ' ')
		c = 'E';
	txd = (t_txtd){draw_p.x, draw_p.y, -1, scale, md->mmap.img};
	rnd_fast_txt(md, txd, "%c", c);
}

void	show_minimap_entity(t_md *md, t_ent *e, t_image *screen, int no_redraw)
{
	int				draw_clr;
	t_vec2			pos;
	int				icsz;

	if (!e || (no_redraw && e->revealed))
		return ;
	draw_clr = md->rgb[e->type];
	icsz = md->mmap.ic_scl;
	pos = (t_vec2){1 + e->coord.x * icsz, 1 + e->coord.y * icsz};
	if (e->type == nt_plr)
	{
		pos.x = 1 + (e->pos.x / md->t_len) * icsz;
		pos.y = 1 + (e->pos.y / md->t_len) * icsz;
		icsz /= 2;
		draw_clr = md->rgb[RGB_BLUE + md->plr_in_house];
	}
	if (e->type == nt_empty)
		pos = sub_vec2(pos, _v2(1));
	draw_pixels(screen, sub_vec2(pos, _v2(1)), _v2(icsz + 1), _BLACK);
	draw_pixels(screen, pos, _v2(icsz - 1), draw_clr);
	if (!e->revealed && e->type != nt_plr)
		md->mmap.revealed_cur++;
	e->revealed = 1;
	if (e->type == nt_plr)
		show_if_interior(md, e, pos, -1);
}

void	render_minimap(t_md *md, t_mmap *mp)
{
	t_vec2			cntr;
	int				width;

	if (!mp->active)
	{
		rnd_fast_txt(md, (t_txtd){md->win_sz.x - 90, md->win_sz.y - 30, \
			_BLUE, md->prm.txt_sc, md->screen}, "M");
		return ;
	}
	if (mp->cmps)
	{
		width = mp->comps_scl * (8 + 6);
		cntr = v2(md->win_sz.x - width, md->win_sz.y - width);
		show_cmps_mmap(md, cntr, 8);
		return ;
	}
	flush_img(mp->img, _RED, 1, 0);
	remove_img_color(mp->img, _RED);
	draw_alpha_img(mp->bg, mp->img, _v2(0), 1);
	render_minimap_ray(md);
	show_minimap_entity(md, &md->plr, mp->img, 0);
	cntr = v2(md->win_sz.x - mp->img->size.x, \
		0);
	draw_alpha_img(mp->img, md->screen, cntr, 0.5);
	show_revl_done(md, v2(cntr.x, mp->img->size.y - md->prm.txt_sc));
}
