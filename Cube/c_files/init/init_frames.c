/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_frames.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:57:39 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/01 15:00:32 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	init_weapon_frames(t_md *md, t_texture_data *td)
{
	td->wpn_txtr = malloc(sizeof(t_image **) * (WEAPON_TYPE_LEN + 1));
	td->wpn_txtr_2d = malloc(sizeof(t_image **) * (WEAPON_TYPE_LEN + 1));
	if (!td->wpn_txtr || !td->wpn_txtr_2d)
		return ;
	td->wpn_txtr[Knife] = init_weapon(md, \
		&td->wpn_txtr_2d[Knife], "ent/Weapons/Knife/");
	td->wpn_txtr[Pistol] = init_weapon(md, \
		&td->wpn_txtr_2d[Pistol], "ent/Weapons/Pistol/");
	td->wpn_txtr[Shotgun] = init_weapon(md, \
		&td->wpn_txtr_2d[Shotgun], "ent/Weapons/Shotgun/");
	td->wpn_txtr[Machine_Gun] = init_weapon(md, \
		&td->wpn_txtr_2d[Machine_Gun], "ent/Weapons/Machine gun/");
	td->wpn_txtr[Flame_Thrower] = init_weapon(md, \
		&td->wpn_txtr_2d[Flame_Thrower], "ent/Weapons/Flame Thrower/");
	td->wpn_txtr[Rocket] = init_weapon(md, \
		&td->wpn_txtr_2d[Rocket], "ent/Weapons/Rocket/");
	td->wpn_txtr[WEAPON_TYPE_LEN] = NULL;
	td->wpn_txtr_2d[WEAPON_TYPE_LEN] = NULL;
}

void	init_item_frames(t_md *md, t_texture_data *td)
{
	td->item_txtr = malloc(sizeof(t_image **) * (PCKP_TYPE_LEN + 1));
	td->item_txtr_mini = malloc(sizeof(t_image **) * (PCKP_TYPE_LEN + 1));
	if (!td->item_txtr || !td->item_txtr_mini)
		return ;
	td->item_txtr[Pokeball] = init_mini(md, \
		&td->item_txtr_mini[Pokeball], "ent/item/Ammo/");
	td->item_txtr[Health] = init_mini(md, \
		&td->item_txtr_mini[Health], "ent/item/Health/");
	td->item_txtr[Keys] = init_mini(md, \
		&td->item_txtr_mini[Keys], "ent/item/Keys/");
	td->item_txtr[HM] = init_mini(md, \
		&td->item_txtr_mini[HM], "ent/item/Score/");
	td->item_txtr[Weapon] = init_mini(md, \
		&td->item_txtr_mini[Weapon], "ent/item/Weapons/");
	td->item_txtr[PCKP_TYPE_LEN] = NULL;
	td->item_txtr_mini[PCKP_TYPE_LEN] = NULL;
}

void	store_entities_sizes(t_texture_data *td, int base)
{
	int				i;

	i = -1;
	while (++i < ENT_TYPE_LEN)
		td->e_sizes2d[i] = _v2(td->size_2d);
	td->e_scales[nt_plr] = base * .2;
	td->e_scales[nt_mob] = base * .5;
	td->e_scales[nt_item] = base * .15;
	td->e_scales[nt_door] = base;
	td->e_scales[nt_wall] = base;
	td->e_scales[nt_bush] = base * .5;
	td->e_scales[nt_tree] = base * .5;
	td->e_scales[nt_pokemon] = base * .35;
}

void	init_env_frames(t_md *md, t_texture_data *td)
{
	td->bush_txtr = init_images(md, _v2(-1), "ent/bush/");
	td->tree_txtr = init_images(md, _v2(-1), "ent/trees/");
}

void	init_ents_data(t_md *md, t_texture_data *txd)
{
	init_labels(txd);
	init_au(md, &md->au);
	store_entities_sizes(txd, md->t_len);
	init_weapon_frames(md, txd);
	init_item_frames(md, txd);
	init_mobs_frames(md);
	init_pk_framess(md, txd);
	init_env_frames(md, txd);
	txd->grass_tile = init_img(md, _v2(md->t_len), NULL, _GREEN);
	txd->grass_mini = copy_image(md, txd->grass_tile, _v2(txd->size_2d), -1);
	txd->ext_wall = init_img(md, _v2(md->t_len), "utils/exterior.xpm", -1);
	txd->ext_wall_mini = copy_image(md, txd->ext_wall, _v2(txd->size_2d), -1);
	txd->door_txtr = init_img(md, _v2(md->t_len), "ent/door/0.xpm", -1);
	txd->door_txtr_mini = init_img(md, _v2(txd->size_2d), "ent/door/0.xpm", -1);
	if (!txd->door_txtr || !txd->door_txtr->img)
		exit(0);
}
