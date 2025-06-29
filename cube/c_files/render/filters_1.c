/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filters_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:52:06 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/28 17:31:58 by giuliovalen      ###   ########.fr       */
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

void	set_blur(t_image *img, float treshold, const t_vec2 s, uint32_t *b)
{
	t_vec2		p;
	uint32_t	c;
	uint8_t		rgb[3];

	p = _v2(-1);
	while (++p.y < s.y - 1)
	{
		p.x = -1;
		while (++p.x < s.x - 1)
		{
			c = img->src[p.y * s.x + p.x];
			rgb[0] = (c >> 16) & 0xFF;
			rgb[1] = (c >> 8) & 0xFF;
			rgb[2] = c & 0xFF;
			if ((rgb[0] + rgb[1] + rgb[2]) < treshold)
				continue ;
			b[p.y * s.x + p.x] = (uint32_t)(img->src[(p.y + 1) * s.x + p.x] + \
				img->src[(p.y - 1) * s.x + p.x] + \
				img->src[p.y * s.x + p.x + 1] + \
				img->src[p.y * s.x + p.x - 1]) >> 2;
		}
	}
}

void	apply_bloom(t_image *img, float intensity)
{
	uint32_t		*blur;
	const t_vec2	s = img->size;
	int				i;
	uint32_t		sr[2];
	uint8_t			v[9];

	blur = malloc((size_t)s.x * s.y * sizeof(uint32_t));
	set_blur(img, 150.0f - intensity * 100.0f, s, blur);
	i = -1;
	while (++i < s.x * s.y)
	{
		sr[0] = img->src[i];
		sr[1] = blur[i];
		v[0] = (sr[0] >> 16) & 0xFF;
		v[1] = (sr[0] >> 8) & 0xFF;
		v[2] = sr[0] & 0xFF;
		v[3] = (sr[1] >> 16) & 0xFF;
		v[4] = (sr[1] >> 8) & 0xFF;
		v[5] = sr[1] & 0xFF;
		v[6] = (uint8_t)fminf(255.0f, v[0] + v[3] * intensity);
		v[7] = (uint8_t)fminf(255.0f, v[1] + v[4] * intensity);
		v[8] = (uint8_t)fminf(255.0f, v[2] + v[5] * intensity);
		img->src[i] = (sr[0] & 0xFF000000) | (v[6] << 16) | (v[7] << 8) | v[8];
	}
	free(blur);
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
