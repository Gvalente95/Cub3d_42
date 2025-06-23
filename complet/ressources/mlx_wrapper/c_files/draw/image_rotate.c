/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_rotate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 20:10:50 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/23 18:10:07 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

static inline float	deg_to_rad(float angle_deg)
{
	return (angle_deg * (_PI / 180.0f));
}

void	draw_img_yaw(t_md *md, t_image *from, t_image *onto, t_vec3f pos)
{
	const float		scale_x = fmaxf(0.05f, cosf(deg_to_rad(pos.z)));
	const t_vec2	cp = (t_vec2){from->size.x / 2, from->size.y / 2};
	t_vec2			dpos;
	int				color;
	t_vec2			draw_p;

	dpos.y = -1;
	while (++dpos.y < from->size.y)
	{
		dpos.x = -1;
		while (++dpos.x < from->size.x)
		{
			color = from->src[dpos.y * from->size.x + dpos.x];
			draw_p.x = pos.x + (int)((dpos.x - cp.x) * scale_x);
			draw_p.y = pos.y + dpos.y - cp.y + (int)((dpos.x - cp.x) * \
			sinf(deg_to_rad(pos.z)) * 0.5f);
			draw_pixel(onto, v2(draw_p.x, draw_p.y), color, -1);
		}
	}
}

void	draw_img_pitch(t_md *md, t_image *from, t_image *onto, t_vec3f pos)
{
	const float		scale_y = fmaxf(0.05f, cosf(deg_to_rad(pos.z)));
	const t_vec2	cp = (t_vec2){from->size.x / 2, from->size.y / 2};
	t_vec2			draw_p;
	t_vec3			p;
	t_vec2			skew;

	p.y = -1;
	while (++p.y < from->size.y)
	{
		skew.x = (int)((p.y - cp.y) * scale_y);
		skew.y = (int)((p.y - cp.y) * sinf(deg_to_rad(pos.z)) * 0.5f);
		p.x = -1;
		while (++p.x < from->size.x)
		{
			p.z = from->src[p.y * from->size.x + p.x];
			draw_p.x = pos.x + p.x - cp.x;
			draw_p.y = pos.y + skew.x + skew.y;
			draw_pixel(onto, v2(draw_p.x, draw_p.y), p.z, -1);
		}
	}
}

void	draw_sprite_roll(t_md *md, t_image *from, t_image *onto, t_vec3f pos)
{
	const float		angle = deg_to_rad(pos.z);
	const t_vec2f	cosin = (t_vec2f){cosf(angle), sinf(angle)};
	const t_vec2	cp = (t_vec2){from->size.x / 2, from->size.y / 2};
	t_vec3			p;
	t_vec2			rot;

	p.y = -1;
	while (++p.y < from->size.y)
	{
		p.x = -1;
		while (++p.x < from->size.x)
		{
			p.z = from->src[p.y * from->size.x + p.x];
			if (((p.z >> 24) & 0xFF) <= 0x10)
				continue ;
			rot.x = (int)(p.x - cp.x * cosin.x - p.y - cp.y * cosin.y);
			rot.y = (int)(p.x - cp.x * cosin.y + p.y - cp.y * cosin.x);
			draw_pixel(onto, v2(pos.x + rot.x, pos.y + rot.y), p.z, -1);
		}
	}
}
