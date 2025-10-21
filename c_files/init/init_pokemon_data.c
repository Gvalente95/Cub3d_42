/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pokemon_data.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 23:21:21 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/10 01:42:33 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	initElementColors(t_pokemon_data *pkd) {
	pkd->elementColors[eFire] = _RED;
	pkd->elementColors[eWater] = _BLUE;
	pkd->elementColors[eGrass] = _GREEN;
	pkd->elementColors[eElectricity] = _YELLOW;
	pkd->elementColors[eGround] = _BEIGE;
	pkd->elementColors[eGhost] = _DARKPURPLE;
	pkd->elementColors[eRock] = _BROWN;
	pkd->elementColors[eNormal] = _GREY;
	pkd->elementColors[ePsychic] = _PINK;
	pkd->elementColors[ePoison] = _DARKGREEN;
	pkd->elementColors[eFlight] = _SILVER;
}

void	initStatusField(t_status_data* sd, const char* name, const char* abbr, int color,
	int endProbability, int damagePerTurn, int damageProbability, float damageMult, int missProbability, int type) {
	ft_strlcpy(sd->name, name, 10);
	ft_strlcpy(sd->abbrev, abbr, 3);
	sd->color = color;
	sd->endProbability = endProbability;
	sd->damagePerTurn = damagePerTurn;
	sd->damageProbability = damageProbability;
	sd->damageMult = damageMult;
	sd->missProbability = missProbability;
	sd->elType = type;
}

void	setStatus(t_md *md, t_status_data* status, t_sType type) {
	*status = md->pkd.statusData[type];
	status->active = true;
	status->turns = 0;
}

void	initStatusNames(t_pokemon_data* pkd) {
	pkd->statusData[sShock] = (t_status_data){
		.type = sShock,
		.name = "shocked",
		.abbrev = "el",
		.color = _YELLOW,
		.damageMult = 0,
		.damagePerTurn = 0,
		.damageProbability = 0,
		.endProbability = 30,
		.missProbability = 30,
		.turns = 0,
		.elType = -1,
	};
	pkd->statusData[sFear] = (t_status_data){
		.type = sFear,
		.name = "scared",
		.abbrev = "scr",
		.color = _YELLOW,
		.damageMult = 0,
		.damagePerTurn = 0,
		.damageProbability = 0,
		.endProbability = 100,
		.missProbability = 100,
		.turns = 0,
		.elType = -1,
	};
	pkd->statusData[sPoison] = (t_status_data){
		.type = sPoison,
		.name = "poisoned",
		.abbrev = "tox",
		.color = _DARKPURPLE,
		.damageMult = 1.2,
		.damagePerTurn = 10,
		.damageProbability = 100,
		.endProbability = 30,
		.missProbability = 0,
		.turns = 0,
		.elType = ePoison,
	};
	pkd->statusData[sBurn] = (t_status_data){
		.type = sBurn,
		.name = "burned",
		.abbrev = "brn",
		.color = _RED,
		.damageMult = 0,
		.damagePerTurn = 20,
		.damageProbability = 100,
		.endProbability = 30,
		.missProbability = 0,
		.turns = 0,
		.elType = eFire,
	};
	pkd->statusData[sFreeze] = (t_status_data){
		.type = sFreeze,
		.name = "frozen",
		.abbrev = "frz",
		.color = _WHITE,
		.damageMult = 0,
		.damagePerTurn = 0,
		.damageProbability = 0,
		.endProbability = 20,
		.missProbability = 100,
		.turns = 0,
		.elType = eIce,
	};
	pkd->statusData[sSleep] = (t_status_data){
		.type = sSleep,
		.name = "asleep",
		.abbrev = "zzz",
		.color = _PINK,
		.damageMult = 0,
		.damagePerTurn = 0,
		.damageProbability = 0,
		.endProbability = 20,
		.missProbability = 100,
		.turns = 0,
		.elType = -1,
	};
	pkd->statusData[sConfusion] = (t_status_data){
		.type = sConfusion,
		.name = "confused",
		.abbrev = "?",
		.color = _PINK,
		.damageMult = 0,
		.damagePerTurn = 0,
		.damageProbability = 0,
		.endProbability = 0,
		.missProbability = 30,
		.turns = 0,
		.elType = -1,
	};
}

