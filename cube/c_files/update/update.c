/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:45:36 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/09 16:58:14 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	create_wall(t_md *md)
{
	const int		depth = minmax(1, 10, (45 - (max(0, md->cam.rot.y)) / 2));
	const t_vec2	coord = v2(md->plr.coord.x + md->plr.dir.x * depth, \
	md->plr.coord.y + md->plr.dir.y * depth);
	t_ent			*e;
	t_dblst			*node;
	int				prv_index;

	e = get_mapped_at_cord(md, coord);
	if (!e || e->type != nt_empty)
		return (0);
	node = md->entities;
	while (node)
	{
		if ((t_ent *)node->content == e)
			break ;
		node = node->next;
	}
	if (!node)
		return (0);
	prv_index = e->map_index;
	free_ent(md, e);
	e = init_ent(md, '2', coord, prv_index);
	e->pos.z = -(md->t_len * max(0, -md->cam.rot.y * 100));
	e->coord.z = ((e->pos.z / md->t_len) / 450) / 2;
	return (node->content = (void *)e, play_sound(md, AU_BOP), 1);
}

void	update_audio(t_md *md, t_au_manager *au)
{
	play_loop(md, &au->mus_pid, AU_MUS, 1);
	play_loop(md, &au->wind_pid, AU_WIND, !md->menu.active && md->prm.fly_cam);
	if (md->menu.active && md->mouse.click == MOUSE_PRESS)
		play_sound(md, AU_MOUSE_CLICK);
	if (md->menu.active && md->mouse.click == MOUSE_RELEASE)
		play_sound(md, AU_MOUSE_RELEASE);
	if (!md->prm.au_on)
		return ;
	if (!md->timer.trig_walk || md->prm.fly_cam || md->inv.active)
		return ;
	if (!md->plr.grounded || !md->cam.is_moving)
		return ;
	if (md->cam.pos.z + md->prm.height < -.5 && !md->plr.on_floor)
		return ;
	if (!same_vec3f(md->plr.mov, v3f(0), .01))
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
	md->mouse.locked = !mode;
	if (mode)
		play_sound(md, AU_MENU_IN);
	else
		lock_mouse_center(md);
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
	if (md->BA_d.active)
		return (update_BA_scene(md, &md->BA_d),
			reset_mlx_values(md), 0);
	if (!md->map.explored_all && md->mmap.revealed_cur == md->mmap.revealed_len)
	{
		md->map.explored_all = 1;
		add_alert(md, 10, NULL, "Map fully explored, gg..");
	}
	update_portals(md, md->portal.found, md->portal.out_pos);
	update_audio(md, &md->au);
	if (md->menu.active)
		return (update_menu(md, &md->menu));
	update_input(md);
	if (md->BA_d.active)
		return (0);
	update_mouse(md);
	if (md->inv.active)
		update_inventory(md, &md->inv);
	if (md->timer.time > 5)
		update_player(md, &md->plr);
	return (render(md), reset_mlx_values(md), 0);
}
