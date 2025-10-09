/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_autocam.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 22:58:02 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/08 14:11:40 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	exit_autocam(t_md *md, t_autocam *autocam)
{
	md->plr.pos.x = md->plr.start_pos.x;
	md->plr.pos.y = md->plr.start_pos.y;
	md->plr.pos.z = md->plr.start_pos.z;
	md->plr.coord.x = md->plr.pos.x / md->t_len;
	md->plr.coord.y = md->plr.pos.y / md->t_len;
	md->plr.coord.z = md->plr.pos.z / md->t_len;
	md->cam.rot.y = 0;
	md->cam.rot.x = md->cam.x_dir_start;
	md->plr.dir.x = 0;
	md->plr.dir.y = 0;
	md->timer.time = 1;
	md->key_click = -1;
	md->prm.fly_cam = 0;
	md->prm.show_ceiling = 1;
	md->prm.ent_mode = 1;
	md->fx.fog = .2f;
	md->prm.ray_depth = md->map.size.x * 1.5 * md->t_len;
	autocam->active = 0;
	autocam->quitting = 0;
	add_item(md, Pokeball, 5);
	add_item(md, Keys, 2);
	add_item(md, Health, 5);
	return (1);
}

static void	init_autocam(t_md *md, t_autocam *autocam)
{
	const t_vec2	center = div_v2(md->win_sz, 2);
	const float		map_w = minf(40, md->map.size.x) * md->t_len;
	const float		map_h = minf(40, md->map.size.y) * md->t_len;
	const float		map_diag = sqrtf(map_w * map_w + map_h * map_h);

	autocam->hasFade = true;
	autocam->quitting = false;
	autocam->center = center;
	autocam->map_w = map_w;
	autocam->map_h = map_h;
	autocam->map_diag = map_diag;
	autocam->acc_spd = 0.0f;
	autocam->base_y = 80 - md->map.size.y;
	md->plr.angle = M_PI_2;
	md->plr.pos.z = -md->t_len * minf(8, (md->map.size.y / 2));
	md->fx.fog = .05f;
	md->timer.time = 10;
	md->prm.ray_depth = md->map.size.x * 4 * md->t_len;
	md->prm.fly_cam = true;
	md->prm.show_ceiling = false;
	md->prm.super_view = true;
	md->prm.ent_mode = false;
}

void	update_player_orbit(t_md *md, t_autocam *aut)
{
	t_vec2f			delta;
	const float		amplitude = .3f;
	const float		spd = 5.0f;

	md->plr.angle += 0.005f;
	if (md->plr.angle > _PI * 2)
		md->plr.angle -= _PI * 2;
	md->plr.pos.x = aut->center.x + (aut->map_diag * 1) * cosf(md->plr.angle);
	md->plr.pos.y = aut->center.y + (aut->map_diag * 1) * sinf(md->plr.angle);
	delta.x = aut->center.x - md->plr.pos.x;
	delta.y = aut->center.y - md->plr.pos.y;
	md->cam.rot.z = 0;
	md->cam.rot.x = atan2f(delta.y, delta.x) * (180.0f / _PI);
	md->cam.rot.y = aut->base_y + sinf(md->plr.angle * spd) * amplitude;
	md->plr.dir.x = cosf(md->plr.angle);
	md->plr.dir.y = sinf(md->plr.angle);
	update_cam(md, &md->cam);
	if (!md->mouse.delta.y)
		return ;
	aut->base_y = minmaxf(40, 100, aut->base_y + md->mouse.delta.y * .01f);
}

int	handle_autocam_input(t_md *md, t_autocam *autocam, int key)
{
	unsigned char	c;

	if (key == Q_KEY)
		return (0);
	else if (key == ESC_KEY)
		free_and_quit(md, NULL, NULL);
	if (!autocam->quitting && (key == ENTER_KEY || md->mouse.click))
		autocam->quitting = 1;
	if (key == -1)
		return (1);
	c = get_input_char(md, key);
	if (key == DEL_KEY && md->plr_name_indx > 0)
		md->plr_name[--md->plr_name_indx] = '\0';
	else if (c && md->plr_name_indx < 15)
	{
		md->plr_name[md->plr_name_indx++] = c;
		md->plr_name[md->plr_name_indx] = '\0';
	}
	return (1);
}

int	update_autocam(t_md *md, t_autocam *autocam)
{
	if (md->timer.time <= 1)
		init_autocam(md, autocam);
	if (!handle_autocam_input(md, autocam, md->key_click))
		return (exit_autocam(md, autocam));
	else if (autocam->quitting && move_cam_to_start(md))
		return (exit_autocam(md, autocam));
	else if (!autocam->quitting)
		update_player_orbit(md, autocam);
	play_loop(md, &md->au.mus_pid, AU_MUS, 1);
	play_loop(md, &md->au.wind_pid, AU_WIND, autocam->quitting);
	render_background(md);
	cast_ray_threads_lp(md);
	render_sun(md);
	if (!autocam->quitting)
		render_autocam_text(md, 0);
	mlx_put_image_to_window(md->mlx, md->win, md->screen->img, 0, 0);
	render_cursor(md, NULL, 0);
	reset_mlx_values(md);
	return (1);
}
