/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 10:37:22 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/28 15:35:43 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	show_init_information(t_md *md)
{
	const char	active[2][20] = {PRED"Disabled"PRESET, PGREEN "Enabled" PRESET};

	printf("\n%sMAP\n%s%s%s\n", PYELLOW, PGREEN, md->map.name, PRESET);
	print_vec2(md->map.size, "Size");
	print_color(md->hud.floor_color, "Floor color");
	print_color(md->hud.ceiling_color, "Sky color");
	printf("\n%sPLAYER%s\n", PYELLOW, PRESET);
	print_vec3(md->plr.coord, "Coord");
	print_vec3f(md->plr.pos, "Position");
	print_vec3f(md->cam.rot, "Rotation");
	printf("\nAudio %s\n\n", active[md->prm.au_on]);
}

void	show_debug_time(t_md *md, t_txtd txt_data)
{
	txt_data.y -= md->prm.txt_sc * 1.5;
	rnd_fast_txt(md, txt_data, "g_time: %d", md->timer.time);
	txt_data.y -= md->prm.txt_sc * 1.5;
	rnd_fast_txt(md, txt_data, "delta:	%.3f", md->timer.delta_time);
}

void	show_update_information(t_md *md)
{
	t_vec2	p;

	p = v2(1, 1);
	draw_pixels(md->screen, _v2(0), div_v2(md->win_sz, 4), 0xAA000000);
	show_vec3f(md, "input", md->cam.input_mov, v2(p.x, p.y++));
	show_vec2(md, "input offst", md->cam.input_offst, v2(p.x, p.y++));
	show_vec3f(md, "mov", md->plr.mov, v2(p.x, p.y++));
	show_vec3f(md, "wrd_mv", md->cam.plr_wrd_mv, v2(p.x, p.y++));
	show_vec3f(md, "pos", md->plr.pos, v2(p.x, p.y++));
	show_vec3f(md, "dir", md->plr.dir, v2(p.x, p.y++));
	show_vec3f(md, "rot", md->cam.rot, v2(p.x, p.y++));
	show_float(md, "angle: ", md->plr.angle, v2(p.x, p.y++));
	show_vec3(md, "crd", md->plr.coord, v2(p.x, p.y++));
	show_vec3f(md, "mouse pos", md->mouse.pos, v2(p.x, p.y++));
	show_vec2(md, "mouse delta", md->mouse.delta_raw, v2(p.x, p.y++));
	show_vec2(md, "mouse scroll", md->mouse.scroll_raw, v2(p.x, p.y++));
	show_float(md, "fov", md->prm.fov, v2(p.x, p.y++));
	show_float(md, "cam z", md->cam.pos.z, v2(p.x, p.y++));
	show_int(md, "key click", md->last_key, v2(p.x, p.y++));
	if (md->cam.pointed)
		rnd_txt_simple(md, v2(p.x, p.y * (md->prm.txt_sc * 1.5f)), \
	"PTD_WALL: %s", md->txd.ents_types_names[md->cam.pointed->type]);
	if (md->cam.pointed_ent)
		rnd_txt_simple(md, v2(p.x, (p.y + 1) * (md->prm.txt_sc * 1.5f)), \
	"PTD_ENT: %s", md->txd.ents_types_names[md->cam.pointed_ent->type]);
}

void	print_color(int color, const char *label)
{
	const char	c[4][10] = {PRED, PGREEN, PBLUE, PYELLOW};

	if (label)
		printf("%s%-12s%s ", PBLUE, label, PRESET);
	printf("%s%-3d %s%-3d %s%-3d %s%d%s\n", \
		c[0], (color >> 16) & 0xFF, \
		c[1], (color >> 8) & 0xFF, \
		c[2], color & 0xFF, \
		c[3], (color >> 24) & 0xFF, PRESET);
}

void	show_debug(t_md *md, char *msg, int *value, char *attribute)
{
	const t_vec2	pos = div_v2(md->win_sz, 2);
	const t_txtd	td = (t_txtd){pos.x, pos.y, -1, md->prm.txt_sc, md->screen};

	if (value && attribute)
		rnd_fast_txt(md, td, "%s: %d %s", msg, *value, attribute);
	else if (value)
		rnd_fast_txt(md, td, "%s: %d", msg, *value);
	else if (attribute)
		rnd_fast_txt(md, td, "%s: %s", msg, attribute);
	else
		rnd_fast_txt(md, td, "%s", msg);
}
