/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall_column.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:01:50 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/26 20:00:35 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	skip_pxl(t_md *md, t_ray_draw_d *d)
{
	int				rgb;
	int				is_transparent;
	int				is_portal_color;
	const t_ent		*hit = d->ray->wall_hit;

	if (!hit->hp && d->txd_crd.x > md->t_len * .1 && \
		d->txd_crd.x < md->t_len * .9)
		return (1);
	is_transparent = ((d->pxl_clr >> 24) != 0x00);
	if (is_transparent)
		return (1);
	if (!md->portal.ends[0].e || !md->portal.ends[1].e)
		return (0);
	rgb = d->pxl_clr & 0x00FFFFFF;
	is_portal_color = (rgb == 0x0000FF || rgb == 0xFF0000);
	if (is_portal_color)
	{
		d->has_portal = 1;
		return (1);
	}
	return (0);
}

static int	pxl_draw(t_md *md, t_ray_draw_d *d, t_vec2 win_sz)
{
	t_vec2		win_p;
	const int	max_i = (d->img->size.y * (d->img->size_line / 4));

	d->pxl_i = d->win_y * (d->img->size_line / 4) + d->txd_crd.x;
	if (d->pxl_i < 0 || d->pxl_i >= max_i)
		return (0);
	win_p.y = d->y_start + d->win_start.y - md->cam.pos.z;
	if (win_p.y > win_sz.y)
		return (0);
	d->pxl_clr = d->img->src[d->pxl_i];
	win_p.x = d->win_start.x;
	if (win_p.y < 0 || win_p.x < 0 || win_p.x > win_sz.x || skip_pxl(md, d))
		return (1);
	if (d->ray->wall_strip_pos.x == md->hud.floor_start)
		d->ray->wall_strip_pos.x = win_p.y;
	if (md->prm.show_walls != 1)
		return (1);
	if (md->cam.pointed == d->ray->wall_hit)
		d->pxl_clr = blend_color(d->pxl_clr, _WHITE, 0.11f);
	draw_pixel(md->screen, win_p, d->pxl_clr, 1);
	if (md->fx.fog)
		draw_pixel(md->screen, win_p, md->hud.fog_color, \
	minmaxf(0, .95, ((d->ray->distance / md->t_len) / 10) * md->fx.fog));
	return (1);
}

static void	draw_wall_strip(t_md *md, t_ray_draw_d *d, float step)
{
	const t_vec2	win_sz = md->win_sz;

	d->y_start = (win_sz.y / 2 - d->txd_crd.y / 2) - 1;
	d->y_end = (win_sz.y / 2 + d->txd_crd.y / 2);
	while (++d->y_start < d->y_end)
	{
		d->win_y = (d->y_start - (win_sz.y / 2 - d->txd_crd.y / 2)) * step;
		if (d->win_y < 0 || d->win_y >= d->img->size.y)
			continue ;
		if (d->txd_crd.x < 0 || d->txd_crd.x >= d->img->size.x)
			return ;
		if (!pxl_draw(md, d, win_sz))
			return ;
		d->ray->wall_strip_pos.y = d->y_start + d->win_start.y - md->cam.pos.z;
	}
}

static t_vec2	render_strip(t_md *md, t_ray *ray, \
	t_vec3f txtr_crd, t_vec3 screen_p)
{
	t_image			*img;
	t_wrd_dir		dir;
	t_ray_draw_d	draw_d;
	float			step;

	img = ray->wall_hit->frame;
	if (ray->wall_hit->type == nt_wall)
	{
		dir = get_wall_orientation(ray);
		if (!ray->wall_hit->overlay)
			ray->wall_hit->overlay_dir = dir;
		img = ray->wall_hit->frames[(int)dir];
		if (ray->wall_hit->overlay && ray->wall_hit->overlay_dir == dir)
			img = ray->wall_hit->overlay;
		ray->wall_hit->frame = img;
	}
	draw_d = (t_ray_draw_d){ray, ray->wall_hit->frame, \
		screen_p, screen_p, 0, 0, txtr_crd, 0, 0, 0, 9999, 0, 0, 0};
	step = draw_d.img->size.y / draw_d.txd_crd.y;
	draw_wall_strip(md, &draw_d, step);
	return (draw_d.ray->wall_strip_pos);
}

int	draw_wall_line(t_md *md, float dist, t_ent *hit, t_ray *ray)
{
	t_vec3f	txtr_cord;
	t_vec3	screen_pos;

	ray->wall_hit = hit;
	ray->distance = maxf(5, dist);
	txtr_cord.y = correct_fisheye(md, ray, hit, dist);
	if (ray->vertical_hit)
		txtr_cord.x = (int)fmod(ray->pos.y, hit->size.y);
	else
		txtr_cord.x = (int)fmod(ray->pos.x, hit->size.x);
	screen_pos.x = ray->index;
	screen_pos.y = compute_row_start(md, hit, ray->distance);
	screen_pos.z = ray->hits_len > 0;
	render_strip(md, ray, txtr_cord, screen_pos);
	if (md->cam.closest_x == ray->index)
		update_wall_pointed(md, hit, ray->wall_strip_pos);
	return (1);
}
