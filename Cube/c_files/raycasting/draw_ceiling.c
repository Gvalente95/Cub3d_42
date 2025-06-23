/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ceiling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 17:56:10 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/11 19:44:57 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	draw_ceiling_pxl(t_md *md, t_floor_draw_d d)
{
	const int	fog_clr = md->hud.fog_color;

	d.fogalpha = -1;
	if (md->fx.fog > 0)
		d.fogalpha = minmaxf(0, 1, (d.rwd / 10.0f) * md->fx.fog);
	if (d.fogalpha >= 1)
		return (draw_pixel(md->screen, d.win, fog_clr, d.fogalpha), 1);
	draw_pixel(md->screen, d.win, d.clr, 1);
	if (d.fogalpha > 0)
		draw_pixel(md->screen, d.win, fog_clr, d.fogalpha);
	return (1);
}

static int	set_ceiling_pxl(t_md *md, t_floor_draw_d *d)
{
	const t_image	*img = md->hud.ceiling;
	const t_vec2	img_sz = img->size;
	t_vec2f			flr_t;
	int				map_i;
	char			map_c;

	if (d->win.x < 0 || d->win.x >= md->win_sz.x)
		return (0);
	flr_t.x = d->flr.x + d->rwd * d->dirl.x + d->stp.x * d->win.x;
	flr_t.y = d->flr.y + d->rwd * d->dirl.y + d->stp.y * d->win.x;
	if (!md->plr_in_house && md->out_map[md->cam.plr_map_i] != 'D')
	{
		map_i = (int)flr_t.y * (md->map.size.x + 1) + (int)flr_t.x;
		if (map_i < 0 || map_i >= md->map.len)
			return (0);
		map_c = md->out_map[map_i];
		if (map_c != 'I')
			return (1);
	}
	d->txp.x = ((int)(flr_t.x * img_sz.x) % img_sz.x + img_sz.x) % img_sz.x;
	d->txp.y = ((int)(flr_t.y * img_sz.y) % img_sz.y + img_sz.y) % img_sz.y;
	d->clr = img->src[d->txp.y * (img->size_line / 4) + d->txp.x];
	return (draw_ceiling_pxl(md, *d));
}

t_vec2	get_tile_behind_door_from_angle(t_vec3 door, t_ray *ray)
{
	t_vec2		result;
	const float	dx = ray->dir.x;
	const float	dy = ray->dir.y;
	const int	step_x = (dx > 0.5f) - (dx < -0.5f);
	const int	step_y = (dy > 0.5f) - (dy < -0.5f);

	result.x = door.x + step_x;
	result.y = door.y + step_y;
	return (result);
}

int	calculate_door_y(t_md *md, t_floor_draw_d d)
{
	const float	dist = d.ray->dist_at_door;
	int			fisheye;
	int			door_y;
	t_vec2		exit_crd;
	int			map_i;

	door_y = md->win_sz.y - 1;
	if (!d.ray->had_door)
		return (door_y);
	exit_crd = get_tile_behind_door_from_angle(d.ray->door->coord, d.ray);
	map_i = exit_crd.y * (md->map.size.x + 1) + exit_crd.x;
	if (map_i > 0 && map_i < md->map.len && md->out_map[map_i] == 'I')
		return (door_y);
	fisheye = correct_fisheye(md, d.ray, d.ray->door, dist);
	door_y = md->win_sz.y / 2 - fisheye / 2 - 1;
	door_y += compute_row_start(md, d.ray->door, dist);
	return (door_y);
}

void	draw_ceiling(t_md *md, t_floor_draw_d d, float pitch_offset)
{
	d.door_y_start = calculate_door_y(md, d);
	d.win.y = (d.ray->wall_strip_pos.x + md->prm.height + 1);
	if (md->prm.show_sky)
		draw_strip(md->hud.sky_buffer, md->screen, _v2(d.win.x), d.win.y);
	else
		draw_pixels(md->screen, v2(d.win.x, 0), v2(1, d.win.y), \
		md->hud.ceiling_color);
	d.p = (md->win_sz.y / 2 - pitch_offset) - d.win.y;
	while (d.win.y-- > 0)
	{
		if (d.win.y > d.door_y_start)
		{
			d.p++;
			continue ;
		}
		if (d.p == 0)
			d.p = 1;
		d.rwd = ((0.5 + (md->cam.pos.z / md->t_len)) * md->win_sz.y) / d.p;
		d.stp.x = d.rwd * (d.dirr.x - d.dirl.x) / md->win_sz.x;
		d.stp.y = d.rwd * (d.dirr.y - d.dirl.y) / md->win_sz.x;
		if (!set_ceiling_pxl(md, &d))
			break ;
		d.p++;
	}
}
