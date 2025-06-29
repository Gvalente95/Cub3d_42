/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_image.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:46:54 by gvalente          #+#    #+#             */
/*   Updated: 2025/06/28 15:26:38 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

int	free_image_data(t_md *md, t_image *img_data)
{
	int	res;

	if (!img_data)
		return (0);
	res = 0;
	if (img_data->img)
		res += mlx_destroy_image(md->mlx, img_data->img);
	free(img_data);
	return (res + 1);
}

int	free_images_data(t_md *md, t_image **images, const char *label)
{
	int	i;
	int	free_count;

	if (!images)
	{
		if (label)
			printf("tried to free \"%s\" img_datas\n", label);
		return (0);
	}
	free_count = 0;
	i = -1;
	while (images[++i])
		free_count += free_image_data(md, images[i]);
	free(images);
	return (free_count + 1);
}

int	free_images_array(t_md *md, t_image ***arr, const char *label)
{
	int	i;
	int	fa;

	fa = 0;
	if (!arr)
		return (fa);
	i = -1;
	while (arr[++i])
		fa += free_images_data(md, arr[i], label);
	free(arr);
	return (fa);
}

int	free_mob_images(t_md *md, t_ent *e, char *label)
{
	int	action;
	int	fa;

	fa = 0;
	if (!e->anim)
		return (fa);
	action = -1;
	while (++action < ENT_ACTION_LEN)
	{
		if (!e->anim[action])
			break ;
		fa += free_images_data(md, e->anim[action], "label");
	}
	fa += safe_free(e->anim);
	return (fa);
}

int	free_menu(t_md *md, t_menu *menu)
{
	int	i;
	int	fa;

	fa = 0;
	fa += free_image_data(md, menu->overlay);
	fa += free_image_data(md, menu->bgr_overlay);
	fa += free_image_data(md, menu->ui_overlay);
	i = -1;
	while (++i < 3)
		fa += free_image_data(md, menu->clrp[i].img);
	printf("MENU %-5s%d%s\n", PGREEN, fa, PRESET);
	return (fa);
}
