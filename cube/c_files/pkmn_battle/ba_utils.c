/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 11:28:59 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/09 21:22:31 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	clear_BA_text(t_BA_d* bd) {
	bd->log_message[0] = '\0';
}
void	set_BA_text(t_BA_d* bd, const char* format, ...)
{
	va_list	args;

	va_start(args, format);
	vsnprintf(bd->log_message, sizeof(bd->log_message), format, args);
	va_end(args);
}

void	exit_battle(t_md *md, t_BA_d *bd)
{
	stop_sound(md->au.BA_pid);
	play_sound(md, AU_PKB_ESCAPE);
	md->plr.mov = v3f(0);
	md->key_click = -1;
	bd->active = 0;
}

int	handle_my_pkmn_ko(t_md *md, t_BA_d *bd)
{
	bd->stored_dealt[BME] = 0;
	bd->pk[BME] = NULL;
	bd->opt_i = 2;
	bd->sub_i = -1;
	for (int i = 0; i < md->inv.team_size; i++) {
		t_ent* e = md->inv.pokemon_team[i];
		if (!e) break;
		if (e->hp > 0) {
			bd->sub_i = i;
			return (0);
		}
	}
	set_bTransition(md, bd, bd->opponent->is_trainer ?
		BT_PUNITION : BT_QUIT, "All your pokemons are ko..");
	return (1);
}

int	handle_ennemy_pkmn_ko(t_md *md, t_BA_d *bd)
{
	t_ent	*valid_pk = NULL;

	bd->stored_dealt[BFOE] = 0;
	if (bd->opponent)
		valid_pk = get_valid_pkmn(bd->opponent->pk_team, bd->opponent->team_sz);
	if (valid_pk) {
		change_pokemon(md, valid_pk, 0, 1);
		return (0);
	}
	bd->stored_dealt[BME] = 0;
	stop_sound(md->au.BA_pid);
	if (bd->opponent->is_trainer) {
		set_BA_text(bd, "You Beat %s", bd->opponent->label);
		bd->bstate = BT_REWARD;
	}
	else {
		set_BA_text(bd, "foe %s is ko..", bd->opponent->label);
		bd->bstate = BT_WON;
	}
	return (1);
}

t_ent	*get_valid_pkmn(t_ent **team, int team_size)
{
	int	i;

	i = -1;
	while (++i < team_size)
	{
		if (!team[i])
			break ;
		if (team[i]->hp > 0)
			return (team[i]);
	}
	return (NULL);
}

t_move	*get_valid_move(t_ent* e) {
	if (!e || !e->poke_moves)
		return NULL;
	int	validIndex[4];
	int	validLen = 0;
	for (int i = 0; i < 3; i++) {
		if (e->poke_moves[i].cc)
			validIndex[validLen++] = i;
	}
	if (!validLen)
		return (NULL);
	int randIndex = r_range(0, validLen);
	return (&e->poke_moves[validIndex[randIndex]]);
}

int	free_BA_data(t_md *md, t_BA_d *bd)
{
	int	fa;
	int	i;

	fa = free_image_data(md, bd->overlay);
	i = -1;
	while (++i < 4)
		fa = free_image_data(md, bd->buttons[i]);
	return (fa);
}
