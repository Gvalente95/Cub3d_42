/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_fes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 15:30:20 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/31 23:35:03 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

t_fe_type	get_random_fe_type(void)
{
	const int	distr[4] = {[fe_grass] = 400, [fe_tree] = 2, [fe_bush] = 1};
	int			total;
	int			r;
	int			i;
	int			acc;

	total = 0;
	i = -1;
	while (++i < fe_type_len)
		total += distr[i];
	r = r_range(0, total - 1);
	acc = 0;
	i = -1;
	while (++i < fe_type_len)
	{
		acc += distr[i];
		if (r < acc)
			return (i);
	}
	return (fe_grass);
}

void	init_fe(t_md *md, t_fe *fe)
{
	fe->active = r_range(0, FE_PER_TILE) == 0;
	if (!fe->active)
		return ;
	fe->type = fe_grass;
	fe->cut_len = 0;
	fe->base_color = \
		v4_to_color(r_range(0, 20), r_range(120, 200), r_range(0, 40), 255);
	fe->growth_factor = f_range(0.99, 1.01);
	fe->size = v2(r_range(14, 15), 1);
	fe->height_max = r_range(5, 7);
	fe->was_drawn = 0;
	if (fe->type != fe_grass)
	{
		fe->height_max = md->prm.grass_sz.y * 2;
		fe->base_color = _TURQ;
		fe->growth_factor = f_range(.3, .6);
		fe->size = v2(10, 5);
	}
	fe->age = 0;
	fe->height = fe->size.y;
}

int	free_env(t_md *md, t_env_manager *env)
{
	const t_vec2	mapsz = md->map.size;
	t_vec2			map;
	t_vec2			cord;
	int				free_count;

	free_count = free_image_data(md, env->grass_overlay);
	map.y = -1;
	while (++map.y < mapsz.y)
	{
		map.x = -1;
		while (++map.x < mapsz.x)
		{
			if (!env->grass[map.y][map.x])
				continue ;
			cord.y = -1;
			while (++cord.y < md->t_len)
				free(env->grass[map.y][map.x][cord.y]);
			free(env->grass[map.y][map.x]);
		}
		free(env->grass[map.y]);
	}
	free(env->grass);
	free_count = (map.x * md->t_len) * map.y + 2;
	printf("ENV %-5s%d%s\n", PGREEN, free_count, PRESET);
	return (free_count);
}

void	init_fes(t_md *md, t_env_manager *env, int tlen)
{
	t_vec2			map;
	t_vec2			cord;

	env->grass_overlay = init_img(md, md->win_sz, NULL, _NULL);
	env->grass = md_malloc(md, sizeof(t_fe ***) * md->map.size.y);
	map.y = -1;
	while (++map.y < md->map.size.y)
	{
		env->grass[map.y] = md_malloc(md, sizeof(t_fe **) * md->map.size.x);
		map.x = -1;
		while (++map.x < md->map.size.x)
		{
			cord.y = -1;
			if (md->map.buffer[map.x + map.y * (md->map.size.y + 1)] != 'g')
			{
				env->grass[map.y][map.x] = NULL;
				continue ;
			}
			env->grass[map.y][map.x] = md_malloc(md, sizeof(t_fe *) * tlen);
			while (++cord.y < tlen)
			{
				env->grass[map.y][map.x][cord.y] = \
					md_malloc(md, sizeof(t_fe) * tlen);
				cord.x = -1;
				while (++cord.x < tlen)
					init_fe(md, &env->grass[map.y][map.x][cord.y][cord.x]);
			}
		}
	}
}
