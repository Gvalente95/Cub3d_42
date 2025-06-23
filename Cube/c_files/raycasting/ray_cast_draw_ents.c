/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_draw_ents.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 19:44:01 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/24 13:14:58 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	get_scale_and_pos(t_md *md, t_ent *e, t_vec2 win_sz, t_vec2 *draw_p)
{
	float		scale_factor;
	float		dist;
	int			scale;
	int			e_size;
	const float	pitch_offset = \
		tanf(md->cam.rot.y * (_PI / 180.0f)) * (win_sz.y / 2);

	e_size = max(md->t_len, e->size.y);
	scale_factor = e_size / md->txd.e_scales[e->type];
	dist = (maxf(0.1, e->hit_dist) / 2) * scale_factor;
	scale = minmaxf(5, win_sz.y * 2, (win_sz.y * e->size.y) / dist);
	draw_p->x = e->ray_hit_index - scale / 2;
	draw_p->y = (win_sz.y / 2 - pitch_offset - (scale * 0.4f) - \
		((md->plr.pos.z * .8f) * scale_factor));
	if (e->type == nt_pokemon)
	{
		draw_p->y -= ((float)((e->size.y * .8) - \
		(float)md->txd.feet_offsets[e->type]) / e->size.y) * scale;
		scale *= 1.5;
	}
	if (e->type == nt_item)
		draw_p->y += scale * (1.0f - (md->txd.e_scales[e->type] / md->t_len));
	return (scale);
}

void	draw_sprite_thread(t_md *md, t_ent *e, int had_door, float fogalpha)
{
	t_image		*img;
	t_vec2		sz;
	t_vec2		draw_p;
	const int	scale = get_scale_and_pos(md, e, md->win_sz, &draw_p);

	img = copy_image(md, e->frame, _v2(scale), -1);
	sz = img->size;
	if (!had_door)
		update_pointed(md, draw_p, v3(sz.x, sz.y, scale), e);
	if (e->caught)
		flush_img(img, _WHITE, 10, 1);
	else if (fogalpha < .95)
		flush_img(img, md->hud.fog_color, fogalpha, 1);
	draw_sphere(md->screen, v2(draw_p.x, draw_p.y + sz.y * .8), \
		v2(sz.x, sz.y * .25), v3(_BLACK, 3, 1));
	if (had_door)
		draw_alpha_img(img, md->screen, draw_p, .3f);
	else if (md->cam.pointed_ent == e && !e->caught)
		draw_img_contour(md, img, draw_p, (t_vec2){_MAGENT, 8});
	else
		draw_img(img, md->screen, draw_p, -1);
	e->screen_p = draw_p;
	e->screen_sz = img->size;
	free_image_data(md, img);
	e->in_screen = 1;
}

static void	sort_ent_list_by_distance(t_dblst **lst)
{
	t_dblst	*node;
	int		count;
	t_ent	**ent_array;
	t_dblst	*new_list;
	int		i;

	new_list = NULL;
	count = 0;
	count = dblst_size(*lst);
	ent_array = malloc(sizeof(t_ent *) * count);
	node = dblst_first(*lst);
	i = -1;
	while (++i < count)
	{
		ent_array[i] = (t_ent *)node->content;
		node = node->next;
	}
	qsort(ent_array, count, sizeof(t_ent *), \
		(int (*)(const void *, const void *))ent_sort_cmp);
	i = -1;
	while (++i < count)
		dblst_add_back(&new_list, dblst_new(ent_array[i]));
	dblst_clear(lst, NULL);
	*lst = new_list;
	free(ent_array);
}

void	draw_found_ents(t_md *md, t_thrd_manager *mon)
{
	t_dblst	*node;
	t_ent	*e;
	float	fogalpha;
	t_ent	*door;
	int		has_door;

	sort_ent_list_by_distance(&mon->ents_to_draw);
	node = dblst_first(mon->ents_to_draw);
	while (node)
	{
		e = (t_ent *)node->content;
		fogalpha = 1 - minmaxf(0, .95, (e->hit_dist / 1000.0f) * md->fx.fog);
		door = md->rays[e->ray_hit_index].door;
		has_door = (door && door->hp && door->cam_distance < e->cam_distance);
		draw_sprite_thread(md, e, has_door, fogalpha);
		node = node->next;
	}
	if (md->inv.held_i != Pokeball)
		update_pointed_ent(md);
	dblst_clear(&mon->ents_to_draw, NULL);
}
