/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal_raycast.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 01:58:28 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/16 15:33:56 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	dir_to_rotation(t_wrd_dir a, t_wrd_dir b)
{
	if (a == b)
		return (2);
	if ((a == NORTH && b == SOUTH) || (a == SOUTH && b == NORTH) || \
	(a == EAST && b == WEST) || (a == WEST && b == EAST))
		return (0);
	if ((a == NORTH && b == EAST) || (a == EAST && b == SOUTH) || \
		(a == SOUTH && b == WEST) || (a == WEST && b == NORTH))
		return (1);
	return (3);
}

void	rotate_90_deg(t_vec3f *vec, int times)
{
	float	x;
	float	y;

	times = (times % 4 + 4) % 4;
	x = vec->x;
	y = vec->y;
	if (times == 1)
	{
		vec->x = y;
		vec->y = -x;
	}
	else if (times == 2)
	{
		vec->x = -x;
		vec->y = -y;
	}
	else if (times == 3)
	{
		vec->x = -y;
		vec->y = x;
	}
}

int	get_portal_data_index(t_md *md, t_ray *ray, t_ent *portal_data)
{
	if (ray->teleported_once > 2)
		return (-1);
	if (!md->portal_data.ends[0].e || !md->portal_data.ends[1].e)
		return (-1);
	if (portal_data == md->portal_data.ends[0].e)
		return (0);
	if (portal_data == md->portal_data.ends[1].e)
		return (1);
	return (-1);
}

int	translate_ray(t_md *md, t_ray *ray, t_ent *portal_data, int view_index)
{
	t_wrd_dir	src_dir;
	t_wrd_dir	dst_dir;
	t_vec2		out_pos;
	int			rot_offset;

	(void)portal_data;
	src_dir = md->portal_data.ends[view_index].dir;
	dst_dir = md->portal_data.ends[!view_index].dir;
	out_pos = md->portal_data.ends[!view_index].out;
	rot_offset = dir_to_rotation(src_dir, dst_dir);
	rotate_90_deg(&ray->dir, rot_offset);
	ray->pos.x = out_pos.x;
	ray->pos.y = out_pos.y;
	init_base_ray(ray, ray->index, ray->pos, ray->distance);
	ray->teleported_once++;
	cast_ray(md, ray, get_2d_ray_pos(md));
	return (1);
}
