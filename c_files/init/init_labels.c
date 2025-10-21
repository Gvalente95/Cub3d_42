/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_labels.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:41:10 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/16 11:36:36 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	init_action_labels(t_texture_data *td)
{
	td->ents_act_names[m_idle] = "IDLE";
	td->ents_act_names[m_walk] = "WALK";
	td->ents_act_names[m_atk] = "ATTACK";
	td->ents_act_names[m_death] = "DEATH";
	td->plr_act_names[plr_walk] = "WALK";
	td->plr_act_names[plr_attack] = "ATTACK";
	td->plr_act_names[plr_hurt] = "HURT";
	td->plr_act_names[plr_death] = "DEATH";
	td->plr_act_names[plr_idle] = "idle";
}

static void	init_weapon_labels(t_texture_data *td)
{
	td->weapons_names[Knife] = "Knife";
	td->weapons_names[Pistol] = "Pistol";
	td->weapons_names[Shotgun] = "Shotgun";
	td->weapons_names[Machine_Gun] = "Machine gun";
	td->weapons_names[Flame_Thrower] = "Flame Thrower";
	td->weapons_names[Rocket] = "Rocket";
}

static void	init_ents_labels(t_texture_data *td)
{
	td->ents_types_names[nt_wall] = "Wall";
	td->ents_types_names[nt_ext_wall] = "ext_wall";
	td->ents_types_names[nt_interior] = "interior";
	td->ents_types_names[nt_empty] = "Grass";
	td->ents_types_names[nt_plr] = "Player";
	td->ents_types_names[nt_mob] = "Mob";
	td->ents_types_names[nt_door] = "Door";
	td->ents_types_names[nt_item] = "item";
	td->ents_types_names[nt_bush] = "Bush";
	td->ents_types_names[nt_tree] = "Tree";
	td->ents_types_names[nt_pokemon] = "Pokemon";
	td->item_names[Keys] = "Keys";
	td->item_names[HM] = "HM";
	td->item_names[Health] = "Super Potion";
	td->item_names[Weapon] = "Weapn";
	td->item_names[Pokeball] = "Great Ball";
	td->mob_names[Rat] = "Rat";
	td->mob_names[Guard] = "Guard";
	td->mob_names[Elite_Guard] = "Elite Guard";
	td->mob_names[Officer] = "Officer";
	td->mob_names[Death_Knight] = "Death Knight";
	td->mob_names[Mecha_Meister] = "Mecha Meister";
}

void	init_labels(t_texture_data *txd)
{
	txd->ents_tp_map[0] = "12IG*MDPBTK0";
	init_weapon_labels(txd);
	init_ents_labels(txd);
	init_action_labels(txd);
}
