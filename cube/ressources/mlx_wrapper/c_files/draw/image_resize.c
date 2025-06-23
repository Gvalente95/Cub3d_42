/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_resize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:17:46 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 09:51:04 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

t_vec3f	set_new_size(t_image *q, t_vec2 *old_size, t_vec2 *new_size)
{
	double	aspect_ratio;
	t_vec3f	ratio;

	aspect_ratio = (double)old_size->x / old_size->y;
	if ((double)new_size->x / new_size->y > aspect_ratio)
		new_size->x = (int)(new_size->y * aspect_ratio);
	else
		new_size->y = (int)(new_size->x / aspect_ratio);
	ratio.x = (double)old_size->x / new_size->x;
	ratio.y = (double)old_size->y / new_size->y;
	return (ratio);
}

void	*scale_img_keep_ratio(t_md *md, void *img, \
	t_vec2 *old_size, t_vec2 new_size)
{
	t_image	m;
	t_vec3f	ratio;

	if (!img)
		return (printf("Error -> img to scale not find\n"), NULL);
	ratio = set_new_size(&m, old_size, &new_size);
	m.img = mlx_new_image(md->mlx, new_size.x, new_size.y);
	if (!m.img)
		return (NULL);
	m.src = (int *)mlx_get_data_addr(img, &m.bps, &m.size_line, &m.endian);
	m.scl_d = (int *)mlx_get_data_addr(m.img, &m.bps, &m.size_line, &m.endian);
	m.pos.y = -1;
	while (++m.pos.y < new_size.y)
	{
		m.pos.x = -1;
		while (++m.pos.x < new_size.x)
		{
			m.size.x = (int)(m.pos.x * (double)ratio.x);
			m.size.y = (int)(m.pos.y * (double)ratio.y);
			m.scl_d[m.pos.y * new_size.x + m.pos.x] = \
				m.src[m.size.y * old_size->x + m.size.x];
		}
	}
	mlx_destroy_image(md->mlx, img);
	return (img = m.img, *old_size = v2(new_size.x, new_size.y), m.img);
}

void	*resize_img(t_md *md, void *img, t_vec2 *old_size, t_vec2 new_size)
{
	t_image	q;
	t_vec3f	ratio;

	q.img = mlx_new_image(md->mlx, new_size.x, new_size.y);
	if (!q.img)
		return (NULL);
	q.src = (int *)mlx_get_data_addr(img, &q.bps, &q.size_line, &q.endian);
	q.scl_d = (int *)mlx_get_data_addr(q.img, &q.bps, &q.size_line, &q.endian);
	ratio.x = (float)old_size->x / new_size.x;
	ratio.y = (float)old_size->y / new_size.y;
	q.pos.y = -1;
	while (++q.pos.y < new_size.y)
	{
		q.pos.x = -1;
		while (++q.pos.x < new_size.x)
		{
			q.size.x = (int)(q.pos.x * (double)ratio.x);
			q.size.y = (int)(q.pos.y * (double)ratio.y);
			q.scl_d[q.pos.y * new_size.x + q.pos.x] = \
				q.src[q.size.y * old_size->x + q.size.x];
		}
	}
	mlx_destroy_image(md->mlx, img);
	*old_size = v2(new_size.x, new_size.y);
	return (q.img);
}

t_image	*scale_imgd(t_md *md, t_image *imgd, t_vec2 new_size, int keep_ratio)
{
	if (keep_ratio)
		imgd->img = scale_img_keep_ratio(md, imgd->img, &imgd->size, new_size);
	else
		imgd->img = resize_img(md, imgd->img, &imgd->size, new_size);
	imgd->addr = mlx_get_data_addr(imgd->img, &imgd->bpp, \
		&imgd->size_line, &imgd->endian);
	if (!imgd->addr)
		return (printf("ERR: Failed to get image data address\n"), imgd);
	imgd->src = (int *)imgd->addr;
	if (!imgd->src)
		return (printf("ERR: Failed to get src data\n"), imgd);
	return (imgd);
}

int	get_feet_offset(t_image *img)
{
	t_vec2		pos;
	uint32_t	pixel;

	pos = _v2(-1);
	while (++pos.y < img->size.y - 1)
	{
		pos.x = -1;
		while (++pos.x < img->size.x)
		{
			pixel = img->src[pos.y * img->size.x + pos.x];
			if ((pixel >> 24) & 0xFF)
				return (img->size.y - 1 - pos.y);
		}
	}
	return (0);
}
