/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_fe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:24:01 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/23 18:10:07 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	render_fe(t_md *md, t_fe *fe, t_vec2 pos, int width)
{
	float		shade;
	t_vec4		rgb;
	float		shading;
	t_image		*dst;
	float		blend;

	dst = md->env.grass_overlay;
	while (++pos.y < fe->end_y)
	{
		if (pos.y >= dst->size.y || pos.y < 0)
			continue ;
		if (dst->src[pos.y * (dst->size_line / 4) + pos.x] != _NULL)
			continue ;
		shade = 0.5f + 0.5f * ((float)(fe->end_y - pos.y) / (float)fe->height);
		rgb = color_to_v4(fe->base_color);
		shading = shade * (1.0f - fe->fogalpha);
		rgb.r = min(255, (rgb.r * shading) + fe->age * fe->growth_factor);
		rgb.g *= shading;
		rgb.b *= shading;
		blend = (float)(pos.y - fe->pos.y) / (float)fe->height;
		rgb.a = (blend) * 255.0f;
		draw_pixels((t_image *)dst, pos, v2(width, 1), \
			v4_to_color(rgb.r, rgb.g, rgb.b, 255));
	}
	return (fe->height);
}

void	update_fe(t_md *md, t_fe *fe, t_floor_draw_d *d)
{
	t_vec2			pos;
	int				height;
	const int		height_max = md->prm.grass_sz.y + fe->height_max;
	const t_vec2	start = d->win;

	height = fe->size.y + (fe->growth_factor * md->timer.fe_time) - fe->cut_len;
	if (height > height_max)
		height = height_max;
	height /= d->rwd;
	fe->height = max(1, height);
	fe->size.x = max(1, md->prm.grass_sz.x / d->rwd);
	pos.x = start.x;
	pos.y = start.y - height;
	if (pos.y < md->env.overlay_y_start)
		md->env.overlay_y_start = pos.y;
	fe->pos = pos;
	fe->end_y = start.y;
	fe->fogalpha = d->fogalpha;
}

void	render_world_map(t_md *md, t_floor_draw_d d, int y_size)
{
	const t_vec2	draw_p = (t_vec2){d.win.x, d.win.y - y_size};
	char			c;
	int				map_i;
	int				clr;
	t_vec2			cord_transl;

	cord_transl = div_v2(d.txp, 8);
	if (cord_transl.x == md->plr.coord.x && cord_transl.y == md->plr.coord.y)
	{
		draw_pixels(md->screen, v2(d.win.x, d.win.y - y_size * 3), \
			v2(1, y_size * 2), _RED);
		clr = _GOLD;
		draw_pixels(md->screen, draw_p, v2(1, y_size), clr);
		return ;
	}
	map_i = cord_transl.y * (md->map.size.x + 1) + cord_transl.x;
	if (map_i < 0 || map_i > md->map.len)
		return ;
	draw_pixel(md->screen, v2(draw_p.x, draw_p.y + 10), _BLACK, .5f);
	c = md->map.buffer[map_i];
	if (c == '0' || c == ' ')
		return ;
	clr = md->rgb[get_char_index(md->txd.ents_tp_map[0], c)];
	draw_pixels(md->screen, v2(draw_p.x, draw_p.y), v2(1, y_size), clr);
}

int	update_and_render_fe(t_md *md, t_floor_draw_d *d, t_vec2 t_crd)
{
	t_fe				*fe;
	const t_env_manager	*env = &md->env;

	if ((int)t_crd.x >= md->map.size.x - 1 || (int)t_crd.x < 0 || \
	(int)t_crd.y >= md->map.size.y - 1 || (int)t_crd.y < 0)
		return (1);
	if (d->txp.x >= md->t_len || d->txp.x < 0 || \
		d->txp.y >= md->t_len || d->txp.y < 0)
		return (1);
	fe = &env->grass[t_crd.y][t_crd.x][d->txp.y][d->txp.x];
	if (!fe->active || fe->was_drawn == md->timer.time)
		return (1);
	update_fe(md, fe, d);
	render_fe(md, fe, fe->pos, fe->size.x);
	fe->was_drawn = md->timer.time;
	return (1);
}

float	get_pitch_offset(t_md *md)
{
	const float		pitch = tanf(md->cam.rot.y * (_PI / 180.0f));
	const float		pitch_offset = pitch * md->win_sz.y / 2;

	return (pitch_offset);
}
