/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filters_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:52:06 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 09:51:32 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	apply_barrel_fx(t_image *img, t_vec2 sz, t_vec2 c, float str)
{
	t_vec2			p;
	t_vec2			n;
	t_vec2f			m;
	t_vec3f			rad;
	uint32_t		*original;

	rad = (t_vec3f){0, str * .3f, 1.0f / (1.0f + (str * .3f) * sqrtf(2.0f))};
	original = malloc(sizeof(uint32_t) * sz.x * sz.y);
	memcpy(original, img->src, sizeof(uint32_t) * sz.x * sz.y);
	p.y = -1;
	while (++p.y < sz.y)
	{
		p.x = -1;
		while (++p.x < sz.x)
		{
			m = (t_vec2f){(p.x - c.x) / (float)c.x, (p.y - c.y) / (float)c.y};
			rad.x = sqrtf(m.x * m.x + m.y * m.y);
			n.x = c.x + m.x * rad.z * (1.0f + rad.y * rad.x) * c.x;
			n.y = c.y + m.y * rad.z * (1.0f + rad.y * rad.x) * c.y;
			if (n.x >= 0 && n.x < sz.x && n.y >= 0 && n.y < sz.y)
				img->src[p.y * sz.x + p.x] = \
					original[(int)n.y * sz.x + (int)n.x];
		}
	}
	free(original);
}

void	apply_color_banding(t_image *img, float intensity)
{
	t_vec2			pos;
	unsigned int	pixel;
	unsigned char	rgba[4];
	int				levels;
	int				factor;

	intensity = 1 - intensity;
	levels = 2 + (int)(intensity * 14.0f);
	factor = 255 / levels;
	pos.y = -1;
	while (++pos.y < img->size.y)
	{
		pos.x = -1;
		while (++pos.x < img->size.x)
		{
			pixel = img->src[pos.y * img->size.x + pos.x];
			rgba[0] = ((pixel >> 16) & 0xFF) / factor * factor;
			rgba[1] = ((pixel >> 8) & 0xFF) / factor * factor;
			rgba[2] = (pixel & 0xFF) / factor * factor;
			rgba[3] = (pixel >> 24) & 0xFF;
			img->src[pos.y * img->size.x + pos.x] = \
				(rgba[3] << 24) | (rgba[0] << 16) | (rgba[1] << 8) | rgba[2];
		}
	}
}

void	apply_bloom(t_image *img, float intensity)
{
	t_vec3			pos;
	unsigned int	blrx[2];
	unsigned char	rgba[4];

	pos = v3(0, -1, 150 - (int)(intensity * 100.0f));
	while (++pos.y < img->size.y - 1)
	{
		pos.x = -1;
		while (++pos.x < img->size.x - 1)
		{
			blrx[1] = img->src[pos.y * img->size.x + pos.x];
			rgba[0] = (blrx[1] >> 16) & 0xFF;
			rgba[1] = (blrx[1] >> 8) & 0xFF;
			rgba[2] = blrx[1] & 0xFF;
			rgba[3] = (blrx[1] >> 24) & 0xFF;
			if (rgba[0] + rgba[1] + rgba[2] <= pos.z)
				continue ;
			blrx[0] = img->src[(pos.y + 1) * img->size.x + pos.x] >> 1;
			blrx[0] += img->src[(pos.y - 1) * img->size.x + pos.x] >> 1;
			blrx[0] += img->src[pos.y * img->size.x + pos.x + 1] >> 1;
			blrx[0] += img->src[pos.y * img->size.x + pos.x - 1] >> 1;
			img->src[pos.y * img->size.x + pos.x] = \
				(rgba[3] << 24) | (blrx[0] & 0xFFFFFF);
		}
	}
}

void	apply_vignette(t_image *img, float intensity, int color)
{
	t_vec2	pos;
	int		txtr_color;
	float	distance;
	float	max_distance;
	float	alpha;

	max_distance = sqrt(pow(img->size.x / 2.0f, 2) + \
		pow(img->size.y / 2.0f, 2));
	pos = v2(-1, -1);
	while (++pos.y < img->size.y)
	{
		pos.x = -1;
		while (++pos.x < img->size.x)
		{
			txtr_color = img->src[pos.y * img->size.x + pos.x];
			distance = sqrt(pow(pos.x - img->size.x / 2.0f, 2) + \
				pow(pos.y - img->size.y / 2.0f, 2));
			alpha = 1.0f - (distance / max_distance);
			alpha = fmax(0.0f, alpha);
			alpha *= intensity;
			img->src[pos.y * img->size.x + pos.x] = \
				blend_color(txtr_color, color, alpha);
		}
	}
}
