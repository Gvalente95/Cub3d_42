/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:34:45 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/05 14:02:17 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	update_battle_text(t_battle_d *bd, const char *format, ...)
{
	va_list	args;

	va_start(args, format);
	vsnprintf(bd->log_message, sizeof(bd->log_message), format, args);
	va_end(args);
}

int	update_battle_input(t_md *md, t_battle_d *bd, int k)
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

void	update_animations(t_md *md, t_battle_d *bd, double cur_time)
{
	upd_timer(&md->timer.tm_anim, cur_time, ANIM_REFRESH, &md->timer.trig_anim);
	if (md->timer.trig_anim)
	{
		update_pk_frames(&md->inv, bd->pk[0]);
		update_pk_frames(&md->inv, bd->pk[1]);
		refresh_battle_screen(md, bd);
	}
}

int	update_battle_scene(t_md *md, t_battle_d *bd)
{
	const double	cur_time = get_time_in_seconds();
	const int		key = md->key_click;
	const int		has_sel = key == ENTER_KEY || \
		md->mouse.click == MOUSE_RELEASE;

	play_loop(md, &md->au.battle_pid, AU_PKB_MUS, 1);
	update_animations(md, bd, cur_time);
	if (bd->trans_lvl == 3 && md->key_click != -1)
		return (0);
	if (bd->trans_lvl == 3)
		bd->trans_lvl = 0;
	if (bd->action_end > cur_time || bd->stored_dealt[0] || bd->stored_dealt[1])
		return (update_action(md, bd), 1);
	if (bd->quitting)
		return (exit_battle(md, bd), 0);
	if (!bd->my_turn)
		return (battle_use_attack(md, 20, "Bite"));
	if (bd->trans_lvl)
		return (render_trans_screen(md, bd, cur_time - bd->trans_start), 1);
	if (has_sel && !set_battle_option(md, bd))
		return (0);
	else if (!has_sel && update_battle_input(md, bd, key))
		refresh_battle_screen(md, bd);
	return (reset_mlx_values(md), 0);
}

void	update_trans_lvl(t_battle_d *bd, double elapsed)
{
	if (elapsed > bd->trans_dur * 1.2)
		bd->trans_lvl = 2;
	if (elapsed > bd->trans_dur * 1.5)
	{
		bd->trans_start = 0;
		bd->trans_lvl = 3;
	}
}