//		t_move
// 	char name[256];
// 	int	cc;
// 	int	ccMax;
// 	int	damage;
// 	int recovery;
// 	t_sType statusInflicted;
// 	t_move_type type;
// 	t_elType	elementType;
void	init_pokemon_moves(t_pokemon_data* pkd) {
	 pkd->struggle = (t_move){ "struggle", 0, 100, 10, -20, 0, Physical, ePoison };

	int i = 0;
	/* Arbok */
	t_move ArbokMoves[4];
	ArbokMoves[i++] = (t_move){ "Acid", 0, 40, 40, 0, -1, Special, ePoison };
	ArbokMoves[i++] = (t_move){ "Bite", 0, 60, 60, 0, -1, Physical, eNormal };
	ArbokMoves[i++] = (t_move){ "Glare", 0, 20, 10, -1, sShock, Physical, eNormal };
	ArbokMoves[i++] = (t_move){ "Poison Sting", 0, 15, 15, -1, sPoison, Physical, ePoison };

	i = 0;
	/* Blastoise */
	t_move BlastoiseMoves[4];
	BlastoiseMoves[i++] = (t_move){ "Water Gun", 0, 20, 40, 0, -1, Special, eWater };
	BlastoiseMoves[i++] = (t_move){ "Bite", 0, 10, 60, 0, -1, Physical, eNormal };
	BlastoiseMoves[i++] = (t_move){ "Hydro Pump", 5, 0, 110, 0, -1, Special, eWater };
	BlastoiseMoves[i++] = (t_move){ "Skull Bash", 10, 0, 130, 0, -1, Physical, eNormal };

	i = 0;
	/* Butterfree */
	t_move ButterfreeMoves[4];
	ButterfreeMoves[i++] = (t_move){ "Confusion", 0, 30, 50, 0, sConfusion, Special, ePsychic };
	ButterfreeMoves[i++] = (t_move){ "Sleep Powder", 0, 10, 0, 0, sSleep, Special, eGrass };
	ButterfreeMoves[i++] = (t_move){ "Gust", 0, 5, 40, 0, -1, Physical, eNormal };
	ButterfreeMoves[i++] = (t_move){ "Psybeam", 0, 10, 65, 0, -1, Special, ePsychic };

	i = 0;
	/* Chansey */
	t_move ChanseyMoves[4];
	ChanseyMoves[i++] = (t_move){ "Pound", 0, 35, 40, 0, -1, Physical, eNormal };
	ChanseyMoves[i++] = (t_move){ "Sing", 0, 35, 0, 0, sSleep, Special, eNormal };
	ChanseyMoves[i++] = (t_move){ "Egg Bomb", 0, 15, 100, 0, -1, Physical, eNormal };
	ChanseyMoves[i++] = (t_move){ "Soft-Boiled", 0, 10, 0, 50, -1, Special, eNormal };

	i = 0;
	/* Charizard */
	t_move CharizardMoves[4];
	CharizardMoves[i++] = (t_move){ "Flamethrower", 0, 10, 90, 0, sBurn, Special, eFire };
	CharizardMoves[i++] = (t_move){ "Slash", 0, 30, 70, 0, -1, Physical, eNormal };
	CharizardMoves[i++] = (t_move){ "Fly", 0, 10, 90, 0, -1, Physical, eFlight };
	CharizardMoves[i++] = (t_move){ "Fire Spin", 0, 15, 35, 0, sBurn, Special, eFire };

	i = 0;
	/* Dodrio */
	t_move DodrioMoves[4];
	DodrioMoves[i++] = (t_move){ "Drill Peck", 0, 15, 80, 0, -1, Physical, eFlight };
	DodrioMoves[i++] = (t_move){ "Tri Attack", 0, 20, 80, 0, -1, Physical, eNormal };
	DodrioMoves[i++] = (t_move){ "Fury Attack", 0, 20, 15, 0, -1, Physical, eFlight };
	DodrioMoves[i++] = (t_move){ "Agility", 0, 20, 0, 0, -1, Special, eNormal };

	i = 0;
	/* Dragonite */
	t_move DragoniteMoves[4];
	DragoniteMoves[i++] = (t_move){ "Dragon Rage", 0, 20, 40, 0, -1, Special, ePsychic };
	DragoniteMoves[i++] = (t_move){ "Hyper Beam", 0, 20, 150, 0, -1, Physical, eNormal };
	DragoniteMoves[i++] = (t_move){ "Thunder Wave", 0, 20, 0, 0, sShock, Special, eElectricity };
	DragoniteMoves[i++] = (t_move){ "Wing Attack", 0, 20, 60, 0, -1, Physical, eFlight };

	i = 0;
	/* Dugtrio */
	t_move DugtrioMoves[4];
	DugtrioMoves[i++] = (t_move){ "Dig", 0, 20, 80, 0, -1, Physical, eGround };
	DugtrioMoves[i++] = (t_move){ "Earthquake", 0, 20, 100, 0, -1, Physical, eGround };
	DugtrioMoves[i++] = (t_move){ "Slash", 0, 20, 70, 0, -1, Physical, eNormal };
	DugtrioMoves[i++] = (t_move){ "Sand Attack", 0, 20, 0, 0, -1, Special, eGround };

	i = 0;
	/* Exeggutor */
	t_move ExeggutorMoves[4];
	ExeggutorMoves[i++] = (t_move){ "Psychic", 0, 20, 90, 0, sConfusion, Special, ePsychic };
	ExeggutorMoves[i++] = (t_move){ "Stun Spore", 0, 20, 0, 0, sShock, Special, eGrass };
	ExeggutorMoves[i++] = (t_move){ "Egg Bomb", 0, 20, 100, 0, -1, Physical, eNormal };
	ExeggutorMoves[i++] = (t_move){ "Solar Beam", 0, 20, 120, 0, -1, Special, eGrass };

	i = 0;
	/* Gengar */
	t_move GengarMoves[4];
	GengarMoves[i++] = (t_move){ "Night Shade", 0, 20, 50, 0, -1, Special, eGhost };
	GengarMoves[i++] = (t_move){ "Hypnosis", 0, 20, 0, 0, sSleep, Special, ePsychic };
	GengarMoves[i++] = (t_move){ "Dream Eater", 0, 20, 100, 50, -1, Special, eGhost };
	GengarMoves[i++] = (t_move){ "Thunderbolt", 0, 20, 90, 0, sShock, Special, eElectricity };

	i = 0;
	/* Golem */
	t_move GolemMoves[4];
	GolemMoves[i++] = (t_move){ "Rock Throw", 0, 20, 50, 0, -1, Physical, eRock };
	GolemMoves[i++] = (t_move){ "Earthquake", 0, 20, 100, 0, -1, Physical, eGround };
	GolemMoves[i++] = (t_move){ "Explosion", 0, 20, 250, -100, -1, Physical, eNormal };
	GolemMoves[i++] = (t_move){ "Defense Curl", 0, 20, 0, 0, -1, Special, eNormal };

	i = 0;
	/* Magneton */
	t_move MagnetonMoves[4];
	MagnetonMoves[i++] = (t_move){ "Thunderbolt", 0, 20, 90, 0, sShock, Special, eElectricity };
	MagnetonMoves[i++] = (t_move){ "Supersonic", 0, 20, 0, 0, sConfusion, Special, eNormal };
	MagnetonMoves[i++] = (t_move){ "Sonic Boom", 0, 20, 20, 0, -1, Special, eNormal };
	MagnetonMoves[i++] = (t_move){ "Thunder Wave", 0, 20, 0, 0, sShock, Special, eElectricity };

	i = 0;
	/* Ninetales */
	t_move NinetalesMoves[4];
	NinetalesMoves[i++] = (t_move){ "Flamethrower", 0, 20, 90, 0, sBurn, Special, eFire };
	NinetalesMoves[i++] = (t_move){ "Quick Attack", 0, 20, 40, 0, -1, Physical, eNormal };
	NinetalesMoves[i++] = (t_move){ "Confuse Ray", 0, 20, 0, 0, sConfusion, Special, ePsychic };
	NinetalesMoves[i++] = (t_move){ "Fire Spin", 0, 20, 35, 0, sBurn, Special, eFire };

	i = 0;
	/* Persian */
	t_move PersianMoves[4];
	PersianMoves[i++] = (t_move){ "Slash", 0, 20, 70, 0, -1, Physical, eNormal };
	PersianMoves[i++] = (t_move){ "Bite", 0, 20, 60, 0, -1, Physical, eNormal };
	PersianMoves[i++] = (t_move){ "Screech", 0, 20, 0, 0, -1, Special, eNormal };
	PersianMoves[i++] = (t_move){ "Pay Day", 0, 20, 40, 0, -1, Physical, eNormal };

	i = 0;
	/* Pidgeot */
	t_move PidgeotMoves[4];
	PidgeotMoves[i++] = (t_move){ "Gust", 0, 20, 40, 0, -1, Physical, eFlight };
	PidgeotMoves[i++] = (t_move){ "Quick Attack", 0, 20, 40, 0, -1, Physical, eNormal };
	PidgeotMoves[i++] = (t_move){ "Wing Attack", 0, 20, 60, 0, -1, Physical, eFlight };
	PidgeotMoves[i++] = (t_move){ "Sky Attack", 0, 20, 140, 0, -1, Physical, eFlight };

	i = 0;
	/* Pikachu */
	t_move PikachuMoves[4];
	PikachuMoves[i++] = (t_move){ "Thunderbolt", 0, 20, 90, 0, sShock, Special, eElectricity };
	PikachuMoves[i++] = (t_move){ "Quick Attack", 0, 20, 40, 0, -1, Physical, eNormal };
	PikachuMoves[i++] = (t_move){ "Thunder Wave", 0, 20, 0, 0, sShock, Special, eElectricity };
	PikachuMoves[i++] = (t_move){ "Thunder", 0, 20, 110, 0, sShock, Special, eElectricity };

	i = 0;
	/* Poliwrath */
	t_move PoliwrathMoves[4];
	PoliwrathMoves[i++] = (t_move){ "Hydro Pump", 0, 20, 110, 0, -1, Special, eWater };
	PoliwrathMoves[i++] = (t_move){ "Hypnosis", 0, 20, 0, 0, sSleep, Special, eNormal };
	PoliwrathMoves[i++] = (t_move){ "Submission", 0, 20, 80, -25, -1, Physical, eNormal };
	PoliwrathMoves[i++] = (t_move){ "Surf", 0, 20, 90, 0, 0, Special, eWater };

	i = 0;
	/* Snorlax */
	t_move SnorlaxMoves[4];
	SnorlaxMoves[i++] = (t_move){ "Body Slam", 0, 20, 85, 0, sShock, Physical, eNormal };
	SnorlaxMoves[i++] = (t_move){ "Rest", 0, 20, 0, 100, sSleep, Special, ePsychic };
	SnorlaxMoves[i++] = (t_move){ "Hyper Beam", 0, 20, 150, 0, -1, Physical, eNormal };
	SnorlaxMoves[i++] = (t_move){ "Amnesia", 0, 20, 0, 0, 0, Special, ePsychic };

	i = 0;
	/* Taurus (Tauros) */
	t_move TaurusMoves[4];
	TaurusMoves[i++] = (t_move){ "Tackle", 0, 20, 40, 0, -1, Physical, eNormal };
	TaurusMoves[i++] = (t_move){ "Stomp", 0, 20, 65, 0, sShock, Physical, eNormal };
	TaurusMoves[i++] = (t_move){ "Body Slam", 0, 20, 85, 0, sShock, Physical, eNormal };
	TaurusMoves[i++] = (t_move){ "Hyper Beam", 0, 20, 150, 0, -1, Physical, eNormal };

	/* local pointers to the per-pokemon move arrays */
	t_move *localMoves[PKMN_TYPE_LEN] = {
		ArbokMoves,
		BlastoiseMoves,
		ButterfreeMoves,
		ChanseyMoves,
		CharizardMoves,
		DodrioMoves,
		DragoniteMoves,
		DugtrioMoves,
		ExeggutorMoves,
		GengarMoves,
		GolemMoves,
		MagnetonMoves,
		NinetalesMoves,
		PersianMoves,
		PidgeotMoves,
		PikachuMoves,
		PoliwrathMoves,
		SnorlaxMoves,
		TaurusMoves
	};

	for (int p = 0; p < PKMN_TYPE_LEN; ++p) {
		for (int q = 0; q < 4; ++q) {
			localMoves[p][q].cc = localMoves[p][q].ccMax;
			pkd->PokemonMoves[p][q] = localMoves[p][q];
		}
	}
}

