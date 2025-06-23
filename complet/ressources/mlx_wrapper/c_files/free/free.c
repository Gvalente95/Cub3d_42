/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:14:54 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/01 14:37:01 by gvalente         ###   ########.fr       */
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

	fa = 0;
	if (!md->init_steps)
		return (0);
	fa += free_hud(md, &md->hud);
	fa += free_var(md, &md->mmap, &md->fx, &md->mouse);
	fa += free_menu(md, &md->menu);
	fa += free_inv(md, &md->inv);
	fa += free_env(md, &md->env);
	if (md->init_steps < 2)
		return (mlx_destroy_window(md->mlx, md->win), \
		mlx_destroy_display(md->mlx), free_void(md->mlx), fa + 2);
	printf("[PART 1] total: %d\n", fa);
	fa += free_ents(md);
	fa += free_battle_data(md, &md->battle_d);
	fa += free_txd(md, &md->txd);
	fa += mlx_destroy_window(md->mlx, md->win) + 1;
	fa += mlx_destroy_display(md->mlx);
	fa += free_void(md->mlx);
	ft_printf("Freed: %d elements\n", fa);
	return (fa);
}

int	free_and_quit(t_md *md, const char *msg, const char *attribute)
{
	if (msg)
	{
		printf("Error\n%s", msg);
		if (attribute)
			printf(": %s", attribute);
		printf("\n");
	}
	stop_sound(md->au.wind_pid);
	stop_sound(md->au.mus_pid);
	cleanup_thread_pool(md);
	free_md(md, 1);
	exit(0);
	return (1);
}
