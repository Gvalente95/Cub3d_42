/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_pixels_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 15:25:37 by gvalente          #+#    #+#             */
/*   Updated: 2025/06/01 15:25:45 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

int	is_inside_smooth_rect(t_vec2 p, t_vec2 pos, t_vec2 limit)
{
	const float	radius = 32.0f;
	t_vec2		c;
	t_vec2		d;
	float		dist;

	if (p.x < pos.x + radius && p.y < pos.y + radius)
		c = (t_vec2){pos.x + radius, pos.y + radius};
	else if (p.x >= limit.x - radius && p.y < pos.y + radius)
		c = (t_vec2){limit.x - radius, pos.y + radius};
	else if (p.x < pos.x + radius && p.y >= limit.y - radius)
		c = (t_vec2){pos.x + radius, limit.y - radius};
	else if (p.x >= limit.x - radius && p.y >= limit.y - radius)
		c = (t_vec2){limit.x - radius, limit.y - radius};
	else
		return (1);
	d.x = p.x - c.x;
	d.y = p.y - c.y;
	dist = sqrtf(d.x * d.x + d.y * d.y);
	return (dist <= radius);
}

int	draw_smooth_rec(t_image *dst, t_vec2 pos, t_vec2 draw_size, int color)
{
	int		idx;
	int		line_size;
	t_vec2	p;
	t_vec2	limit;
	float	alpha;

	if (pos.x + draw_size.x < 0 || pos.y + draw_size.y < 0)
		return (0);
	line_size = dst->size_line / 4;
	alpha = maxf(0.2f, 1.0f - ((float)((color >> 24) & 0xFF) / 255.0f));
	limit.x = min(dst->size.x, pos.x + draw_size.x);
	limit.y = min(dst->size.y, pos.y + draw_size.y);
	p.y = pos.y - 1;
	while (++p.y < limit.y)
	{
		p.x = pos.x - 1;
		while (++p.x < limit.x)
		{
			idx = p.y * line_size + p.x;
			if (is_inside_smooth_rect(p, pos, limit))
				dst->src[idx] = blend_color(dst->src[idx], color, alpha);
		}
	}
}
