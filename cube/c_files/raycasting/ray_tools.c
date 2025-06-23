/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 02:01:00 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/26 18:31:02 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	draw_blood(t_md *md, t_image *img, t_vec2 pos, int color)
{
	int		radius;
	t_vec3	sphere_data;

	(void)md;
	radius = (img->size.x / 6);
	sphere_data = v3(color, 10, 0);
	draw_sphere(img, pos, _v2(radius), sphere_data);
}

t_vec2	get_2d_ray_pos(t_md *md)
{
	t_vec2	cam_2d_ofs;
	t_vec2	center_ofs;
	t_vec2	centr;

	cam_2d_ofs.x = md->win_sz.x / 2 - md->txd.size_2d * 2;
	cam_2d_ofs.y = md->win_sz.y / 2 - md->txd.size_2d * 2;
	center_ofs.x = (md->cam.ofst.x / md->t_len * md->txd.size_2d);
	center_ofs.y = ((md->cam.ofst.y / md->t_len) * md->txd.size_2d);
	centr = v2(cam_2d_ofs.x - center_ofs.x, cam_2d_ofs.y - center_ofs.y);
	return (centr);
}

int	render_ray(t_md *md, t_ray *ray, t_vec2 visu_offset)
{
	t_vec2	_2d_pos;

	if (!md->prm.view_2d || !md->prm.show_rays)
		return (0);
	_2d_pos.x = visu_offset.x + (ray->pos.x / md->t_len) * md->txd.size_2d;
	_2d_pos.y = visu_offset.y + (ray->pos.y / md->t_len) * md->txd.size_2d;
	if (!is_in_screen(md, v2_to_v3(_2d_pos), v2(1, 1)))
		return (0);
	draw_pixel(md->screen, _2d_pos, ray->color, .3);
	return (1);
}

void	init_base_ray(t_ray *ray, int index, t_vec3f start_pos, float distance)
{
	ray->index = index;
	ray->check_hit = NULL;
	ray->had_door = 0;
	ray->init_steps = 0;
	ray->color = _BLACK;
	ray->steps = -1;
	ray->door = NULL;
	ray->start = start_pos;
	ray->delta = v2f(0);
	ray->side_dist = v2f(0);
	ray->pos = start_pos;
	ray->step = _v2(0);
	ray->dda_dist = 0;
	ray->distance = distance;
	ray->vertical_hit = 0;
	ray->wall_strip_pos = _v2(-1);
	ray->teleported_once = 0;
}

int	validate_check_hit(t_md *md, t_ray *ray, t_ent *ent, t_ent_type tp)
{
	const int	is_wall = tp == nt_door || tp == nt_wall || tp == nt_ext_wall;

	if (tp == nt_empty || tp == nt_plr || tp == nt_grass)
		return (0);
	if (!md->prm.ent_mode && !is_wall)
		return (0);
	if (ray->hits_len > 0 && \
		ray->hit_data[ray->hits_len - 1].hit == ent && !md->prm.super_view)
		return (0);
	if (ray->check_hit && ray->check_hit != ent)
		return (0);
	if (is_wall)
		return ((v3f_bounds(ray->pos, v3f(0), \
			ent->pos, get_v3f(ent->frame->size.x + 1, ent->frame->size.y, 0))));
	else if (!ent->is_active)
		return (0);
	if (is_in_list(md->thrd_manager.ents_to_draw, ent))
		return (0);
	if (!cmp_vec2f((t_vec2f){ray->pos.x, ray->pos.y}, \
	(t_vec2f){ent->pos.x + ent->size.x / 2, ent->pos.y + ent->size.y / 2}, 1))
		return (0);
	dblst_add_back(&md->thrd_manager.ents_to_draw, dblst_new((t_ent *)ent));
	return (ent->hit_dist = ray->steps, ent->ray_hit_index = ray->index, 0);
}
