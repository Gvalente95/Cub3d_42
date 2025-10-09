/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_ent_movement.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:44:34 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/09 10:18:40 by giuliovalen      ###   ########.fr       */
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
		new_cord = (t_vec2){ cord.x + moves[i].x, cord.y + moves[i].y };
		if (new_cord.x < 0 || new_cord.x > md->map.size.x - 1 || \
			new_cord.y < 0 || new_cord.y > md->map.size.y - 1)
			continue;
		ent_at_pos = get_mapped_at_cord(md, new_cord);
		if (ent_at_pos && ent_at_pos->type != nt_empty)
			continue ;
		(*val_mvs)[valid_indexes++] = i;
	}
	return (valid_indexes);
}

void	set_ent_target_pos(t_md *md, t_ent *e)
{
	const t_vec2	moves[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
	int				*valid_moves;
	int				valid_amount;
	int				move_dir;

	valid_moves = malloc(sizeof(int) * 4);
	valid_amount = get_valid_moves(md, e->coord, moves, &valid_moves);
	if (!valid_amount)
	{
		free(valid_moves);
		return ;
	}
	move_dir = valid_moves[r_range_seed(&md->r_seed, 0, valid_amount - 1)];
	free(valid_moves);
	e->target_pos = (t_vec3f){e->pos.x + moves[move_dir].x * md->t_len,
		e->pos.y + moves[move_dir].y * md->t_len, e->pos.z };
	e->has_reached_coord = 0;
	e->action = m_walk;
}

void	move_ent_to_target(t_md *md, t_ent *e)
{
	t_vec3f			dir;
	t_vec2			curCoord;
	const float		speed = md->res * .1;
	const t_vec3f	target = e->target_pos;

	dir = normalize_vec3f(sub_vec3f(e->target_pos, e->pos));
	e->transitionOffset.x += dir.x * speed;
	e->transitionOffset.y += dir.y * speed;

	t_vec2f off = get_v2f(fmodf(e->pos.x + e->transitionOffset.x, md->t_len), fmodf(e->pos.y + e->transitionOffset.y, md->t_len));
	t_vec2f edgeDist = get_v2f(fminf(off.x, md->t_len - off.x), fminf(off.y, md->t_len - off.y));
	if (edgeDist.x <= 3 || edgeDist.y <= 3)
	{
		printf("e moved from (x%fy%f) to (x%fy%fy) at %f speed\n", e->pos.x, e->pos.y,
			target.x, target.y, speed);
		e->pos = target;
		curCoord = (t_vec2){ e->pos.x / md->t_len, e->pos.y / md->t_len};
		add_ent_at_cord(md, e, curCoord);
		e->has_reached_coord = 1;
		e->action = m_idle;
		print_vec3f(e->transitionOffset, "move offset");
		print_vec3f(dir, "dir");
		e->target_pos = v3f(0);
		e->transitionOffset = v3f(0);
	}
}

void	update_mob_actions(t_md *md, t_ent *e)
{
	t_ent_action	prev_action;

	prev_action = e->action;
	if (e->action == m_walk)
		move_ent_to_target(md, e);
	if (e->action != prev_action)
		e->frame_index = 0;
}
