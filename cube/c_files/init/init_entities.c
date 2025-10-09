/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 00:11:00 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/09 13:46:57 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	set_type_specifics(t_md *md, t_ent *e, t_ent_type type)
{
	int	i;

	e->hasCollision = \
		type != nt_tree && \
		type != nt_item && \
		type != nt_bush && \
		type != nt_empty && \
		type != nt_grass && \
		type != nt_plr;

	if (type == nt_wall)
	{
		e->crp_pxl = v2(\
			r_range(0, md->win_sz.x), \
			r_range(0, md->win_sz.y));
		e->frames = malloc(sizeof(t_image) * 5);
		i = -1;
		while (++i < 4)
			e->frames[i] = copy_image(md, md->txd.wall_img[i], v2(-1, -1), -1);
		e->frames[4] = NULL;
	}
	else
		e->dir = get_v3f(r_range(-1, 1), r_range(-1, 1), r_range(-1, 1));
	if (type == nt_mob) {
		e->is_trainer = true;
		init_ent_pkteam(md, e, r_range_seed(&md->r_seed, 1, 6));
	}
	else if (e->type == nt_pokemon)
	{
		e->elemType = getPkElement(md, e->mob_type);
		e->pkStatus.active = 0;
		e->max_hp = r_range(70, 140);
		e->hp = e->max_hp;
		e->poke_moves = malloc(sizeof(t_move) * 4);
		t_move *moveSet = md->pkd.PokemonMoves[e->mob_type];
		for (int i = 0; i < 4; i++) {
			e->poke_moves[i] = moveSet[i];
			e->poke_moves[i].cc = moveSet[i].ccMax;
		}
	}
}

static void	set_ent_values(t_md *md, t_ent *e, char c, t_vec2 cord)
{
	e->is_trainer = false;
	e->is_active = true;
	e->revealed = false;
	e->poke_moves = NULL;
	e->anim = NULL;
	e->frames = NULL;
	e->overlay = NULL;
	e->type = minmax(0, ENT_TYPE_LEN - 1, \
		get_char_index(md->txd.ents_tp_map[0], c));
	init_ent_frames(md, &md->txd, e);
	e->size = e->frame->size;
	e->pos.x = (cord.x * md->t_len) + (md->t_len - e->size.x) * 0.5f;
	e->pos.y = (cord.y * md->t_len) + (md->t_len - e->size.y) * 0.5f;
	e->pos.z = 0;
	e->target_pos = e->pos;
	e->transitionOffset = v3f(0);
	e->start_pos = e->pos;
	if (c >= '3' && c <= '9')
	{
		e->pos.z = -md->t_len * (450 * ((c - '0') - 2));
		e->coord.z = -(c - '0') / 2;
	}
	e->mov = v3f(0);
	e->dir = v3f(0);
	e->hp = 5;
}

static void	init_player(t_md *md, char c, t_vec2 cord, int map_index)
{
	char	base_c;
	t_ent	*e;

	e = &md->plr;
	base_c = c;
	c = '*';
	set_ent_values(md, &md->plr, c, cord);
	e->map_index = map_index;
	md->cam.rot.x = -90;
	if (base_c == 'S')
		md->cam.rot.x = 90;
	if (base_c == 'E')
		md->cam.rot.x = 0;
	else if (base_c == 'W')
		md->cam.rot.x = 180;
	md->cam.x_dir_start = md->cam.rot.x;
	e->angle = md->cam.rot.x * (_PI / 180.0f);
	e->size = v2(md->t_len / 2, md->t_len / 2);
	e->pos.z = 0;
	e->pos.x += md->t_len / 2;
	e->start_pos.x = e->pos.x;
	md->cam.pos = e->pos;
	add_ent_at_cord(md, e, cord);
}

t_ent	*init_ent(t_md *md, char c, t_vec2 cord, int map_index)
{
	t_ent	*e;

	e = malloc(sizeof(t_ent));
	memset(e, 0, sizeof(t_ent));
	set_ent_values(md, e, c, cord);
	e->cam_distance = 99999;
	e->map_index = map_index;
	e->screen_p = _v2(-1);
	e->screen_sz = _v2(-1);
	e->target_pos = v3f(0);
	e->pk_team = NULL;
	set_type_specifics(md, e, e->type);
	if (map_index != -1)
		add_ent_at_cord(md, e, cord);
	return (e);
}

void	init_entities(t_md *md, t_vec2 pos)
{
	int			i;
	t_dblst		*ents;
	char		c;

	init_mapped_ent(md);
	ents = NULL;
	pos = v2(0, 0);
	i = -1;
	while (md->map.buffer[++i])
	{
		c = md->map.buffer[i];
		if (c == '\n')
			pos = (t_vec2){-1, pos.y + 1};
		else if (char_in_str(c, "NSEW"))
			init_player(md, c, pos, i);
		else if (!char_in_str(c, " \n"))
			dblst_add_back(&ents, dblst_new((void *)init_ent(md, c, pos, i)));
		pos.x++;
	}
	md->entities = dblst_first(ents);
	md->mmap.revealed_len = dblst_size(md->entities);
}
