/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pokemon.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 13:22:40 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/09 20:17:58 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POKEMON_H
#define POKEMON_H

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

typedef enum e_move_type { Physical, Special }	t_move_type;
typedef enum t_elementType {
	eFire, eWater, eGrass,
	eElectricity, eGround,
	eNormal, eIce, eRock, eGhost,
	ePsychic, ePoison, eFlight, eLen,
}	t_elType;

typedef enum e_status_types {
	sSleep, sPoison, sBurn, sFreeze, sShock, sFear, sConfusion, sStatusLen,
} t_sType;

typedef struct s_move_type {
	char name[256];
	int	cc;
	int	ccMax;
	int	damage;
	int recovery;
	t_sType statusInflicted;
	t_move_type type;
	t_elType	elementType;
}	t_move;

typedef struct s_status_data {
	char 		name[10];
	char		abbrev[10];
	int			color;
	int			endProbability;
	int			damagePerTurn;
	int			damageProbability;
	float		damageMult;
	int			missProbability;
	int			active;
	int			turns;
	t_elType	elType;
	t_sType		type;
}	t_status_data;

typedef struct s_pokemon_data {
	t_status_data	statusData[sStatusLen];
	t_move			PokemonMoves[PKMN_TYPE_LEN][4];
	t_move			struggle;
	int				elementColors[eLen];
	int				pokemonTypes[PKMN_TYPE_LEN];
	const char		*pkmn_names[PKMN_TYPE_LEN];
}	t_pokemon_data;

#endif