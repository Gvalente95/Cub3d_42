/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ba_actions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 11:55:48 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/16 12:01:19 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	change_pokemon(t_md *md, t_ent *new_ent, int side, int change_turn)
{
	t_BA_d	*bd;
	char	*msg;

	bd = &md->BA_d;
	if (new_ent == bd->pk[side] && change_turn)
		return (1);
	if (!new_ent || new_ent->hp <= 0)
		return (0);
	bd->pk[side] = new_ent;
	if (side == 1)
		msg = ft_strjoin("Go! ", new_ent->label);
	else if (bd->opponent)
		msg = ft_megajoin(bd->opponent->label, \
			" sent out ", new_ent->label, NULL);
	else
		msg = ft_strdup("Error | No opponent on change_pokemon side");
	play_sound(md, md->au.pokemon_cries[new_ent->mob_type]);
	if (change_turn)
		set_bTransition(md, BT_SWITCH_TURN, msg);
	else
		set_bTransition(md, BT_WAIT, msg);
	free(msg);
	return (1);
}

int	use_battle_item(t_md *md, t_BA_d *bd, int item_index)
{
	const char	*item_name = md->txd.item_names[item_index];

	if (item_index == PCKP_TYPE_LEN)
		return (1);
	else if (!md->inv.items[item_index])
	{
		play_sound(md, AU_PKB_FAIL);
		set_BA_text(bd, "You don't have that item (%s)", item_name);
		return (1);
	}
	else if (item_index != Health)
	{
		play_sound(md, AU_PKB_FAIL);
		set_BA_text(bd, "Can't use %s in combat", item_name);
		return (1);
	}
	set_bTransition(md, BT_SWITCH_TURN, "Used a super potion");
	md->inv.items[item_index]--;
	return (1);
}

void	update_move_action(t_md *md, t_BA_d *bd)
{
	int		hp_changed;
	int		i;
	int		incr;
	int		new_hp;
	t_ent	*pk;

	hp_changed = 0;
	i = -1;
	while (++i < 2)
	{
		if (bd->stored_dealt[i] == 0)
			continue ;
		incr = -1 + ((bd->stored_dealt[i] < 0) * 2);
		bd->stored_dealt[i] += incr;
		pk = bd->pk[i];
		if (!pk)
			continue ;
		new_hp = minmax(0, pk->max_hp, pk->hp + incr);
		if (new_hp != pk->hp)
		{
			pk->hp = new_hp;
			hp_changed = 1;
		}
		if (pk->hp <= 0)
			bd->stored_dealt[i] = 0;
		else if (pk->hp >= pk->max_hp)
		{
			bd->stored_dealt[i] = 0;
			pk->hp = pk->max_hp;
		}
	}
	render_battle(md, bd);
	if (!hp_changed)
		end_action(md, bd);
}

int	use_battle_move(t_md *md, t_BA_d *bd, t_ent *user, t_move *move)
{
	int			is_foe;
	const char	*foe;

	if (!move || !user)
		return (0);
	is_foe = bd->turn == BFOE;
	if (is_foe)
		foe = "foe ";
	else
		foe = "";
	if (!strncmp(move->name, "struggle", 8))
	{
		if (move->cc <= 0)
		{
			set_action(md, NULL, BT_WAIT, " can't be used: no cc..");
			return (0);
		}
		move->cc--;
	}
	t_status_data st = user->pkStatus;
	if (st.active) {
		int miss = r_range(0, 100) < st.missProbability;
		if (miss) {
			set_bTransition(md, BT_SWITCH_TURN,
				"%s%s missed it's attack because he's %s", foe, user->label, st.name);
			return (1);
		}
		else if (st.type == sConfusion && r_range(0, 100) > st.missProbability){
			set_bTransition(md, BA_MOVE_SELF,
				"%s%s attacked itself because he's %s", foe, user->label, st.name);
			return (1);
		}
	}
	if (r_range(0, 10) == 0)
	{
		set_bTransition(md, 
			BT_SWITCH_TURN, "%s%s missed it's attack...", foe, user->label);
		return (1);
	}
	set_action(md, move, BA_MOVE_USED, NULL);
	return (1);
}

void	update_action(t_md *md, t_BA_d *bd)
{
	if (bd->stored_dealt[BME] || bd->stored_dealt[BFOE])
		update_move_action(md, bd);
	else if (md->mouse.click == MOUSE_RELEASE)
		end_action(md, bd);
}

