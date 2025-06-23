/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_minimap_cmp.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 09:54:23 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/26 18:50:27 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	show_mmap_ent(t_md *md, t_ent *e, t_vec3f cosin, t_vec2 center)
{
	int		color;
	t_vec3f	relp;
	t_vec3f	rotp;
	t_vec3f	pos;
	int		dist;

	dist = hypotf(md->plr.pos.x - e->pos.x, md->plr.pos.y - e->pos.y);
	if (dist > cosin.z * md->t_len)
		return ;
	color = (md->rgb[e->type]);
	if (e->type == nt_mob)
		color = _RED;
	pos.z = minmaxf(0.0f, 1.0f, dist / (cosin.z * md->t_len));
	pos.z = 1.0f - powf(1.0f - pos.z, 3.0f);
	color = set_alpha(color, 1.0f - pos.z);
	relp = sub_vec3f(e->pos, md->plr.pos);
	if (e->type == nt_item || e->type == nt_mob)
		relp = add_vec3f(relp, v3f(md->t_len / 2));
	rotp.x = (relp.x * cosin.x - relp.y * cosin.y - md->cam.ofst.x / md->t_len);
	rotp.y = (relp.x * cosin.y + relp.y * cosin.x - md->cam.ofst.y / md->t_len);
	pos.x = center.x + ((rotp.x / md->t_len) * md->mmap.comps_scl);
	pos.y = center.y + ((rotp.y / md->t_len) * md->mmap.comps_scl);
	draw_pixels(md->screen, v2(pos.x + cosin.z, pos.y + cosin.z), \
		_v2(md->mmap.comps_scl), color);
}

static void	show_mmap_dir(t_md *md, t_vec3f cosin, t_vec2 psz, t_vec2 map_p)
{
	const t_vec2		relp[4] = {\
		(t_vec2){map_p.x + psz.x / 2, map_p.y}, \
		(t_vec2){map_p.x + psz.x / 2, map_p.y + psz.y}, \
		(t_vec2){map_p.x, map_p.y + psz.y / 2}, \
		(t_vec2){map_p.x + psz.x, map_p.y + psz.y / 2}};
	const char			*str[4] = {"N", "S", "W", "E"};
	t_vec3f				p;
	const t_vec2		cnt = v2(map_p.x + psz.x / 2, map_p.y + psz.y / 2);
	int					i;

	cosin.x *= 1.2;
	cosin.y *= 1.2;
	i = -1;
	while (++i < 4)
	{
		p.x = ((relp[i].x - cnt.x) * cosin.x - (relp[i].y - cnt.y) * cosin.y);
		p.y = ((relp[i].x - cnt.x) * cosin.y + (relp[i].y - cnt.y) * cosin.x);
		p.x += cnt.x;
		p.y += cnt.y;
		rnd_fast_txt(md, (t_txtd){p.x, p.y, -1, md->prm.txt_sc, NULL}, str[i]);
	}
}

void	show_ents(t_md *md, t_vec3f pdir, t_vec2 center)
{
	t_ent	*e;
	t_vec2	cord;

	cord = _v2(-1);
	while (++cord.y < md->map.size.y)
	{
		cord.x = -1;
		while (++cord.x < md->map.size.x)
		{
			e = get_mapped_at_cord(md, cord);
			if (e && e->type != nt_plr && e->type != nt_empty)
				show_mmap_ent(md, e, pdir, center);
		}
	}
	show_mmap_ent(md, &md->plr, pdir, center);
}

void	draw_dir_line(t_md *md, t_vec2 p, t_vec2 psz)
{
	const float	rot = (md->plr.angle + M_PI_2);
	t_vec2f		local_dir;
	t_vec2		true_cntr;
	int			color;

	local_dir = rotate_vec2f(get_v2f(md->plr.dir.x, md->plr.dir.y), rot);
	local_dir = scale_vec2f(local_dir, md->mmap.comps_scl * 2);
	true_cntr = v2(p.x + psz.x / 2 + md->mmap.comps_scl / 4, \
		p.y + psz.y / 2);
	color = _BLACK;
	color = set_alpha(color, .5);
	draw_line(md->screen, true_cntr, \
		add_vec2(true_cntr, \
			v2((int)local_dir.x, (int)local_dir.y)), \
			v2(color, 1));
}

void	show_cmps_mmap(t_md *md, t_vec2 center, int view_dist)
{
	t_vec3f	pdir;
	t_vec2	p;
	t_vec2	psz;

	p = v2(center.x - (view_dist - 1) * md->mmap.comps_scl, \
		center.y - (view_dist - 1) * md->mmap.comps_scl);
	psz = _v2((view_dist * 2) * md->mmap.comps_scl);
	draw_sphere(md->screen, p, psz, v3(_WHITE, 6, 1));
	pdir.z = md->plr.angle + M_PI_2;
	pdir.x = cosf(pdir.z);
	pdir.y = sinf(pdir.z);
	show_mmap_dir(md, pdir, psz, p);
	pdir.z = view_dist;
	show_ents(md, pdir, center);
	draw_dir_line(md, p, psz);
}
