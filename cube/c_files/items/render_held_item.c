/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_held_item.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 00:26:01 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 13:48:48 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static t_vec2	get_size(t_md *md, float elapsed, double throw_dur, int max)
{
	float			t;
	float			scale;
	const float		min = md->inv.held_sz.x * .2f;
	const t_vec2	max_size = _v2(max);

	if (!md->inv.held_used)
		return (max_size);
	t = fminf(elapsed / throw_dur, 1.0f);
	scale = 1.0f - t * (1.0f - min / (float)max);
	return (scale_vec2(max_size, scale));
}

static t_vec2	get_pos(t_md *md, t_vec2 sz, float t)
{
	t_vec2			pos;
	const t_mouse	mouse = md->mouse;
	t_vec2			arco;
	t_vec2			target;
	t_ent			*p_ent;

	if (!md->inv.held_set)
	{
		pos.x = md->win_sz.x / 2 - sz.x / 2;
		pos.y = md->win_sz.y / 2 - sz.y / 2;
		md->inv.pkbl_rot_z = 0;
		return (md->inv.held_set = 1, pos);
	}
	md->inv.pkbl_rot_z = minmaxf(-180, 180, \
		md->inv.pkbl_rot_z - mouse.delta.x * .05f);
	if (!md->inv.held_used || !md->cam.pointed_ent)
		return (add_vec2(md->inv.pkbl_p, v2(mouse.delta.x, mouse.delta.y)));
	arco = md->inv.arc_origin;
	p_ent = md->cam.pointed_ent;
	target = add_vec2(p_ent->screen_p, div_v2(p_ent->screen_sz, 8));
	pos.x = arco.x + (target.x - arco.x) * t;
	pos.y = arco.y + (target.y - arco.y) * t + -4.0f * 120.0f * t * (1.0f - t);
	return (pos);
}

static void	end_used_state(t_md *md, t_inventory *inv, t_ent *sel, t_ent *door)
{
	if (inv->held_i == Pokeball && sel && sel->type == nt_pokemon)
	{
		capture_pokemon(md, inv, sel);
		add_log_to_queue(md, _GREEN, "%s was caught", sel->label);
	}
	else if (inv->held_i == Pokeball)
		add_alert(md, 2, NULL, "Not a Pokemon");
	if (inv->held_i == Keys && door)
	{
		if (door->was_hit && md->cam.prv_door)
			door = md->cam.prv_door;
		if (door->was_hit)
			inv->items[Keys]++;
		door->was_hit = 1;
		door->hp = 0;
		play_sound(md, AU_OPEN);
		add_log_to_queue(md, _GREEN, "Door opened using Key");
	}
	inv->held_i = -1;
	inv->held_used = 0;
	inv->held_set = 0;
	free_image_data(md, inv->held_screen_img);
	inv->held_screen_img = NULL;
}

void	render_used(t_md *md, t_inventory *inv, double dur, int item_index)
{
	const float		elapsed = md->timer.cur_tm - inv->held_used_start;
	t_vec2			sz;
	t_vec2			pos;
	t_ent			*sel;

	sel = md->cam.pointed_ent;
	sz = get_size(md, elapsed, dur, inv->held_sz.x);
	if (!cmp_vec2(sz, inv->held_screen_img->size))
	{
		free_image_data(md, inv->held_screen_img);
		inv->held_screen_img = \
			copy_image(md, md->txd.item_txtr[item_index][0], sz, -1);
	}
	pos = get_pos(md, sz, fminf(elapsed / dur, 1.0f));
	inv->pkbl_p = pos;
	draw_rotated(md, inv->held_screen_img, md->screen, \
		(t_vec3f){(float)pos.x, pos.y, inv->pkbl_rot_z});
	render_used_shadow(md, pos, sz, dur);
	if (inv->held_used && sel && sel->type == nt_pokemon && elapsed > dur)
		sel->caught = 1;
	if (inv->held_used && (item_index == Keys || elapsed >= dur * 1.5))
		end_used_state(md, inv, sel, md->cam.pointed_door);
}

void	render_held_item(t_md *md, t_inventory *inv, int hand_index)
{
	t_image	*base;

	if (!inv->held_screen_img)
	{
		base = md->txd.item_txtr[hand_index][0];
		inv->held_screen_img = copy_image(md, base, inv->held_sz, -1);
	}
	render_used(md, inv, .5f, hand_index);
}