void	init_pokemon_types(t_pokemon_data *pkd) {
	pkd->pokemonTypes[Arbok] = ePoison;
	pkd->pokemonTypes[Blastoise] = eWater;
	pkd->pokemonTypes[Butterfree] = eFlight;
	pkd->pokemonTypes[Chansey] = eNormal;
	pkd->pokemonTypes[Charizard] = eFire;
	pkd->pokemonTypes[Dragonite] = eFlight;
	pkd->pokemonTypes[Dugtrio] = eGround;
	pkd->pokemonTypes[Exeggutor] = eGrass;
	pkd->pokemonTypes[Gengar] = eGhost;
	pkd->pokemonTypes[Golem] = eGround;
	pkd->pokemonTypes[Magneton] = eElectricity;
	pkd->pokemonTypes[Pikachu] = eElectricity;
	pkd->pokemonTypes[Ninetales] = eFire;
	pkd->pokemonTypes[Persian] = eNormal;
	pkd->pokemonTypes[Pidgeot] = eFlight;
	pkd->pokemonTypes[Poliwrath] = eWater;
	pkd->pokemonTypes[Snorlax] = eNormal;
	pkd->pokemonTypes[Taurus] = eNormal;
}


const char	*get_pkmn_name(t_md *md, t_pokemon_types type) {
	return (md->pkd.pkmn_names[type]);
}

