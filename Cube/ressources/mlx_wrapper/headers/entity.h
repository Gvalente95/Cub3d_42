/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:29:41 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/01 08:58:40 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENTITY_H
# define ENTITY_H

# include "../mlx_utils.h"
# include "../../lists/lists.h"

# define PRT_AMOUNT		500
# define PART_BASE_SIZE	35

typedef struct s_image_data	t_image;
typedef struct s_entity		t_ent;

typedef enum e_world_direction
{
	NORTH,
	SOUTH,
	EAST,
	WEST
}	t_wrd_dir;

typedef enum e_dir
{
	up,
	left,
	down,
	right,
	none,
	all,
	DIR_LEN
}	t_dir;

typedef enum e_weapon_types
{
	Knife,
	Pistol,
	Shotgun,
	Machine_Gun,
	Flame_Thrower,
	Rocket,
	WEAPON_TYPE_LEN
}	t_weapon_types;

typedef enum e_pokemon_types
{
	Arbok,
	Blastoise,
	Butterfree,
	Chansey,
	Charizard,
	Dodrio,
	Dragonite,
	Dugtrio,
	Exeggutor,
	Gengar,
	Golem,
	Magneton,
	Ninetales,
	Persian,
	Pidgeot,
	Pikachu,
	Poliwrath,
	Snorlax,
	Taurus,
	PKMN_TYPE_LEN
}	t_pokemon_types;

typedef enum e_mob_types
{
	Rat,
	Guard,
	Elite_Guard,
	Officer,
	Death_Knight,
	Mecha_Meister,
	MOB_TYPE_LEN
}	t_mob_types;

typedef enum e_pckp_types
{
	Pokeball,
	Health,
	Keys,
	HM,
	Weapon,
	PCKP_TYPE_LEN
}	t_pckp_types;

typedef enum e_entity_types
{
	nt_wall,
	nt_ext_wall,
	nt_interior,
	nt_grass,
	nt_plr,
	nt_mob,
	nt_door,
	nt_item,
	nt_bush,
	nt_tree,
	nt_pokemon,
	nt_empty,
	ENT_TYPE_LEN
}	t_ent_type;

typedef enum e_player_action
{
	plr_idle,
	plr_walk,
	plr_hurt,
	plr_attack,
	plr_death,
	PLR_ACTION_LEN
}	t_player_action;

typedef enum e_entity_action
{
	m_idle,
	m_walk,
	m_atk,
	m_death,
	ENT_ACTION_LEN
}	t_ent_action;

typedef struct s_entity
{
	t_ent			*col_hit;
	t_ent_type		type;
	t_mob_types		mob_type;
	t_pckp_types	pckp_type;
	t_weapon_types	wpn_type;
	t_ent_action	action;
	t_ent			**pk_team;
	t_vec3f			dir;
	t_vec3f			mov;
	t_vec3f			pos;
	t_vec2			screen_p;
	t_vec2			screen_sz;
	t_vec3f			start_pos;
	t_vec3			coord;
	t_vec3f			target_pos;
	t_vec2			size;
	t_image			*frame;
	t_image			**frames;
	t_image			***anim;
	t_image			*overlay;
	t_wrd_dir		overlay_dir;
	const char		*label;
	double			shot_timer;
	float			angle;
	char			character;
	float			hit_dist;
	t_vec2			crp_pxl;
	int				team_sz;
	int				ray_hit_index;
	int				caught;
	int				was_hit;
	int				shot;
	int				grounded;
	int				map_index;
	int				is_active;
	int				hp;
	int				max_hp;
	int				frame_index;
	int				in_screen;
	int				revealed;
	float			cam_distance;
}	t_ent;

#endif