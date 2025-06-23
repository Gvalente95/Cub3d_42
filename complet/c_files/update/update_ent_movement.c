/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_ent_movement.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:44:34 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/01 15:34:08 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	get_valid_moves(t_md *md, t_vec3 cord, \
	const t_vec2 *moves, int **val_mvs)
{
	int				i;
	t_vec2			new_cord;
	int				valid_indexes;
	t_ent			*ent_at_pos;

	valid_indexes = 0;
	i = -1;
	while (++i < 4)
	{
		new_cord = (t_vec2){cord.x + moves[i].x, cord.y + moves[i].y};
		ent_at_pos = get_mapped_at_cord(md, new_cord);
		if (ent_at_pos && ent_at_pos->type != nt_empty)
			continue ;
		(*val_mvs)[valid_indexes++] = i;
	}
	return (valid_indexes);
}

static void	set_ent_target_pos(t_md *md, t_ent *e)
{
	const t_vec2	moves[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
	int				*valid_moves;
	int				valid_amount;
	int				move_dir;
	t_vec2			new_tile;

	valid_moves = malloc(sizeof(int) * 4);
	valid_amount = get_valid_moves(md, e->coord, moves, &valid_moves);
	if (!valid_amount)
	{
		free(valid_moves);
		return ;
	}
	move_dir = valid_moves[r_range_seed(&md->r_seed, 0, valid_amount - 1)];
	free(valid_moves);
	new_tile = (t_vec2){e->coord.x + moves[move_dir].x,
		e->coord.y + moves[move_dir].y};
	e->target_pos = get_v3f(\
		new_tile.x * md->t_len + (md->t_len - e->size.x) * 0.5f, \
		new_tile.y * md->t_len + (md->t_len - e->size.y) * 0.5f, \
		0);
	e->action = m_walk;
}

void	move_ent_to_target(t_md *md, t_ent *e)
{
	t_vec3f			dir;
	t_vec2			new_cord;
	const t_vec3f	target = e->target_pos;

	e->action = m_walk;
	dir = normalize_vec3f(sub_vec3f(e->target_pos, e->pos));
	e->pos = add_vec3f(e->pos, scale_vec3f(dir, 5));
	new_cord = v2(e->pos.x / md->t_len, \
	e->pos.y / md->t_len);
	if (cmp_vec3f(e->pos, e->target_pos, 10))
	{
		e->pos = target;
		print_vec3(e->coord, "cur_cord");
		print_vec2(new_cord, "new cord");
		printf("\n\n");
		e->target_pos.x = -999;
		add_ent_at_cord(md, e, new_cord);
		e->action = m_idle;
	}
}

void	update_mob_actions(t_md *md, t_ent *e)
{
	t_ent_action	prev_action;

	prev_action = e->action;
	if (1)
	{
		if (e->target_pos.x != -999 && e->action == m_walk)
			move_ent_to_target(md, e);
		else if (md->timer.time % 100 == 0)
			set_ent_target_pos(md, e);
	}
	else
		e->action = m_idle;
	if (e->action != prev_action)
		e->frame_index = 0;
}
