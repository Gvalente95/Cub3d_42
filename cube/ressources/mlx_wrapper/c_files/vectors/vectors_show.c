/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors_show.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:03:13 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/06 20:35:19 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"
#include "../../../cube.h"

void	show_vec2(t_md *md, char *label, t_vec2 vec, t_vec2 pos)
{
	int	scale;

	scale = md->prm.txt_sc;
	rnd_fast_txt(md, \
	(t_txtd){pos.x * (scale * 1.5), pos.y * (scale * 1.5), \
		_WHITE, scale}, \
		"%-10s x%d y%d", \
		label, vec.x, vec.y);
}

void	show_vec3(t_md *md, char *label, t_vec3 vec, t_vec2 pos)
{
	int	scale;

	scale = md->prm.txt_sc;
	rnd_fast_txt(md, \
		(t_txtd){pos.x * (scale * 1.5), pos.y * (scale * 1.5), \
		_WHITE, scale}, \
		"%-10s x%d y%d z%d", \
		label, vec.x, vec.y, vec.z);
}

void	show_vec3f(t_md *md, char *label, t_vec3f vec, t_vec2 pos)
{
	int	scale;

	scale = md->prm.txt_sc;
	rnd_fast_txt(md, \
		(t_txtd){pos.x * (scale * 1.5), pos.y * (scale * 1.5), \
		_WHITE, scale}, \
		"%-10s x%.1f y%.1f z%.1f", \
		label, vec.x, vec.y, vec.z);
}

void	show_int(t_md *md, char *label, int value, t_vec2 pos)
{
	int	scale;

	scale = md->prm.txt_sc;
	rnd_fast_txt(md, \
		(t_txtd){pos.x * (scale * 1.5), pos.y * (scale * 1.5), \
		_WHITE, scale}, "%-10s %d", label, value);
}

void	show_float(t_md *md, char *label, float value, t_vec2 pos)
{
	int	scale;

	scale = md->prm.txt_sc;
	rnd_fast_txt(md, \
		(t_txtd){pos.x * (scale * 1.5), pos.y * (scale * 1.5), \
		_WHITE, scale}, "%-10s %-3f", label, value);
}
