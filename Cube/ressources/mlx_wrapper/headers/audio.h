/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:38:37 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/01 09:18:26 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUDIO_H
# define AUDIO_H

# include "../mlx_utils.h"

# define AU_WALK_METAL		"ressources/audio/footsteps/metal/"
# define AU_WALK_GRASS		"ressources/audio/footsteps/grass/"
# define AU_RELOAD			"ressources/audio/reload/0.mp3"
# define AU_SLICE			"ressources/audio/shoot/0.mp3"
# define AU_OPEN			"ressources/audio/open.mp3"
# define AU_CLOSE			"ressources/audio/close.mp3"
# define AU_PORTAL_SHOOT	"ressources/audio/portal_shoot.mp3"
# define AU_WIND			"ressources/audio/wind.mp3"
# define AU_MUS				"ressources/audio/music.mp3"
# define AU_PKB_MUS			"ressources/audio/pokemon/battle/mus.mp3"
# define AU_PKB_HIT			"ressources/audio/pokemon/battle/hit.mp3"
# define AU_PKB_CRIT_HIT	"ressources/audio/pokemon/battle/hit2.mp3"
# define AU_PKB_LVLUP		"ressources/audio/pokemon/battle/lvlup.mp3"
# define AU_PKB_CATCH		"ressources/audio/pokemon/battle/bl_catch.mp3"
# define AU_PKB_FAIL		"ressources/audio/pokemon/battle/bl_fail.mp3"
# define AU_PKB_MV			"ressources/audio/pokemon/battle/bl_mv.mp3"
# define AU_PKB_SUCCESS		"ressources/audio/pokemon/battle/bl_success.mp3"
# define AU_PKB_FAIL		"ressources/audio/pokemon/battle/bl_fail.mp3"
# define AU_PKB_ESCAPE		"ressources/audio/pokemon/battle/escape.mp3"

# define AU_MOUSE_CLICK		"ressources/audio/ui/mouse_click.mp3"
# define AU_MOUSE_RELEASE	"ressources/audio/ui/mouse_release.mp3"
# define AU_HOV				"ressources/audio/ui/hov.mp3"
# define AU_GRAB			"ressources/audio/ui/hov.mp3"
# define AU_MENU_IN			"ressources/audio/ui/menu_in.mp3"
# define AU_MENU_OFF		"ressources/audio/ui/menu_off.mp3"
# define AU_LOW_CLICK		"ressources/audio/ui/subby_clock.mp3"
# define AU_BOP				"ressources/audio/ui/bop_3.mp3"
# define AU_TUK				"ressources/audio/ui/tuk.mp3"
# define AU_HIT				"ressources/audio/ui/hit.mp3"

typedef struct s_au_manager
{
	pid_t		battle_pid;
	pid_t		wind_pid;
	pid_t		amb_pid;
	pid_t		mus_pid;
	int			walk_index;
	char		pokemon_cries[PKMN_TYPE_LEN][50];
}	t_au_manager;

//		var/audio.c
int				play_rand_sound(t_md *md, const char *path, \
	int len, int last_index);
pid_t			play_sound(t_md *md, const char *filename);
pid_t			play_index(t_md *md, const char *filepath, int index);
void			stop_sound(pid_t pid);
int				play_loop(t_md *md, pid_t *pid, char *filename, int depend);
void			init_au(t_md *md, t_au_manager *au);

#endif