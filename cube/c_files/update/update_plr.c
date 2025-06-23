/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_plr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:43:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/01 13:55:11 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	update_player_rot(t_md *md)
{
	float		speed;
	const float	pitch = md->cam.rot.y * (_PI / 180.0f);

	speed = md->prm.rot_speed;
	if (!md->mouse.lock_rot.x && md->mouse.delta.x && md->mouse.focus)
		md->cam.rot.x += (md->mouse.delta.x * speed);
	if (md->cam.rot.x < -180.0f)
		md->cam.rot.x += 360.0f;
	else if (md->cam.rot.x > 180.0f)
		md->cam.rot.x -= 360.0f;
	if (!md->mouse.lock_rot.y && md->mouse.delta.y && md->mouse.focus)
		md->cam.rot.y += (md->mouse.delta.y * speed);
	if (md->prm.fly_cam)
		md->cam.rot.y = minmaxf(-140, 140, md->cam.rot.y);
	else
		md->cam.rot.y = minmaxf(-80, 80, md->cam.rot.y);
	md->cam.rot.z = 0;
	md->plr.angle = (md->cam.rot.x) * (_PI / 180.0f);
	md->plr.dir.x = cosf(md->plr.angle);
	md->plr.dir.y = sinf(md->plr.angle);
	md->plr.dir.z = sinf(pitch) * .05;
}

static void	update_player_action(t_md *md, t_ent *plr)
{
	int				is_moving;
	t_ent_action	prv_action;

	prv_action = plr->action;
	is_moving = !cmp_vec3f(md->plr.mov, v3f(0), EPSILON);
	if (is_moving)
	{
		set_collisions(md, plr, v2(plr->size.x, plr->size.y));
		md->plr.action = m_walk;
		if (!plr->mov.x)
			md->cam.plr_wrd_mv.x = 0;
		if (!plr->mov.y)
			md->cam.plr_wrd_mv.y = 0;
		move_player(md, plr);
	}
	else if (md->plr.shot)
		md->plr.action = m_atk;
	else
		md->plr.action = m_idle;
	if (md->plr.action != prv_action)
		md->plr.frame_index = 0;
}

int	update_player(t_md *md, t_ent *plr)
{
	t_vec2	plr_map;
	int		map_i;

	plr_map.x = minmax(0, md->map.size.x, (int)(plr->pos.x / md->t_len));
	plr_map.y = minmax(0, md->map.size.y, (int)(plr->pos.y / md->t_len));
	map_i = plr_map.y * (md->map.size.x + 1) + plr_map.x;
	if (map_i < 0 || map_i > md->map.len)
		md->plr_in_house = 0;
	else if (md->out_map[map_i] != 'D')
		md->plr_in_house = char_in_str(md->out_map[map_i], "I");
	md->cam.plr_map_i = minmax(0, md->map.len - 1, map_i);
	if (!md->inv.active)
		update_player_rot(md);
	update_player_mov(md, plr);
	update_player_action(md, plr);
	update_cam(md, &md->cam);
	md->plr.pos.z = minmaxf(-md->t_len * 10, 0, md->plr.pos.z);
	md->hud.floor_start = md->win_sz.y / 2 - md->cam.rot.y * 8 + 1;
	if (md->hud.floor_start < 0)
		md->hud.floor_start = 0;
	md->plr.grounded = fabsf(md->plr.mov.z) < EPSILON;
	return (1);
}
