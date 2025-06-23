/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 10:37:22 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/26 19:49:20 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	show_init_information(t_md *md)
{
	t_vec2	pos;
	t_ent	*e;

	print_vec2(md->map.size, "map size");
	print_vec3f(md->plr.pos, "plr pos");
	print_vec3f(md->cam.rot, "plr rot");
	printf("AU: %d\n", md->prm.au_on);
	print_vec3(md->plr.coord, "plr coord");
	printf("map name: %s\nmap content: \n", md->map.name);
	return ;
	pos = _v2(-1);
	while (++pos.y < md->map.size.y)
	{
		pos.x = -1;
		while (++pos.x < md->map.size.x)
		{
			e = get_mapped_at_cord(md, pos);
			if (e)
				printf("%c", e->character);
			else
				printf(" ");
		}
		printf("\n");
	}
	printf("\n");
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
	show_float(md, "cam z", md->cam.pos.z, v2(p.x, p.y++));
	show_int(md, "key click", md->last_key, v2(p.x, p.y++));
	show_int(md, "plr in house", md->plr_in_house, v2(p.x, p.y++));
	if (md->cam.pointed)
		rnd_txt_simple(md, v2(p.x, p.y * (md->prm.txt_sc * 1.5f)), \
	"PTD_WALL: %s", md->txd.ents_types_names[md->cam.pointed->type]);
	if (md->cam.pointed_ent)
		rnd_txt_simple(md, v2(p.x, (p.y + 1) * (md->prm.txt_sc * 1.5f)), \
	"PTD_ENT: %s", md->txd.ents_types_names[md->cam.pointed_ent->type]);
}

void	print_color(int color, const char *label)
{
	if (label)
		printf("%s: ", label);
	printf("r%d g%d b%d a%d\n", \
		(color >> 16) & 0xFF, \
		(color >> 8) & 0xFF, \
		color & 0xFF, \
		(color >> 24) & 0xFF);
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
