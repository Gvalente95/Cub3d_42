/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_elements.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 04:32:24 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/01 15:31:29 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

int	free_hud(t_md *md, t_hud *hud)
{
	int	fa;

	fa = 0;
	fa += free_image_data(md, hud->overlay);
	fa += free_image_data(md, hud->rgun);
	fa += free_image_data(md, hud->base_sky);
	fa += free_image_data(md, hud->sky);
	fa += free_image_data(md, hud->sky_flipy);
	fa += free_image_data(md, hud->floor);
	fa += free_image_data(md, hud->lock_x_icon);
	fa += free_image_data(md, hud->lock_y_icon);
	fa += free_image_data(md, hud->amm_icon);
	fa += free_image_data(md, hud->amm2_icon);
	fa += free_image_data(md, hud->key_icon);
	fa += free_image_data(md, hud->key2_icon);
	fa += free_image_data(md, hud->hp_icon);
	fa += free_image_data(md, hud->sky_buffer);
	fa += free_image_data(md, hud->ceiling);
	fa += free_image_data(md, hud->wall);
	fa += free_image_data(md, hud->floor2d);
	fa += free_image_data(md, hud->center);
	printf("hud freed %d\n", fa);
	return (fa);
}

int	free_txd(t_md *md, t_texture_data *txd)
{
	int	fa;
	int	i;

	fa = free_images_data(md, txd->bush_txtr, "bush");
	fa += free_images_data(md, txd->tree_txtr, "tree");
	fa += free_image_data(md, txd->grass_mini);
	fa += free_image_data(md, txd->grass_tile);
	fa += free_image_data(md, txd->ext_wall);
	fa += free_image_data(md, txd->ext_wall_mini);
	fa += free_image_data(md, txd->door_txtr);
	fa += free_image_data(md, txd->door_txtr_mini);
	fa += free_images_data(md, txd->wall_img, "wall_img");
	fa += free_images_data(md, txd->wall_img2d, "wall_img2d");
	fa += free_images_array(md, txd->item_txtr, "item tex");
	fa += free_images_array(md, txd->item_txtr_mini, "minipick tex");
	fa += free_images_array(md, txd->pkmn, "pokemon tex");
	fa += free_images_data(md, txd->pkmns_mini, "pokemon_mini tex");
	i = -1;
	while (txd->mobs_txtrs && txd->mobs_txtrs[++i])
		fa += free_images_array(md, txd->mobs_txtrs[i], "mob tex");
	i = -1;
	while (txd->mobs_txtrs_mini && txd->mobs_txtrs_mini[++i])
		fa += free_images_array(md, txd->mobs_txtrs_mini[i], "minimob tex");
	fa += safe_free(txd->mobs_txtrs_mini);
	return (fa += safe_free(txd->mobs_txtrs), printf("txd freed %d\n", fa), fa);
}

int	free_var(t_md *md, t_mmap *mmap, t_fx_data *fx, t_mouse *mouse)
{
	int	fa;
	int	i;

	fa = 0;
	fa += free_images_array(md, md->txd.wpn_txtr, "wpn tex");
	fa += free_images_array(md, md->txd.wpn_txtr_2d, "wpn2d tex");
	fa += free_image_data(md, fx->vignette);
	fa += free_image_data(md, md->screen);
	fa += free_image_data(md, mmap->bg);
	fa += free_image_data(md, mmap->img);
	fa += free_image_data(md, mmap->full_map);
	fa += free_image_data(md, mouse->cursor);
	fa += free_image_data(md, mouse->curs_dtc);
	fa += free_image_data(md, mouse->curs_grb);
	fa += safe_free(md->map.buffer);
	fa += safe_free(md->out_map);
	i = -1;
	while (++i < 256)
		fa += free_image_data(md, md->txd.font[i]);
	i = -1;
	while (++i < 2)
		if (md->portal.ends[i].e && md->portal.ends[i].e->overlay)
			fa += free_image_data(md, md->portal.ends[i].e->overlay);
	printf("var freed %d\n", fa);
	return (fa);
}

int	free_ent(t_md *md, t_ent *e)
{
	int	fa;

	fa = 0;
	if (e->type == nt_mob)
		fa += free_mob_images(md, e, "mob");
	if (e->pk_team)
	{
		while (e->team_sz)
			fa += free_ent(md, e->pk_team[--e->team_sz]);
		free(e->pk_team);
	}
	if (e->frames)
		fa += free_images_data(md, e->frames, "e frames");
	free(e);
	return (fa + 1);
}

int	free_ents(t_md *md)
{
	int		fa;
	t_dblst	*node;
	t_ent	*e;
	int		i;

	fa = 0;
	node = dblst_first(md->entities);
	if (!node)
		return (fa);
	while (node)
	{
		e = (t_ent *)node->content;
		fa += free_ent(md, e);
		node = node->next;
	}
	fa += dblst_size(md->entities);
	fa += free_images_data(md, md->plr.frames, "plr frames");
	dblst_clear(&md->entities, NULL);
	return (printf("ent freed %d\n", fa + 1), fa);
}
