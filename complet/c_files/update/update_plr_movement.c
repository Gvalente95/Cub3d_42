/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_plr_movement.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 11:45:19 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/11 19:15:39 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	move_player(t_md *md, t_ent *e)
{
	t_vec3f	sign_mv;
	t_vec3f	decc;
	float	speed;

	speed = md->prm.plr_speed / RESOLUTION;
	sign_mv = scale_vec3f(e->mov, speed);
	decc = scale_vec3f(e->mov, md->prm.mov_drift);
	if (!md->prm.fly_cam && e->pos.z + sign_mv.z > 0)
	{
		e->pos.z = 0;
		decc.z = 0;
		sign_mv.z = 0;
		e->mov.z = 0;
	}
	e->pos = add_vec3f(e->pos, sign_mv);
	e->mov.x -= decc.x;
	e->mov.y -= decc.y;
	return (1);
}

static t_vec3f	set_input_mov_2(t_md *md, float spd, \
		t_vec3f for_dir, t_vec3f rgt_dir)
{
	t_vec3f	mov;
	float	mv_for;
	float	mv_back;
	float	mv_rght;
	float	mv_lft;

	mov = v3f(0);
	if (md->inv.active)
		return (mov);
	mv_for = ((md->key_prs[W_KEY]) * spd);
	mv_back = ((md->key_prs[S_KEY]) * spd);
	mv_rght = ((md->key_prs[D_KEY]) * spd);
	mv_lft = ((md->key_prs[A_KEY]) * spd);
	mov.x = (mv_for - mv_back) * for_dir.x + (mv_rght - mv_lft) * -rgt_dir.x;
	mov.y = (mv_for - mv_back) * for_dir.y + (mv_rght - mv_lft) * -rgt_dir.y;
	if (md->key_click == SPACE_KEY && \
		md->plr.pos.z + md->prm.height >= -EPSILON)
		mov.z = - ((1 + md->prm.mov_drift) * 5);
	else if (md->key_prs[R_KEY] == 1)
		mov.z = -(spd * .2);
	md->cam.input_mov = get_v3f(mv_lft - mv_rght, mv_for - mv_back, \
		md->key_click == SPACE_KEY || md->key_prs[LFTCMD_KEY] == 1);
	return (mov);
}

static t_vec3f	set_input_mov(t_md *md)
{
	float	spd;
	t_vec3f	for_dir;
	t_vec3f	rgt_dir;

	spd = 1 + md->prm.mov_drift;
	if (md->key_prs[SHIFT_KEY])
		spd *= 2;
	if (md->prm.fly_cam)
		return (update_fly_cam(md, &md->cam, spd));
	for_dir.x = md->plr.dir.x;
	for_dir.y = md->plr.dir.y;
	rgt_dir.x = cosf(md->plr.angle - M_PI_2);
	rgt_dir.y = sinf(md->plr.angle - M_PI_2);
	return (set_input_mov_2(md, spd, for_dir, rgt_dir));
}

void	set_plr_z(t_md *md, t_ent *plr)
{
	if (!md->prm.fly_cam)
	{
		if (md->plr_in_house && plr->pos.z < -md->t_len / 3)
			plr->mov.z = GRAVITY * 10;
		plr->grounded = 0;
		if (plr->pos.z + md->prm.height < 0 && !md->key_prs[R_KEY])
			plr->mov.z += GRAVITY;
		else if (plr->pos.z + md->prm.height > 0)
		{
			plr->pos.z = -md->prm.height;
			plr->mov.z = 0;
		}
	}
}

int	update_player_mov(t_md *md, t_ent *plr)
{
	t_vec3f	conv_mov;
	t_vec3f	new_mov;
	t_vec3f	for_dir;
	t_vec3f	rgt_dir;

	conv_mov = set_input_mov(md);
	if (md->prm.fly_cam)
		conv_mov.z *= 100;
	new_mov = add_vec3f(plr->mov, conv_mov);
	for_dir = plr->dir;
	rgt_dir = get_v3f(cosf(plr->angle - M_PI_2), \
		sinf(plr->angle - M_PI_2), 0);
	md->cam.plr_wrd_mv.x = new_mov.x * rgt_dir.x + new_mov.y * rgt_dir.y;
	md->cam.plr_wrd_mv.y = -(new_mov.x * for_dir.x + new_mov.y * for_dir.y);
	md->cam.plr_wrd_mv.z = new_mov.z;
	plr->mov.x += new_mov.x * md->timer.delta_time;
	plr->mov.y += new_mov.y * md->timer.delta_time;
	if (md->prm.fly_cam)
		plr->mov.z = new_mov.z * md->timer.delta_time;
	else
		plr->mov.z += new_mov.z * md->timer.delta_time;
	set_plr_z(md, plr);
	return (conv_mov.x || conv_mov.y);
}
