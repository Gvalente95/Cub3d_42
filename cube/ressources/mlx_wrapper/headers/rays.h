/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:11:19 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/24 13:32:55 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYS_H
# define RAYS_H

# include "../mlx_utils.h"
# include <pthread.h>

# define MAX_RAY_SPRITE		70
# define MAX_RAYS 			3000

typedef struct s_md		t_md;
typedef struct s_ray	t_ray;

typedef struct s_portal_wall
{
	t_ent		*e;
	t_wrd_dir	dir;
	t_vec2		out;
}	t_portal_wall;

typedef struct s_portal
{
	t_portal_wall	ends[2];
	t_vec2			out_pos;
	t_ent			*found;
	int				last_shot_index;
}	t_portal;

typedef struct s_hit_data
{
	t_ent	*hit;
	t_vec3f	post_at_hit;
	float	dist_at_e;
	int		vertical_hit_at_e;
	int		row_index;
}	t_hit_data;

typedef struct s_ray_draw_d
{
	t_ray	*ray;
	t_image	*img;
	t_vec3	win_start;
	t_vec3	wp;
	int		y_start;
	int		y_end;
	t_vec3f	txd_crd;
	int		pixel;
	int		pass;
	int		has_portal;
	int		y_max;
	int		pxl_clr;
	int		pxl_i;
	int		win_y;
}	t_ray_draw_d;

typedef struct s_floor_draw_d
{
	t_ray	*ray;
	t_vec2	win;
	t_vec3f	plr;
	t_vec2f	dirl;
	t_vec2f	dirr;
	int		door_y_start;
	t_vec2f	flr_t;
	int		is_wmap;
	float	rwd;
	t_vec2f	stp;
	t_vec2f	flr;
	t_vec2	txp;
	int		clr;
	float	p;
	float	fogalpha;
}	t_floor_draw_d;

typedef struct s_ray
{
	t_hit_data	hit_data[MAX_RAY_SPRITE];
	int			hits_len;
	t_ent		*wall_hit;
	t_ent		*check_hit;
	t_ent		*door;
	t_vec3f		pos;
	t_vec3f		start;
	t_vec2		cord;
	t_vec2		step;
	t_vec3f		dir;
	t_vec2f		delta;
	t_vec2f		side_dist;
	float		dda_dist;
	float		distance;
	float		sprite_distance;
	float		angle;
	int			vertical_hit;
	int			corner;
	int			index;
	int			color;
	int			had_door;
	float		dist_at_door;
	int			is_floor_worker;
	int			active;
	int			steps;
	int			init_steps;
	int			teleported_once;
	t_vec2		wall_strip_pos;
}	t_ray;

#endif