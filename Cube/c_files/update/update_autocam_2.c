/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_autocam_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:31:03 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/23 18:10:07 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	init_fade_intro(t_md *md, double duration)
{
	float	alpha;
	double	elapsed;

	elapsed = get_time_in_seconds() - md->timer.game_start;
	render_background(md);
	if (elapsed > duration)
	{
		md->autocam.fade = 0;
		return ;
	}
	alpha = 1.0f - (elapsed / duration);
	draw_alpha_img(md->hud.overlay, md->screen, _v2(0), alpha);
}

static void	render_autocam_subtext(t_md *md, t_vec2 base_p, t_txtd d, float t)
{
	t_vec2		name_pos;
	const int	name_len = ft_strlen(md->plr_name);

	d.scale = 15;
	d.x = base_p.x + 200;
	d.y = base_p.y + 80 * 1.3 + sinf(t) * 10;
	rnd_fast_txt(md, d, "By Gvalente");
	d.x = base_p.x - 50;
	d.scale = 20;
	d.y += sinf(t) * 5;
	d.y += 20;
	rnd_fast_txt(md, d, "Press [ENTER] to start");
	name_pos = div_v2(md->win_sz, 2);
	d.x = name_pos.x - (11 * d.scale) / 2;
	d.y = name_pos.y;
	rnd_fast_txt(md, d, "PLAYER NAME");
	d.color = md->rgb[RGB_GREEN - (name_len == 0)];
	d.x = name_pos.x - (name_len * d.scale) / 2;
	d.y += d.scale * 2;
	if (name_len < 1)
		rnd_fast_txt(md, d, "Cube243");
	else
		rnd_fast_txt(md, d, md->plr_name);
}

void	render_autocam_text(t_md *md, float t)
{
	const char	*txt[] = {"C", "U", "B", "E", " ", "3", "D"};
	const int	scale = 80;
	t_vec2		base_p;
	t_txtd		d;
	int			i;

	base_p = (t_vec2){md->win_sz.x / 2 - (scale * 2), scale * 1.5};
	d = (t_txtd){base_p.x, base_p.y, md->hud.fog_color, scale, md->screen};
	i = -1;
	while (++i < 7)
	{
		d.scale = scale;
		if (i >= 4)
			d.scale = 30;
		if (v2_touch((t_vec2){d.x, d.y}, _v2(scale), md->mouse.real, _v2(4)))
			d.scale += 10;
		d.color = -1;
		if (i % 2 != 0)
			d.color = md->hud.fog_color;
		t = (md->timer.time++) * 0.01f + i * 0.5f;
		d.y = base_p.y + sinf(t) * 10 + (i >= 4) * 30;
		rnd_fast_txt(md, d, txt[i]);
		d.x += d.scale;
	}
	render_autocam_subtext(md, base_p, d, t);
}

int	move_cam_to_start(t_md *md)
{
	const float		acspd = md->autocam.acc_spd;
	const float		mv_spd = .05f + acspd;
	const t_vec2f	rot_spd = (t_vec2f){0.01f + acspd, 0.01f + acspd * .5f};
	const t_vec3f	diff = sub_vec3f(md->plr.pos, md->plr.start_pos);

	md->prm.alternate_draw = 0;
	md->fx.fog = .1f;
	md->autocam.acc_spd += 0.003f;
	if (fabsf(md->plr.pos.x - md->plr.start_pos.x) > EPSILON)
		md->plr.pos.x += (md->plr.start_pos.x - md->plr.pos.x) * mv_spd;
	if (fabsf(md->plr.pos.y - md->plr.start_pos.y) > EPSILON)
		md->plr.pos.y += (md->plr.start_pos.y - md->plr.pos.y) * mv_spd;
	if (fabsf(md->plr.pos.z - md->plr.start_pos.z) > EPSILON)
		md->plr.pos.z += (md->plr.start_pos.z - md->plr.pos.z) * mv_spd;
	if (fabsf(md->cam.rot.x - md->cam.x_dir_start) > EPSILON)
		md->cam.rot.x += (md->cam.x_dir_start - md->cam.rot.x) * rot_spd.x;
	if (md->cam.rot.y > 0)
		md->cam.rot.y -= md->cam.rot.y * rot_spd.y;
	md->plr.angle = md->cam.rot.x * (_PI / 180.0f);
	md->plr.dir = (t_vec3f){cosf(md->plr.angle), sinf(md->plr.angle), 0};
	md->hud.floor_start = md->win_sz.y / 2 - (md->cam.rot.y * 8) + 1;
	render_background(md);
	update_cam(md, &md->cam);
	return (fabsf(diff.x) < .1 && fabsf(diff.y) < .1 && fabsf(diff.z) < .1);
}
