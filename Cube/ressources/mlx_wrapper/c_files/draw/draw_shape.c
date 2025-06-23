/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_shape.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 22:04:37 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/23 18:10:07 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

void	draw_trimmed(t_image *src, t_image *dst, t_vec2 pos, t_vec3 draw_end)
{
	t_draw_d	draw_d;

	if (!src || !dst || !src->src || !dst->src)
		return ;
	draw_d.src = src;
	draw_d.dst = dst;
	draw_d.pos = v2(-1, -1);
	while (++draw_d.pos.y < src->size.y)
	{
		draw_d.src_pos.y = draw_d.pos.y;
		draw_d.dst_pos.y = pos.y + draw_d.pos.y;
		if (draw_d.dst_pos.y >= draw_end.y)
			break ;
		if (draw_d.dst_pos.y < 0 || draw_d.dst_pos.y >= dst->size.y)
			continue ;
		draw_d.pos.x = -1;
		while (++draw_d.pos.x < src->size.x)
		{
			draw_d.src_pos.x = draw_d.pos.x;
			draw_d.dst_pos.x = pos.x + draw_d.pos.x;
			put_pxl_if_vis(&draw_d, -1, 0, 0);
		}
	}
}

// data = x = color | y = opacity | z = perfectly round toggle (bool)
void	draw_sphere(t_image *dst, t_vec2 pos, t_vec2 sz, t_vec3 data)
{
	t_draw_d			dd;
	const t_vec2		end_crd = (t_vec2){pos.x + sz.x, pos.y + sz.y};
	const t_vec3f		r = (t_vec3f){sz.x / 2.0f, sz.y / 2.0f, 0};
	const t_vec3f		cnt = (t_vec3f){pos.x + r.x, pos.y + r.y, 0};
	t_vec3f				rdp;

	if (!dst || !dst->src || pos.x < 0 || pos.y < 0)
		return ;
	dd = (t_draw_d){NULL, dst, v2(0, 0)};
	dd.pos.y = pos.y - 1;
	while (++dd.pos.y <= end_crd.y)
	{
		dd.pos.x = pos.x - 1;
		while (++dd.pos.x <= end_crd.x)
		{
			rdp.x = (dd.pos.x - cnt.x) / r.x;
			rdp.y = (dd.pos.y - cnt.y) / r.y;
			rdp.z = 1.0f - sqrt(rdp.x * rdp.x + rdp.y * rdp.y);
			if (!(rdp.z > 0 && (data.z || r_range(0, 20) < rdp.z * 50)))
				continue ;
			dd.dst_pos = dd.pos;
			dd.src_pos.x = data.x;
			put_pxl_if_vis(&dd, -1, 1, rdp.z * (data.y / 10.0f));
		}
	}
}

void	draw_rotated_pixels(t_image *nw, t_image *sr, \
	t_image *onto, t_vec4f ctx)
{
	t_vec4		cn;
	t_draw_d	d;
	t_vec4		p;
	t_vec2		final_pos;

	cn = v4(sr->size.x / 2, sr->size.y / 2, nw->size.x / 2, nw->size.y / 2);
	d.dst = nw;
	d.src = sr;
	p.y = -1;
	while (++p.y < nw->size.y)
	{
		p.x = -1;
		while (++p.x < nw->size.x)
		{
			p.z = (p.x - cn.b) * ctx.g - (p.y - cn.a) * ctx.b + cn.r;
			p.w = (p.x - cn.b) * ctx.b + (p.y - cn.a) * ctx.g + cn.g;
			d.dst_pos = v2(p.x, p.y);
			d.src_pos = v2((int)p.z, (int)p.w);
			if (p.z < 0 || p.z >= sr->size.x || p.w < 0 || p.w >= sr->size.y)
				continue ;
			put_pxl_if_vis(&d, -1, 0, 0);
		}
	}
	final_pos = v2(nw->pos.x - (cn.b - cn.r), nw->pos.y - (cn.a - cn.g));
	draw_img(nw, onto, final_pos, -1);
}

//		pos.z = Angle
void	draw_rotated(t_md *md, t_image *from, t_image *onto, t_vec3f pos)
{
	t_image	*rotated;
	t_vec2	new_size;
	t_vec4f	ctx;

	if (!md || !from || !onto || !from->src || !onto->src)
		return ;
	ctx.r = pos.z * (_PI / 180.0f);
	ctx.g = cosf(ctx.r);
	ctx.b = sinf(ctx.r);
	ctx.a = 0;
	new_size.x = fabsf(from->size.x * ctx.g) + fabsf(from->size.y * ctx.b);
	new_size.y = fabsf(from->size.x * ctx.b) + fabsf(from->size.y * ctx.g);
	rotated = init_img(md, new_size, NULL, 0xFF000000);
	if (!rotated)
	{
		printf("rotated img alloc failed\n");
		return ;
	}
	rotated->pos = v2(pos.x, pos.y);
	draw_rotated_pixels(rotated, from, onto, ctx);
	free_image_data(md, rotated);
}

void	draw_contour(t_image *img, int color, int thickness, float transparency)
{
	t_vec2	lines[4][2];
	int		half_thck;
	int		i;

	color = set_alpha(color, transparency);
	half_thck = thickness / 2;
	if (half_thck < 1)
		half_thck = 1;
	lines[0][0] = _v2(0);
	lines[0][1] = v2(img->size.x - half_thck, 0);
	lines[1][0] = _v2(0);
	lines[1][1] = v2(0, img->size.y - half_thck);
	lines[2][0] = v2(img->size.x - thickness, 0);
	lines[2][1] = v2(img->size.x - thickness, img->size.y);
	lines[3][0] = v2(0, img->size.y - thickness);
	lines[3][1] = v2(img->size.x - half_thck, img->size.y - thickness);
	i = -1;
	while (++i < 4)
		draw_line(img, lines[i][0], lines[i][1], v2(color, thickness));
}
