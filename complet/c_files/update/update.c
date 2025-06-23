/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:45:36 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/24 14:00:59 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	update_audio(t_md *md, t_au_manager *au)
{
	play_loop(md, &au->mus_pid, AU_MUS, 1);
	play_loop(md, &au->wind_pid, AU_WIND, !md->menu.active && md->prm.fly_cam);
	if (!md->prm.au_on)
		return ;
	if (!md->timer.trig_walk || md->prm.fly_cam || md->inv.active)
		return ;
	if (!md->plr.grounded || !md->cam.is_moving)
		return ;
	if (md->cam.pos.z + md->prm.height < -.5)
		return ;
	if (!cmp_vec3f(md->plr.mov, v3f(0), .01))
		md->au.walk_index = \
			play_rand_sound(md, AU_WALK_GRASS, 8, md->au.walk_index);
}

static void	update_portals(t_md *md, t_ent *e, t_vec2 out_pos)
{
	t_portal	*p;
	int			index;

	p = &md->portal;
	if (!md->portal.found)
		return ;
	if (!p->ends[0].e)
		p->ends[0].e = e;
	else if (!p->ends[1].e)
		p->ends[1].e = e;
	else
	{
		index = p->last_shot_index;
		p = &md->portal;
		free_image_data(md, p->ends[index].e->overlay);
		p->ends[index].e->overlay = NULL;
		p->ends[index].e = e;
		p->ends[index].dir = e->overlay_dir;
		p->ends[index].out = out_pos;
		p->last_shot_index = !p->last_shot_index;
	}
	p->found = NULL;
}

int	set_menu_mode(t_md *md, t_menu *menu, int mode)
{
	if (mode)
		play_sound(md, AU_MENU_IN);
	md->mouse.locked = !mode;
	if (md->is_linux)
	{
		if (mode)
		{
			show_cursor(md);
			mlx_mouse_move(md->mlx, md->win, \
				md->win_sz.x / 2, md->win_sz.y / 2);
		}
		else
		{
			hide_cursor(md);
			mlx_mouse_move(md->mlx, md->win, \
				md->mouse.prev.x, md->mouse.prev.y);
		}
	}
	menu->active = mode;
	menu->slider_hov = -1;
	menu->button_hov = -1;
	menu->refresh_bg = mode;
	menu->refresh_ui = mode;
	return (mode);
}

int	update_and_render(t_md *md)
{
	if (md->autocam.active)
		return (update_autocam(md, &md->autocam));
	if (md->battle_d.active)
		return (update_battle_scene(md, &md->battle_d));
	update_portals(md, md->portal.found, md->portal.out_pos);
	update_audio(md, &md->au);
	if (md->menu.active)
		return (update_menu(md, &md->menu));
	update_input(md);
	if (md->battle_d.active)
		return (0);
	update_mouse(md);
	if (md->inv.active)
		update_inventory(md, &md->inv);
	if (md->timer.time > 5)
		update_player(md, &md->plr);
	update_ents(md);
	render(md);
	reset_mlx_values(md);
	return (0);
}
