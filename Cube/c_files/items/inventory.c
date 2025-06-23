/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inventory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 16:31:25 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/30 19:25:03 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	close_option_pannel(t_inventory *inv)
{
	inv->sel_i = -1;
	inv->opt_i = 0;
}

void	collect_item(t_md *md, t_inventory *inv, t_ent *e)
{
	inv->items[e->pckp_type]++;
	add_log_to_queue(md, _RED, "%s collected", e->label);
	play_sound(md, AU_TUK);
	remove_ent(md, e);
	e->is_active = 0;
}

int	grab_item(t_md *md, t_inventory *inv, int item_index)
{
	char		*txt;
	const char	*label = md->txd.item_names[item_index];

	if (!inv->items[item_index])
	{
		txt = ft_megajoin("No ", label, " in bag", NULL);
		return (add_alert(md, 1, NULL, txt), free(txt), 0);
	}
	inv->update_img = 1;
	if (item_index == Pokeball || item_index == Keys)
	{
		if (md->inv.held_i != item_index && md->inv.held_screen_img)
		{
			md->inv.items[md->inv.held_i]++;
			free_image_data(md, md->inv.held_screen_img);
			md->inv.held_screen_img = NULL;
		}
		inv->held_i = item_index;
		set_inventory(md, inv, 0);
	}
	inv->items[item_index]--;
	return (play_sound(md, AU_GRAB), inv->items[item_index]);
}

void	set_inventory(t_md *md, t_inventory *inv, int active)
{
	(void)md;
	if (!inv->active && active)
		inv->update_img = 1;
	inv->hov_indexes[0] = 0;
	inv->hov_indexes[1] = 0;
	inv->opt_i = 0;
	inv->sel_i = -1;
	if (active)
		inv->update_map = 1;
	inv->active = active;
}

int	update_inv_selection(t_md *md, t_inventory *inv)
{
	if (inv->page == 1 && !inv->pokemon_team[inv->hov_indexes[1]])
		return (add_alert(md, .5f, NULL, "Pokemon is null"), 0);
	if (inv->page == 0 && !inv->items[inv->hov_indexes[0]])
		return (add_alert(md, .5f, NULL, "You don't have that item"), 0);
	inv->update_opt = 1;
	inv->update_img = 1;
	if (inv->page == 0)
	{
		if (inv->sel_i == -1)
			inv->sel_i = inv->hov_indexes[0];
		else if (!grab_item(md, inv, inv->hov_indexes[0]))
			close_option_pannel(inv);
		return (0);
	}
	if (inv->sel_i != inv->hov_indexes[1])
		return (inv->sel_i = inv->hov_indexes[1], 1);
	if (inv->opt_i == 0)
		return (0);
	else if (inv->opt_i == 1)
		return (0);
	play_sound(md, \
		md->au.pokemon_cries[inv->pokemon_team[inv->sel_i]->mob_type]);
	return (inv->pokemon_team[inv->sel_i] = NULL, inv->sel_i = -1, 1);
}
