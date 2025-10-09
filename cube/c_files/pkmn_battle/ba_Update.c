/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ba_Update.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:34:45 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/09 22:31:07 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	update_BA_input(t_md *md, t_BA_d *bd, int k)
{
	t_vec2		incr;
	int			cur_but_row;
	int			cur_sub_row;
	const int	rows[4] = {4, PCKP_TYPE_LEN - 1, md->inv.team_size, 1};
	const int	cur_sub = min(0, bd->but_i);

	if (k == Q_KEY)
		return (exit_battle(md, bd), 1);
	incr.x = (k == RIGHT_KEY || k == D_KEY) - (k == LEFT_KEY || k == A_KEY);
	incr.y = (k == DOWN_KEY || k == S_KEY) - (k == UP_KEY || k == W_KEY);
	if (!incr.x && !incr.y)
		return (0);
	if (incr.x)
		return (bd->in_sub = (incr.x > 0), bd->sub_i = 0, 1);
	if (!bd->in_sub)
	{
		cur_but_row = bd->opt_i + incr.y;
		if (cur_but_row < 0 || cur_but_row > 4)
			return (0);
		return (bd->opt_i = cur_but_row, 1);
	}
	cur_sub_row = bd->sub_i + incr.y;
	if (cur_sub_row < 0 || cur_sub_row >= rows[cur_sub])
		return (0);
	return (bd->sub_i = cur_sub_row, 1);
}

void	update_animations(t_md *md, t_BA_d *bd, double cur_time)
{
	upd_timer(&md->timer.tm_anim, cur_time, ANIM_REFRESH, &md->timer.trig_anim);
	if (md->timer.trig_anim)
	{
		if (bd->pk[0]) update_pk_frames(md, bd->pk[0]);
		else if (bd->opponent) update_ent_frame(md, bd->opponent);
		if (bd->pk[1]) update_pk_frames(md, bd->pk[1]);
		render_battle(md, bd);
	}
}

void	update_status_effect(t_md* md, t_ent* pk) {
	if (!pk) return;
	t_BA_d* bd = &md->BA_d;
	t_status_data* sd = &pk->pkStatus;
	if (!sd->active) return;
	if (sd->turns && (sd->turns > 5 || r_range(0, 100) < sd->endProbability)) {
		set_bTransition(md, &md->BA_d, BT_WAIT, "%s is no longer %s", pk->label, sd->name);
		sd->active = false;
		return;
	}
	if (sd->damagePerTurn && r_range(0, 100) > sd->damageProbability) {
		int damageAmount = sd->damagePerTurn + sd->damageMult * sd->turns;
		bd->stored_dealt[bd->my_turn] = damageAmount;
		set_action(md, NULL, BA_STATUS_EFFECT, "%s lost hp because he's %s", bd->pk[bd->my_turn], sd->name);
	}
	sd->turns++;
}

int	update_opponent_turn(t_md* md, t_BA_d* bd) {
	t_ent* opponent = bd->opponent;
	t_ent* pk;

	if (!bd->pk[BME]) {
		set_bTransition(md, bd, BT_SWITCH_TURN, "Error - No pokemon on MY side");
		return (1);
	}
	pk = opponent->is_trainer ? bd->pk[BFOE] : opponent;
	t_move* rmove = get_valid_move(pk);
	if (!rmove) rmove = &md->pkd.struggle;
	use_battle_move(md, bd, pk, rmove);
	return (1);
}

int	update_BA_scene(t_md *md, t_BA_d *bd)
{
	const double	cur_time = get_time_in_seconds();
	const int		key = md->key_click;
	const int		has_sel = key == ENTER_KEY || \
		md->mouse.click == MOUSE_RELEASE;

	if (bd->bstate != BT_START)
		render_cursor(md, NULL, 1);
	play_loop(md, &md->au.BA_pid, (bd->bstate == BT_REWARD || bd->bstate == BT_WON) ? AU_PKB_WIN : AU_PKB_MUS, 1);
	update_animations(md, bd, cur_time);
	if (bd->quitting)
		return (exit_battle(md, bd), 0);
	if (bd->bstate != BT_ON)
		return (update_bstate(md, bd, cur_time - bd->trans_start));
	if (bd->action_type)
		return (update_action(md, bd), 1);
	if (!bd->my_turn)
		return (update_opponent_turn(md, bd));
	if (has_sel && !set_BA_option(md, bd))
		return (0);
	else if (!has_sel && update_BA_input(md, bd, key))
		render_battle(md, bd);
	return (0);
}

void	set_bTransition(t_md* md, t_BA_d* bd, int type, const char* format, ...) {
	bd->bstate = type;
	bd->trans_start = md->timer.cur_tm;
	if (format) {
		va_list	args;
		va_start(args, format);
		vsnprintf(bd->log_message, sizeof(bd->log_message), format, args);
		va_end(args);
	}
}

int	update_bstate(t_md* md, t_BA_d* bd, double elapsed)
{
	if (bd->bstate == BT_START) {
		render_trans_screen(md, bd, elapsed);
		if (elapsed > bd->trans_dur) {
			if (bd->opponent->is_trainer)
				set_bTransition(md, bd, BT_OPPPKSET, "%s wants to fight!", bd->opponent->label);
			else
				set_bTransition(md, bd, BT_MYPKSET, "A wild %s appeared!", bd->opponent->label);
		}
		return (1);
	}
	if (md->mouse.click != MOUSE_RELEASE) return (1);
	switch (bd->bstate)
	{
		case BT_OPPPKSET:
			change_pokemon(md, get_valid_pkmn(bd->opponent->pk_team, bd->opponent->team_sz), 0, 0);
			bd->bstate = BT_MYPKSET;
			break;
		case BT_MYPKSET:
			change_pokemon(md, get_valid_pkmn(md->inv.pokemon_team, md->inv.team_size), BME, 0);
			bd->my_turn = true;
			break;
		case BT_PUNITION:
			set_BA_text(bd, "you gave %s a %s", bd->opponent->label, get_item_name(md, md->inv.items[0]));
			md->plr.pos = md->plr.start_pos;
			bd->bstate = BT_QUIT;
			break;
		case BT_REWARD:
		{
			int item = r_range(0, PCKP_TYPE_LEN - 1);
			add_item(md, item, 1);
			set_BA_text(bd, "%s gave you a %s", bd->opponent->label, get_item_name(md, item));
			bd->bstate = BT_QUIT;
			break;
		}
		break;
		case BT_SWITCH_TURN:
			BA_switch_turn(md, bd);
			break;
		case BT_QUIT: case BT_WON:
			exit_battle(md, bd);
			break;
		default:
			set_BA_text(bd, "");
			set_bTransition(md, bd, BT_ON, NULL);
		break;
	}
	bd->stored_dealt[BME] = 0;
	bd->stored_dealt[BFOE] = 0;
	return (1);
}
