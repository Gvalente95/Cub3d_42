/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_ents.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 22:16:05 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/07 22:47:14 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	add_ent_at_cord(t_md *md, t_ent *e, t_vec2 new_cord)
{
	t_vec2			prv_crd;
	t_ent			*prv;
	const t_vec2	limit = _v2(MAPPED_ENT_MAX);

	prv_crd = (t_vec2){e->coord.x, e->coord.y};
	prv = get_mapped_at_cord(md, prv_crd);
	if (prv && prv == e)
		remove_ent(md, prv);
	e->is_active = 1;
	if (new_cord.x >= 0 && new_cord.x <= limit.x && \
	new_cord.y >= 0 && new_cord.y <= limit.y)
		md->map_ents[new_cord.x][new_cord.y] = e;
	e->coord = v3(new_cord.x, new_cord.y, e->coord.z);
	return (1);
}

void	init_mapped_ent(t_md *md)
{
	t_vec2		cord;

	cord = _v2(-1);
	while (++cord.y < MAPPED_ENT_MAX)
	{
		cord.x = -1;
		while (++cord.x < MAPPED_ENT_MAX)
			md->map_ents[cord.x][cord.y] = NULL;
	}
}

t_ent	*get_mapped_at_cord(t_md *md, t_vec2 cord)
{
	t_ent	*found;

	if (cord.x < 0 || cord.x > md->map.size.x + 1 || \
		cord.y < 0 || cord.y > md->map.size.y + 1)
		return (NULL);
	found = md->map_ents[cord.x][cord.y];
	return (found);
}

t_ent	*get_mapped_at_pos(t_md *md, t_vec2f pos)
{
	t_vec2	cord;

	cord = v2((int)pos.x / md->t_len, (int)pos.y / md->t_len);
	return (get_mapped_at_cord(md, cord));
}

int	remove_ent(t_md *md, t_ent *e)
{
	if (!e)
		return (0);
	if (e->coord.x < 0 || e->coord.x > MAPPED_ENT_MAX || \
		e->coord.y < 0 || e->coord.y > MAPPED_ENT_MAX)
		return (0);
	if (md->cam.pointed == e)
		md->cam.pointed = NULL;
	if (md->cam.prv_pointed == e)
		md->cam.prv_pointed = NULL;
	if (md->cam.pointed_door == e)
		md->cam.pointed_door = NULL;
	if (md->cam.pointed_ent == e)
		md->cam.pointed_ent = NULL;
	if (md->cam.prv_pointed_ent == e)
		md->cam.prv_pointed_ent = NULL;
	if (md->cam.prv_door == e)
		md->cam.prv_door = NULL;
	md->map_ents[e->coord.x][e->coord.y] = NULL;
	return (1);
}
