/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 22:39:23 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 13:27:12 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	change_pokemon(t_md *md, t_ent *new, int side, int change_turn)
{
	t_battle_d	*bd;
	char		*msg;

	bd = &md->battle_d;
	if (new == bd->pk[side])
		return (1);
	if (!new || new->hp <= 0)
		return (0);
	bd->pk[side] = new;
	if (side == 1)
		msg = ft_strjoin("I choose you ", new->label);
	else if (bd->trainer)
		msg = ft_megajoin(bd->trainer->label, " choose ", new->label, NULL);
	else
		msg = ft_strdup("Error | No trainer on change_pokemon side");
	wait_action(md, 0, msg, change_turn);
	play_sound(md, md->au.pokemon_cries[new->mob_type]);
	free(msg);
	return (1);
}

int	use_battle_item(t_md *md, t_battle_d *bd, int item_index)
{
	const char	*item_name = md->txd.item_names[item_index];

	if (item_index == PCKP_TYPE_LEN)
		return (1);
	else if (!md->inv.items[item_index])
	{
		play_sound(md, AU_PKB_FAIL);
		update_battle_text(bd, "You don't have that item (%s)", item_name);
		return (1);
	}
	else if (item_index != Health)
	{
		play_sound(md, AU_PKB_FAIL);
		update_battle_text(bd, "Can't use %s in combat", item_name);
		return (1);
	}
	wait_action(md, -100, "Used a Super potion", 1);
	md->inv.items[item_index]--;
	return (1);
}

void	update_action(t_md *md, t_battle_d *bd)
{
	int		i;
	int		incr;
	t_ent	*pk;

	if (bd->quitting)
		return ;
	i = -1;
	while (++i < 2)
	{
		if (bd->stored_dealt[i] == 0)
			continue ;
		pk = bd->pk[i];
		if (bd->stored_dealt[i] == 0)
			continue ;
		incr = -1 + ((bd->stored_dealt[i] < 0) * 2);
		bd->pk[i]->hp = minmax(0, pk->max_hp, pk->hp + incr);
		bd->stored_dealt[i] += incr;
		if (pk->hp <= 0)
			handle_pkmn_ko(md, bd, pk, i);
	}
	refresh_battle_screen(md, bd);
}

int	set_battle_option(t_md *md, t_battle_d *bd)
{
	if (!bd->in_sub)
		return (0);
	play_sound(md, AU_TUK);
	if (bd->opt_i == 0)
	{
		if (bd->sub_i == 0)
			return (battle_use_attack(md, 30, "bite"));
		if (bd->sub_i == 1)
			return (battle_use_attack(md, 10, "Charge"));
		if (bd->sub_i == 2)
			return (battle_use_attack(md, 20, "Griffe"));
		return (battle_use_attack(md, 100, "Stuff"));
	}
	else if (bd->opt_i == 1)
	{
		use_battle_item(md, bd, bd->sub_i);
		refresh_battle_screen(md, bd);
	}
	else if (bd->opt_i == 2 && \
		!change_pokemon(md, md->inv.pokemon_team[bd->sub_i], 1, 1))
		return (0);
	else if (bd->opt_i == 3)
		return (exit_battle(md, bd), 1);
	return (refresh_battle_screen(md, bd), 1);
}

int	battle_use_attack(t_md *md, int dmg, const char *atk_name)
{
	char		*label;
	t_battle_d	*bd;
	t_ent		*pk;
	int			is_critical;

	bd = &md->battle_d;
	pk = md->battle_d.pk[bd->my_turn];
	if (r_range(0, 10) == 0)
	{
		label = ft_strjoin(pk->label, " missed it's attack..");
		return (wait_action(md, 0, label, 1), free(label), 1);
	}
	is_critical = (dmg > 0 && r_range(0, 4) == 0);
	if (is_critical)
	{
		label = ft_megajoin(pk->label, " used ", atk_name, " [CRITICAL HIT]");
		play_sound(md, AU_PKB_HIT);
	}
	else
	{
		label = ft_megajoin(pk->label, " used ", atk_name, NULL);
		play_sound(md, AU_PKB_CRIT_HIT);
	}
	wait_action(md, dmg + (is_critical * dmg), label, 1);
	return (free(label), 1);
}