void	switch_turn(t_md* md, t_BA_d* bd) {
	bd->turn = !bd->turn;
	t_ent* pk = bd->turn == BME ? bd->pk[BME] : bd->opponent->is_trainer ? bd->pk[BFOE] : bd->opponent;
	if (!update_status_effect(md, pk))
		bd->bstate = BT_ON;
}

int	try_set_status(t_md* md, t_BA_d* bd, t_ent* victim, t_move* move) {
	if (!move || !victim || !victim->hp) return (0);
	int	newType = move->statusInflicted;
	if (newType < 0) return (0);
	int affect = r_range(0, 100) > 30;
	if (!affect) return (0);
	t_ent* pk = bd->pk[!bd->turn];
	if (!pk) return (0);
	if (pk->pkStatus.active) {
		set_BA_text(bd, "%s is already %s", pk->label, pk->pkStatus.name);
		return (1);
	}
	setStatus(md, &pk->pkStatus, newType);
	set_BA_text(bd, "%s is now %s", pk->label, pk->pkStatus.name);
	return (1);
}

void	end_action(t_md* md, t_BA_d* bd)
{
	int		prev_action;
	t_move	*last_move;
	bool	has_given_status;

	prev_action = bd->action_type;
	last_move = bd->last_move_used;
	has_given_status = false;
	bd->log_message[0] = '\0';
	bd->opt_i = 0;
	bd->sub_i = 0;
	bd->last_move_used = NULL;
	bd->action_type = BA_NO_ACTION;
	if (battleHasEnded(md, bd))
		return ;
	if (prev_action == BA_MOVE_SELF)
		set_bTransition(md, BT_SWITCH_TURN, "%s hurt itself in confusion..", bd->pk[bd->turn]);
	else if (prev_action != BA_STATUS_EFFECT && last_move) {
		t_ent* victim = bd->turn == BME ? bd->pk[BME] : bd->opponent->is_trainer ? bd->pk[BFOE] : bd->opponent;
		has_given_status = try_set_status(md, bd, victim, last_move);
	}
	if (has_given_status)
		set_bTransition(md, BT_SWITCH_TURN, NULL);
	else
	{
		switch_turn(md, bd);
		bd->bstate = BT_ON;
	}
}

void	set_action(t_md *md, t_move *moveToUse, int actionType, const char* format, ...)
{
	t_BA_d	*bd;

	bd = &md->BA_d;
	bd->action_type = actionType;
	if (moveToUse) {
		bd->last_move_used = moveToUse;
		t_ent* user = bd->pk[bd->turn];
		int damageOther = moveToUse->damage;
		int recoveryMine = moveToUse->recovery;
		if (actionType == BA_MOVE_SELF) {
			recoveryMine = -30;
			damageOther = 0;
		}
		int is_critical = (damageOther > 0 && r_range(0, 8) == 0);
		if (damageOther || recoveryMine < 0)
			play_sound(md, is_critical ? AU_PKB_CRIT_HIT : AU_PKB_HIT);
		bd->stored_dealt[!bd->turn] = damageOther;
		bd->stored_dealt[bd->turn] = -recoveryMine;
		if (actionType == BA_MOVE_SELF)
			clear_BA_text(bd);
		else if (!format)
			set_BA_text(bd, "%s used %s %s", user->label, moveToUse->name, is_critical ? " [CRIT]" : "");
	}
	if (format) {
		va_list	args;
		va_start(args, format);
		vsnprintf(bd->log_message, sizeof(bd->log_message), format, args);
		va_end(args);
	}
}

int	set_BA_option(t_md *md, t_BA_d *bd)
{
	if (bd->opt_i == 3) {
		set_bTransition(md, BT_QUIT, "You run away");
		return (0);
	}
	if (!bd->in_sub)
		return (0);
	play_sound(md, AU_TUK);
	if (bd->opt_i == 0 && bd->sub_i >= 0 && bd->sub_i < 4 && bd->pk[BME])
		return (use_battle_move(md, bd, bd->pk[BME], &bd->pk[BME]->poke_moves[bd->sub_i]));
	else if (bd->opt_i == 1)
	{
		use_battle_item(md, bd, bd->sub_i);
		render_battle(md, bd);
	}
	else if (bd->opt_i == 2 && \
		!change_pokemon(md, md->inv.pokemon_team[bd->sub_i], 1, 1))
		return (0);
	return (render_battle(md, bd), 1);
}
