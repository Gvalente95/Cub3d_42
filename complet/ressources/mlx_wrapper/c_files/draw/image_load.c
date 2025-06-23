/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_load.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 23:00:24 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 09:51:04 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"
#include "../../../cube.h"

char	**get_frames(t_md *md, char *dir, int *amount, int max)
{
	char	**frames;
	char	*path;
	char	*num_str;

	if (!dir || !amount)
		return (NULL);
	num_str = ft_strdup("0");
	frames = md_malloc(md, sizeof(char *) * (max + 1));
	*amount = 0;
	path = ft_megajoin(IMG_PATH, dir, num_str, ".xpm");
	if (access(path, F_OK) == -1)
		return (printf("\"%s\" path not found\n", path), free(path), NULL);
	while (access(path, F_OK) != -1 && *amount < max)
	{
		free(path);
		frames[*amount] = ft_megajoin(dir, num_str, ".xpm", NULL);
		frames[*amount + 1] = NULL;
		setstr(&num_str, ft_itoa(*amount));
		*amount += 1;
		path = ft_megajoin(IMG_PATH, dir, num_str, ".xpm");
		if (!path)
			return (free(num_str), free_void_array((void **)frames), NULL);
	}
	return (free(num_str), free(path), frames[*amount] = NULL, frames);
}

//	data.x = ratio | data.y = add IMG_PATH
void	*ld_txtr(t_md *md, t_vec2 *final_size, char *path, t_vec2 data)
{
	t_vec2	txt_size;
	void	*texture;
	char	*full_path;

	if (data.y)
		full_path = md_strjoin(md, IMG_PATH, path);
	else
		full_path = md_strdup(md, path);
	if (!full_path)
		return (printf("\"%s\" alloc failed\n", path), NULL);
	if (access(full_path, F_OK) == -1)
		return (printf("\"%s\" file not found in ld_txtr\n", full_path), NULL);
	texture = md->mlx_make(md->mlx, full_path, &txt_size.x, &txt_size.y);
	free(full_path);
	if (!texture)
		return (printf("\"%s\" can't make txtr\n", path), NULL);
	if (final_size->x <= 0 || final_size->y <= 0)
		*final_size = v2(txt_size.x, txt_size.y);
	else if (data.x)
		texture = scale_img_keep_ratio(md, texture, &txt_size, *final_size);
	else
		texture = resize_img(md, texture, &txt_size, *final_size);
	if (!texture)
		return (printf("\"%s\" can't scale txtr\n", path), NULL);
	return (texture);
}

void	**ld_txtrs(t_md *md, t_vec2 final_size, char *dirpath, int keep_ratio)
{
	t_vec2	size;
	void	**txtr;
	char	*fulldir;
	char	**frms;
	int		i;

	fulldir = md_strjoin(md, IMG_PATH, dirpath);
	frms = get_frames(md, dirpath, &i, ENT_FRAMES_MAX);
	if (!frms)
		return (printf("\"%s\" frames NULL\n", fulldir), free(fulldir), NULL);
	free(fulldir);
	txtr = md_malloc(md, sizeof(void *) * (i + 1));
	i = 0;
	while (frms[i])
	{
		txtr[i] = md->mlx_make(md->mlx, frms[i], &size.x, &size.y);
		if (keep_ratio)
			txtr[i] = scale_img_keep_ratio(md, txtr[i], &size, final_size);
		else
			txtr[i] = resize_img(md, txtr[i], &size, final_size);
		free(frms[i++]);
	}
	return (free(frms), txtr[i] = NULL, txtr);
}

void	copy_image_src(t_md *md, t_image *new_img, t_image *src_img)
{
	int		src_index;
	int		dst_index;
	t_vec2	pos;

	pos = v2(-1, -1);
	while (++ pos.y < src_img->size.y)
	{
		pos.x = -1;
		while (++pos.x < src_img->size.x)
		{
			src_index = pos.y * (src_img->size_line / 4) + pos.x;
			dst_index = pos.y * (new_img->size_line / 4) + pos.x;
			new_img->src[dst_index] = src_img->src[src_index];
		}
	}
}

t_image	*copy_image(t_md *md, t_image *src_img, t_vec2 size, int color)
{
	t_image	*new_img;

	if (!md || !src_img || !src_img->src)
		return (NULL);
	new_img = malloc(sizeof(t_image));
	if (!new_img)
		return (NULL);
	new_img->img = mlx_new_image(md->mlx, src_img->size.x, src_img->size.y);
	if (!new_img->img)
		return (free(new_img), NULL);
	new_img->src = (int *)mlx_get_data_addr(new_img->img, &new_img->bpp, \
		&new_img->size_line, &new_img->endian);
	if (!new_img->src)
		return (mlx_destroy_image(md->mlx, new_img->img), free(new_img), NULL);
	copy_image_src(md, new_img, src_img);
	new_img->size = src_img->size;
	new_img->bpp = src_img->bpp;
	new_img->size_line = src_img->size_line;
	new_img->endian = src_img->endian;
	ft_strlcpy(new_img->path, src_img->path, 10);
	if (size.x > 0 && size.y > 0 && !cmp_vec2(new_img->size, size))
		scale_imgd(md, new_img, size, 1);
	if (color != -1)
		flush_img(new_img, color, 0.5, 1);
	return (new_img);
}
