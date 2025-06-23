/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions_portal.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:52:49 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/23 18:10:07 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	dir_to_angle(t_wrd_dir dir)
{
	if (dir == NORTH)
		return (0);
	if (dir == EAST)
		return (90);
	if (dir == SOUTH)
		return (180);
	if (dir == WEST)
		return (270);
	return (0);
}

int	get_portal_angle_offset(t_wrd_dir start_dir, t_wrd_dir end_dir)
{
	int	start_angle;
	int	end_angle;
	int	angle_diff;

	if (start_dir == end_dir)
		return (180);
	if (start_dir == NORTH && end_dir == SOUTH)
		return (0);
	if (start_dir == SOUTH && end_dir == NORTH)
		return (0);
	if (start_dir == EAST && end_dir == WEST)
		return (0);
	if (start_dir == WEST && end_dir == EAST)
		return (0);
	start_angle = dir_to_angle(start_dir);
	end_angle = dir_to_angle(end_dir);
	angle_diff = start_angle - end_angle;
	if (angle_diff > 180)
		angle_diff -= 360;
	if (angle_diff < -180)
		angle_diff += 360;
	return (angle_diff);
}

static void	set_pos_and_angle(t_md *md, t_wrd_dir start_dir, \
	t_wrd_dir end_dir, t_vec2 out_pos)
{
	t_ent	*plr;
	int		angle_diff;
	int		new_angle;

	plr = &md->plr;
	angle_diff = get_portal_angle_offset(start_dir, end_dir);
	new_angle = md->cam.rot.x + angle_diff;
	if (new_angle < 180.0)
		new_angle += 360;
	if (new_angle > 180)
		new_angle -= 360;
	md->cam.rot.x = new_angle;
	plr->angle = (new_angle) * (_PI / 180.0f);
	plr->dir.x = cosf(plr->angle);
	plr->dir.y = sinf(plr->angle);
	plr->pos.x = out_pos.x;
	plr->pos.y = out_pos.y;
}

static int	pass_through(t_md *md, int cur_index, t_vec2 in_pos, t_vec2 out_pos)
{
	t_wrd_dir	start_dir;
	t_wrd_dir	end_dir;
	t_vec2		out_p;

	start_dir = md->portal.ends[cur_index].dir;
	end_dir = md->portal.ends[!cur_index].dir;
	if (start_dir == EAST && md->plr.pos.x + md->plr.size.x < in_pos.x)
		return (0);
	else if (start_dir == NORTH && md->plr.pos.y > in_pos.y)
		return (0);
	else if (start_dir == SOUTH && md->plr.pos.y + md->plr.size.y < in_pos.y)
		return (0);
	else if (start_dir == WEST && md->plr.pos.x > in_pos.x)
		return (0);
	if (md->timer.no_portal_cooldown > md->timer.cur_tm)
		return (0);
	printf("MADE IT THROUGH\n");
	out_p = out_pos;
	if (start_dir == EAST && end_dir == NORTH)
		out_p.x += in_pos.y - md->plr.pos.y;
	set_pos_and_angle(md, start_dir, end_dir, out_p);
	md->timer.no_portal_cooldown = md->timer.cur_tm + 1;
	return (0);
}

int	validate_portal_collision(t_md *md, t_ent *b)
{
	t_ent	*portal_end;
	int		cur_index;
	t_vec2	in_pos;
	t_vec2	out_pos;

	portal_end = NULL;
	cur_index = -1;
	while (++cur_index < 2)
	{
		if (md->portal.ends[cur_index].e != b)
			continue ;
		portal_end = md->portal.ends[!cur_index].e;
		in_pos = md->portal.ends[cur_index].out;
		out_pos = md->portal.ends[!cur_index].out;
		break ;
	}
	if (!portal_end)
		return (1);
	return (pass_through(md, cur_index, in_pos, out_pos));
}
