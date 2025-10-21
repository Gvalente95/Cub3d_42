/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall_column.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:01:50 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/16 14:59:38 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	skip_pxl(t_md *md, t_ray_draw_d *d)
{
	const t_ent		*hit = d->ray->wall_hit;

	if (!hit->hp && d->txd_crd.x > md->t_len * .1 && \
		d->txd_crd.x < md->t_len * .9)
		return (1);
	if ((d->pxl_clr >> 24) != 0x00)
		return (1);
	return (0);
}

static int	pxl_draw(t_md *md, t_ray_draw_d *d, t_ent *hit)
{
	d->pxl_clr = d->img->src[d->pxl_i];
	if (skip_pxl(md, d))
		return (1);
	if (d->ray->wall_strip_pos.x == md->hud.floor_start)
		d->ray->wall_strip_pos.x = d->winp.y;
	if (md->prm.show_walls != 1)
		return (1);
	if (d->ray->is_double_hit && hit->type == nt_wall)
		d->pxl_clr = 0x333333;
	if (md->cam.pointed == hit)
		d->pxl_clr = blend_color(d->pxl_clr, _WHITE, 0.21f);
	draw_pixel(md->screen, d->winp, d->pxl_clr, 1);
	if (md->fx.fog)
		draw_pixel(md->screen, d->winp, md->hud.fog_color, \
	minmaxf(0, .95, ((d->ray->distance / md->t_len) / 10) * md->fx.fog));
	d->ray->wall_strip_pos.y = d->winp.y;
	d->ray->dirty_checks[d->winp.y] = 1;
	return (1);
}

static void draw_bottom_strip(t_md* md, t_ray_draw_d* d) {
	t_vec2 p = d->winp;
	int raw_winy = 0;
	for (int i = 0; i < raw_winy; i++) {
		p.y++;
		if (d->ray->dirty_checks[p.y])
			continue;
		draw_pixel(md->screen, p, _RED, 1);
	}
}

static void	draw_wall_strip(t_md *md, t_ray_draw_d *d, t_ent *hit, float step)
{
	const t_vec2	win_sz = md->win_sz;
	int				prev_start;

	if (d->txd_crd.x < 0 || d->txd_crd.x >= d->img->size.x)
		return;
	int txtStartY = win_sz.y / 2 - d->txd_crd.y / 2;
	prev_start = d->y_start;
	while (++d->y_start < d->y_end)
	{
		d->winp.y++;
		if (d->ray->dirty_checks[d->winp.y]) continue;
		d->txtY = (d->y_start - txtStartY) * step;
		d->pxl_i = d->txtY * (d->img->size_line / 4) + d->txd_crd.x;
		if (d->pxl_i < 0 || d->pxl_i > d->img->size.y * (d->img->size_line / 4))
			continue;
		if (!pxl_draw(md, d, hit))
			return ;
	}
	if (hit->pos.z < 0)
		draw_bottom_strip(md, d);
	if (hit->pos.z < 0 && !md->prm.x_vision)
		draw_wall_shadow(md, d, prev_start);
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
	draw_d = (t_ray_draw_d){
		.ray = ray,
		.img = ray->wall_hit->frame, \
		.win_start = screen_p,
		.wp = screen_p,
		.txd_crd = txtr_crd,
		.y_start = (md->win_sz.y / 2 - txtr_crd.y / 2) - 1,
		.y_end = (md->win_sz.y / 2 + txtr_crd.y / 2),
		.winp = (t_vec2){ 0, 0 } };
	draw_d.winp.y = draw_d.y_start + draw_d.win_start.y - md->cam.pos.z + (ray->wall_hit->pos.z / ray->distance) * 1.75;
	if (draw_d.winp.y < 0) {
		draw_d.y_start += -draw_d.winp.y;
		draw_d.winp.y = 0;
	}
	if (draw_d.winp.y + draw_d.y_end - draw_d.y_start > md->win_sz.y) {
		int overflow = (draw_d.winp.y + draw_d.y_end - draw_d.y_start) - md->win_sz.y;
		draw_d.y_end -= overflow - 1;
	}
	draw_d.winp.x = draw_d.win_start.x;
	if (draw_d.winp.x < 0 || draw_d.winp.x > md->win_sz.x)
		return (v2(-1, -1));
	draw_d.wp.z = draw_d.y_end - draw_d.y_start;
	step = draw_d.img->size.y / draw_d.txd_crd.y;
	draw_wall_strip(md, &draw_d, ray->wall_hit, step);
	return (draw_d.ray->wall_strip_pos);
}

int	draw_grass(t_md* md, float dist, t_ent* hit, t_ray* ray) {
	(void)md;
	(void)dist;
	(void)hit;
	(void)ray;
	return 1;
}

int	draw_wall_line(t_md *md, float dist, t_ent *hit, t_ray *ray)
{
	t_vec3f	txtr_cord;
	t_vec3	screen_pos;

	if (hit->type == nt_grass)
		return (draw_grass(md, dist, hit, ray));
	ray->wall_hit = hit;
	ray->distance = maxf(.3, dist);
	txtr_cord.y = correct_fisheye(md, ray, hit, dist);
	if (ray->vertical_hit)
		txtr_cord.x = (int)fmod(ray->pos.y, hit->size.y);
	else
		txtr_cord.x = (int)fmod(ray->pos.x, hit->size.x);
	screen_pos.x = ray->index;
	screen_pos.y = compute_row_start(md, ray->distance);
	screen_pos.z = ray->hits_len > 0;
	render_strip(md, ray, txtr_cord, screen_pos);
	if (md->cam.closest_x == ray->index)
		update_wall_pointed(md, hit, ray->wall_strip_pos);
	return (1);
}
