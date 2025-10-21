/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_au.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 01:25:07 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/15 01:27:37 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	init_au(t_md *md, t_au_manager *au)
{
	const char	path[] = "ressources/audio/pokemon/cries/";
	char		*full_path;
	int			i;

	i = -1;
	while (++i < PKMN_TYPE_LEN)
	{
		full_path = ft_megajoin(path, get_pkmn_name(md, i), ".mp3", NULL);
		if (!full_path) {
			free_and_quit(md, "alloc in init_au", NULL);
		}
		ft_strlcpy(au->pokemon_cries[i], full_path, 50);
		free(full_path);
	}
}
