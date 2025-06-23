/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   held_item.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:05:46 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/01 15:07:45 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	use_held_item(t_md *md, t_inventory *inv, t_ent *pointed, int index)
{
	if (inv->held_used)
		return (0);
	if (index == Keys && md->cam.pointed_door)
	{
		md->cam.pointed_door->was_hit = 1;
		md->cam.pointed_door->hp = 0;
		play_sound(md, AU_OPEN);
		inv->held_used = 1;
		return (1);
	}
	else if (pointed)
	{
		inv->held_used = 1;
		inv->held_used_start = md->timer.cur_tm;
		inv->arc_origin = inv->pkbl_p;
		return (1);
	}
	inv->items[index]++;
	inv->held_i = -1;
	add_alert(md, .5f, NULL, "Can't use that");
	return (0);
}

void	capture_pokemon(t_md *md, t_inventory *inv, t_ent *e)
{
	if (!e)
		return ;
	if (r_range(0, 4) == 0)
	{
		add_alert(md, 2, NULL, "Oh no! The pokemon resisted..");
		play_sound(md, AU_PKB_FAIL);
		e->caught = 0;
		if (r_range(0, 15) > 3 && inv->team_size > 1)
			start_battle(md, &md->battle_d, NULL, e);
		return ;
	}
	if (inv->team_size > 5)
		add_alert(md, 2, NULL, "Team already full, placing in computer");
	inv->pokemon_team[inv->team_size++] = e;
	play_sound(md, AU_PKB_CATCH);
	add_log_to_queue(md, _BLUE, "%s caught!", e->label);
	remove_ent(md, e);
	inv->update_img = 1;
	e->is_active = 0;
}
