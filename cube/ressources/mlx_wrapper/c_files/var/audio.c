/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 04:54:44 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/24 12:37:33 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

int	play_rand_sound(t_md *md, const char *path, int len, int last_index)
{
	char	*with_index;
	char	*fullpath;
	char	*index_str;
	int		index;

	if (!md->prm.au_on)
		return (0);
	index = r_range(0, len - 1);
	while (index == last_index)
		index = r_range(0, len - 1);
	index_str = ft_itoa(index);
	with_index = ft_strjoin(path, index_str);
	free(index_str);
	fullpath = ft_strjoin(with_index, ".mp3");
	free(with_index);
	play_sound(md, fullpath);
	free(fullpath);
	return (index);
}

pid_t	play_sound(t_md *md, const char *filename)
{
	pid_t	pid;

	if (!md->prm.au_on)
		return (0);
	if (access(filename, F_OK) == -1)
		return (printf("%s audio file not found\n", filename), 0);
	pid = fork();
	if (pid == 0)
	{
		if (md->is_linux)
			execlp("ffplay", "ffplay", "-v", "quiet", "-nodisp", "-autoexit", \
				filename, (char *) NULL);
		else
			execlp("afplay", "afplay", filename, (char *) NULL);
		perror("execlp failed!");
		exit(EXIT_FAILURE);
	}
	return (pid);
}

void	stop_sound(pid_t pid)
{
	if (pid > 0)
		kill(pid, SIGTERM);
}

int	play_loop(t_md *md, pid_t *pid, char *filename, int depend)
{
	int	status;

	if (!pid)
		return (printf("called play_loop_au with no pid"), 0);
	if (!depend || !md->prm.au_on && pid)
	{
		stop_sound(*pid);
		return (0);
	}
	if (waitpid(*pid, &status, WNOHANG) == 0)
		return (0);
	if (access(filename, F_OK) == -1)
	{
		*pid = play_sound(md, filename);
		return (1);
	}
	*pid = play_sound(md, filename);
	return (1);
}

void	init_au(t_md *md, t_au_manager *au)
{
	const char	path[] = "ressources/audio/pokemon/cries/";
	char		*full_path;
	int			i;

	i = -1;
	while (++i < PKMN_TYPE_LEN)
	{
		full_path = ft_megajoin(path, md->txd.pkmn_names[i], ".mp3", NULL);
		if (!full_path)
			free_and_quit(md, "alloc in init_au", NULL);
		ft_strlcpy(au->pokemon_cries[i], full_path, 50);
		free(full_path);
	}
}
