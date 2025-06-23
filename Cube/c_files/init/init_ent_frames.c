/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ent_frames.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 21:42:52 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/01 14:18:01 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static t_image	**copy_action_frames(t_md *md, t_image **base_images)
{
	int		j;
	int		frame_count;
	t_image	**action_images;

	if (!base_images)
		return (NULL);
	frame_count = 0;
	while (base_images[frame_count])
		frame_count++;
	action_images = malloc(sizeof(t_image *) * (frame_count + 1));
	if (!action_images)
		return (NULL);
	j = -1;
	while (++j < frame_count)
		action_images[j] = copy_image(md, base_images[j], v2(-1, -1), -1);
	action_images[j] = NULL;
	return (action_images);
}

static int	copy_anim_frames(t_md *md, t_ent *e)
{
	t_image		***ent_images;
	t_image		***base_images;
	int			i;

	e->mob_type = r_range_seed(&md->r_seed, 0, MOB_TYPE_LEN - 1);
	base_images = md->txd.mobs_txtrs[e->mob_type];
	ent_images = malloc(sizeof(t_image **) * (ENT_ACTION_LEN + 1));
	if (!ent_images)
		return (printf("Error alloc of image in copy anim\n"), 0);
	i = -1;
	while (++i < ENT_ACTION_LEN)
	{
		ent_images[i] = copy_action_frames(md, base_images[i]);
		if (ent_images[i])
			continue ;
		while (--i >= 0)
			free(ent_images[i]);
		free(ent_images);
		return (printf("Error alloc of image in copy anim\n"), 0);
	}
	ent_images[i] = NULL;
	e->anim = ent_images;
	e->frame = e->anim[0][0];
	return (1);
}

void	set_item_frame(t_md *md, t_texture_data *txd, t_ent *e)
{
	e->pckp_type = r_range_seed(&md->r_seed, 0, PCKP_TYPE_LEN - 1);
	e->wpn_type = r_range(0, 3);
	e->frame = txd->item_txtr[e->pckp_type][0];
	if (e->pckp_type == Weapon)
		e->frame = txd->item_txtr[4][e->wpn_type];
}

void	set_ent_label(t_texture_data *txd, t_ent *e, t_ent_type type)
{
	e->label = NULL;
	if (type == nt_mob)
		e->label = txd->mob_names[e->mob_type];
	else if (type == nt_door)
		e->label = "DOOR";
	else if (type == nt_pokemon)
		e->label = txd->pkmn_names[e->mob_type];
	else if (type == nt_item)
		e->label = txd->item_names[e->pckp_type];
}

void	init_ent_frames(t_md *md, t_texture_data *txd, t_ent *e)
{
	e->pckp_type = -1;
	e->action = 0;
	e->frame_index = 0;
	if (e->type == nt_empty)
		e->frame = md->hud.floor;
	else if (e->type == nt_wall)
		e->frame = txd->wall_img[0];
	else if (e->type == nt_mob)
		copy_anim_frames(md, e);
	else if (e->type == nt_door)
		e->frame = txd->door_txtr;
	else if (e->type == nt_item)
		set_item_frame(md, txd, e);
	else if (e->type == nt_ext_wall)
		e->frame = txd->ext_wall;
	else if (e->type == nt_grass)
		e->frame = txd->grass_tile;
	else
	{
		e->mob_type = r_range_seed(&md->r_seed, 0, PKMN_TYPE_LEN - 1);
		e->frames = copy_action_frames(md, md->txd.pkmn[e->mob_type]);
		e->frame = e->frames[0];
	}
	set_ent_label(txd, e, e->type);
}
