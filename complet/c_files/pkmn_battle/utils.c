/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 11:28:59 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/01 15:06:17 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	exit_battle(t_md *md, t_battle_d *bd)
{
	stop_sound(md->au.battle_pid);
	play_sound(md, AU_PKB_ESCAPE);
	md->plr.mov = v3f(0);
	md->key_click = -1;
	bd->active = 0;
}

void	wait_action(t_md *md, int incr_hp, const char *msg, int change_turn)
{
	t_battle_d	*bd;

	bd = &md->battle_d;
	update_battle_text(bd, msg);
	refresh_battle_screen(md, bd);
	mlx_put_image_to_window(md->mlx, md->win, md->screen->img, 0, 0);
	bd->action_end = get_time_in_seconds() + bd->action_dur;
	if (incr_hp < 0)
		bd->stored_dealt[bd->my_turn] = incr_hp;
	else if (incr_hp)
		bd->stored_dealt[!bd->my_turn] = incr_hp;
	if (change_turn)
		bd->my_turn = !bd->my_turn;
}

void	handle_pkmn_ko(t_md *md, t_battle_d *bd, t_ent *pk, int i)
{
	t_ent	*new_pk;
	char	*msg;

	play_sound(md, md->au.pokemon_cries[pk->mob_type]);
	new_pk = NULL;
	if (i == 0 && bd->trainer)
		new_pk = get_valid_pkmn(bd->trainer->pk_team, bd->trainer->team_sz);
	else if (i == 1)
		new_pk = get_valid_pkmn(md->inv.pokemon_team, md->inv.team_size);
	if (new_pk)
	{
		bd->stored_dealt[i] = 0;
		if (i == 0)
			change_pokemon(md, new_pk, 0, 1);
		update_battle_text(bd, "%s is ko...", pk->label);
		return ;
	}
	bd->quitting = 1;
	bd->stored_dealt[0] = 0;
	bd->stored_dealt[1] = 0;
	msg = ft_megajoin(md->plr_name, "has no more pokemons..", NULL, NULL);
	wait_action(md, 0, msg, 0);
	free(msg);
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

int	free_battle_data(t_md *md, t_battle_d *bd)
{
	int	fa;
	int	i;

	fa = free_image_data(md, bd->overlay);
	i = -1;
	while (++i < 4)
		fa = free_image_data(md, bd->buttons[i]);
	return (fa);
}
