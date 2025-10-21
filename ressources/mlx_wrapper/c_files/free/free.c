/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:14:54 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/15 00:21:44 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../cube.h"

int	safe_free(void *item)
{
	if (item)
	{
		free(item);
		return (1);
	}
	return (0);
}

int	free_void_array(void **elements)
{
	int	i;
	int	free_count;

	free_count = 0;
	if (!elements)
		return (printf("tried to free null void **\n"), 0);
	i = 0;
	while (elements[i])
	{
		free(elements[i]);
		elements[i] = NULL;
		free_count++;
		i++;
	}
	free(elements);
	return (free_count + 1);
}

int	free_void(void *elem)
{
	if (!elem)
		return (0);
	free(elem);
	elem = NULL;
	return (1);
}

int	free_md(t_md *md, int quit)
{
	int	fa;

	printf("%sFREE%s\n", PGREEN, PRESET);
	fa = 0;
	fa += free_hud(md, &md->hud);
	fa += free_var(md, &md->mmap, &md->fx, &md->mouse);
	fa += free_menu(md, &md->menu);
	fa += free_inv(md, &md->inv);
	if (md->init_steps > 1)
		fa += free_env(md, &md->env);
	fa += free_ents(md);
	fa += free_BA_data(md, &md->BA_d);
	fa += free_txd(md, &md->txd, -1);
	fa += free_void(md->mlx);
	fa += free_ray_data(md);
	printf("Total: %s%d elements freed%s\n", PGREEN, fa, PRESET);
	return (fa);
}

int	free_and_quit(t_md *md, const char *msg, const char *attribute)
{
	if (msg)
	{
		printf("%sError%s\n%s", PRED, PRESET, msg);
		if (attribute)
			printf(": %s", attribute);
		printf("\n");
	}
	printf("%sExit after %s%.2fs\n", \
		PBLUE, PRESET, md->timer.cur_tm - md->timer.game_start);
	cleanup_thread_pool(md);
	stop_sound(md->au.wind_pid);
	stop_sound(md->au.mus_pid);
	free_md(md, 1);
	exit(0);
	return (1);
}