static void	init_pkmn_labels(t_pokemon_data *pkd)
{
	pkd->pkmn_names[Arbok] = "Arbok";
	pkd->pkmn_names[Blastoise] = "Blastoise";
	pkd->pkmn_names[Butterfree] = "Butterfree";
	pkd->pkmn_names[Chansey] = "Chansey";
	pkd->pkmn_names[Charizard] = "Charizard";
	pkd->pkmn_names[Dodrio] = "Dodrio";
	pkd->pkmn_names[Dragonite] = "Dragonite";
	pkd->pkmn_names[Dugtrio] = "Dugtrio";
	pkd->pkmn_names[Exeggutor] = "Exeggutor";
	pkd->pkmn_names[Gengar] = "Gengar";
	pkd->pkmn_names[Golem] = "Golem";
	pkd->pkmn_names[Magneton] = "Magneton";
	pkd->pkmn_names[Ninetales] = "Ninetales";
	pkd->pkmn_names[Persian] = "Persian";
	pkd->pkmn_names[Pidgeot] = "Pidgeot";
	pkd->pkmn_names[Pikachu] = "Pikachu";
	pkd->pkmn_names[Poliwrath] = "Poliwrath";
	pkd->pkmn_names[Snorlax] = "Snorlax";
	pkd->pkmn_names[Taurus] = "Tauros";
}

int	getPkElColor(t_md *md, t_elType type) {
	return (md->pkd.elementColors[type]);
}

t_status_data	getStatusInfo(t_md *md, t_sType type) {
	return (md->pkd.statusData[type]);
}
char* getPkStatusName(t_md* md, t_sType type) {
	return (md->pkd.statusData[type].name);
}
int	getPkElement(t_md *md, int type) {
	return (md->pkd.pokemonTypes[type]);
}
void	init_pokemon_data(t_md *md) {
	init_pokemon_types(&md->pkd);
	init_pokemon_moves(&md->pkd);
	initElementColors(&md->pkd);
	initStatusNames(&md->pkd);
	init_pkmn_labels(&md->pkd);
}

void	init_ent_pkteam(t_md *md, t_ent *e, int team_size)
{
	int	i;

	e->pk_team = md_malloc(md, sizeof(t_ent *) * (team_size + 1));
	i = -1;
	while (++i < team_size) {
		e->pk_team[i] = init_ent(md, 'K', _v2(-1), -1);
	}
	e->pk_team[i] = NULL;
	e->team_sz = team_size;
}