/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_camera.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:49:48 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/06 12:34:41 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	update_cam_bob(t_md *md, t_ent *plr)
{
	float	bob_speed;
	float	bob_amount;
	float	vrt_offset;
	int		is_running;

	is_running = md->key_prs[SHIFT_KEY];
	if (md->plr.grounded && !cmp_vec3f \
		(get_v3f(plr->mov.x, plr->mov.y, 0), v3f(0), 0.01f))
		md->cam.bob_time += md->timer.delta_time;
	bob_speed = BOB_SPD * (1 + is_running);
	bob_amount = md->prm.bob_amount;
	if (md->key_prs[SHIFT_KEY])
		bob_amount *= 2;
	vrt_offset = sinf(md->cam.bob_time * bob_speed) * bob_amount;
	md->cam.pos.z += vrt_offset;
}

void	update_plr_offsets(t_md *md, t_cam *cam)
{
	const t_vec2	win_sz = md->win_sz;
	const t_vec2	plr_size = md->plr.size;
	const t_ent		*plr = &md->plr;
	const t_vec3f	plr_p = plr->pos;
	t_vec3f			dspl;

	dspl.x = plr_p.x - plr_size.x / 2 - win_sz.x / 2 + md->t_len / 2;
	dspl.y = plr_p.y - plr_size.y / 2 - win_sz.y / 2 + md->t_len / 2;
	dspl.z = plr_p.z;
	cam->ofst = dspl;
	cam->wrd_mv_offst.x += cam->plr_wrd_mv.x * 20;
	cam->wrd_mv_offst.y += cam->plr_wrd_mv.y * 20;
	cam->input_offst.x += (int)(cam->plr_wrd_mv.x);
	cam->input_offst.y -= (int)(cam->plr_wrd_mv.y);
	cam->pos.x = plr_p.x + md->t_len / 4 - (plr->dir.x * md->prm.zoom);
	cam->pos.y = plr_p.y + md->t_len / 4 - (plr->dir.y * md->prm.zoom);
	cam->pos.z = plr_p.z - md->prm.height;
	md->plr.coord = div_v3((t_vec3){plr_p.x, plr_p.y, plr_p.z}, md->t_len);
}

int	get_center_valid_column(t_md *md, int center)
{
	const int	is_even_frame = md->timer.time % 2 == 0;
	const int	ray_mod = (int)floorf(md->prm.ray_mod);
	int			offset;
	int			left;
	int			right;

	if (ray_mod == 0)
		return (center);
	offset = -1;
	while (++offset < md->win_sz.x / 2)
	{
		left = center - offset;
		right = center + offset;
		if (left >= 0 && left % ray_mod == 0 && \
			((left % 2 == 0) == is_even_frame))
			return (left);
		if (right < md->win_sz.x && \
			right % ray_mod == 0 && \
			((right % 2 == 0) == is_even_frame))
			return (right);
	}
	return (center);
}

void	update_cam(t_md *md, t_cam *cam)
{
	const int	prv_is_moving = md->cam.is_moving;

	md->cam.closest_x = get_center_valid_column(md, md->win_sz.x / 2);
	cam->pointed_door = NULL;
	if (cam->prv_pointed_ent)
		cam->pointed_ent = md->cam.prv_pointed_ent;
	md->cam.prv_pointed_ent = NULL;
	if (md->cam.pointed_ent)
		cam->pointed = NULL;
	else if (cam->prv_pointed)
		cam->pointed = cam->prv_pointed;
	cam->prv_pointed = NULL;
	cam->is_moving = !cmp_vec3f(cam->input_mov, v3f(0), .01);
	if (prv_is_moving != cam->is_moving)
		md->timer.tm_walk = 0;
	update_plr_offsets(md, cam);
	if (md->prm.bob_amount > 0)
		update_cam_bob(md, &md->plr);
}

t_vec3f	update_fly_cam(t_md *md, t_cam *cam, float spd)
{
	t_vec3f			mov;
	const t_vec3f	cam_forward = normalize_vec3f(md->plr.dir);
	const t_vec3f	cam_right = get_v3f(-cam_forward.y, cam_forward.x, 0);
	const t_vec3f	cam_up = get_v3f(0, 0, -1);

	mov = v3f(0);
	if (md->key_prs[W_KEY])
		mov = add_vec3f(mov, scale_vec3f(cam_forward, spd));
	if (md->key_prs[S_KEY])
		mov = add_vec3f(mov, scale_vec3f(cam_forward, -spd));
	if (md->key_prs[D_KEY])
		mov = add_vec3f(mov, scale_vec3f(cam_right, spd));
	if (md->key_prs[A_KEY])
		mov = add_vec3f(mov, scale_vec3f(cam_right, -spd));
	if (md->key_prs[SPACE_KEY])
		mov = add_vec3f(mov, scale_vec3f(cam_up, spd * .05f));
	if (md->key_prs[Y_KEY])
		mov = add_vec3f(mov, scale_vec3f(cam_up, -spd * .05f));
	cam->input_mov.x = md->key_prs[A_KEY] - md->key_prs[D_KEY];
	cam->input_mov.y = md->key_prs[W_KEY] - md->key_prs[S_KEY];
	cam->input_mov.z = md->key_prs[SPACE_KEY] - md->key_prs[Y_KEY];
	return (mov);
}
