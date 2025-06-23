/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 01:55:29 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/11 18:15:13 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	update_buttons_triggers(t_md *md, t_menu *menu, unsigned int c)
{
	int	i;

	i = -1;
	while (menu->buttons[++i].active)
	{
		if (c != menu->buttons[i].key_trigger)
			continue ;
		*menu->buttons[i].value = !(*menu->buttons[i].value);
		menu->refresh_ui = 1;
		menu->refresh_bg = 1;
		reset_fps_timer(&md->timer);
		render(md);
		play_sound(md, AU_MOUSE_CLICK);
		return (1);
	}
	return (0);
}

int	update_key_input(t_md *md, t_menu *menu, t_inventory *inv, unsigned int c)
{
	if (update_buttons_triggers(md, menu, c))
		return (1);
	if (c == TAB_KEY)
		set_menu_mode(md, &md->menu, !menu->active);
	else if (c == I_KEY)
		set_inventory(md, &md->inv, !md->inv.active);
	else if (c == ESC_KEY)
		free_and_quit(md, NULL, NULL);
	else if (c == SHIFT_KEY)
		md->timer.tm_walk = md->timer.cur_tm - 1;
	if (inv->active)
		return (update_inv_input(md, inv, c));
	return (1);
}

static int	try_open_door(t_md *md, t_ent *door)
{
	if (door->was_hit)
	{
		door->hp = !door->hp;
		if (!door->hp)
			md->cam.prv_door = door;
		if (!door->hp)
			return (play_sound(md, AU_OPEN), 1);
		return (play_sound(md, AU_CLOSE), 1);
	}
	else if (md->inv.items[Keys] > 0)
	{
		door->hp = 0;
		door->was_hit = 1;
		return (play_sound(md, AU_OPEN), md->inv.items[Keys]--, 1);
	}
	add_alert(md, .5f, NULL, "No key left in inventory");
	return (0);
}

static int	update_mouse_input(t_md *md, t_ent *wall, t_ent *ent, t_ent *door)
{
	t_ent	*pk;
	t_ent	*enm_pk;

	if (md->mouse.click == MOUSE_RELEASE && md->inv.held_i > -1)
		return (use_held_item(md, &md->inv, ent, md->inv.held_i));
	if (md->mouse.click != MOUSE_RELEASE || (!wall && !ent && !door))
		return (0);
	if (wall && wall->type == nt_wall && md->key_prs[X_KEY])
		return (remove_ent(md, wall));
	if (door)
		try_open_door(md, door);
	if (ent && ent->type == nt_mob)
	{
		pk = get_valid_pkmn(md->inv.pokemon_team, md->inv.team_size);
		if (!pk)
			return (add_alert(md, .5f, NULL, "No Valid pokemon to fight"), 0);
		enm_pk = get_valid_pkmn(ent->pk_team, ent->team_sz);
		if (!enm_pk)
			return (add_alert(md, .5f, NULL, "Trainer's pokemon are Ko.."), 0);
		return (start_battle(md, &md->battle_d, ent, enm_pk), 1);
	}
	return (0);
}

void	update_input(t_md *md)
{
	float		r_spd;
	t_cam		*c;
	t_inventory	*inv;

	inv = &md->inv;
	if (md->key_click != -1)
		update_key_input(md, &md->menu, inv, (unsigned int)md->key_click);
	if (md->menu.active)
		return ;
	c = &md->cam;
	r_spd = ARROW_ROT_SPEED;
	if (md->key_prs[SHIFT_KEY])
		r_spd *= 2;
	md->prm.fov = minmax(10, 360, md->prm.fov + md->mouse.scroll_delta.y * 10);
	c->rot.x += ((md->key_prs[RIGHT_KEY] - md->key_prs[LEFT_KEY]) * r_spd);
	c->rot.y += ((md->key_prs[DOWN_KEY] - md->key_prs[UP_KEY]) * r_spd);
	if (!md->inv.active)
		update_mouse_input(md, c->pointed, c->pointed_ent, c->pointed_door);
	return ;
}
