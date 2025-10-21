/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   battle.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:00:46 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/16 10:57:03 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BA_H
#define BA_H

# include "../mlx_utils.h"

typedef struct s_image_data	t_image;

# define BME					1
# define BFOE					0

# define BT_ON				0
# define BT_START			1
# define BT_OPPPKSET		2
# define BT_MYPKSET			3
# define BT_WAIT			4
# define BT_SWITCH_TURN		5
# define BT_TURN_END		6
# define BT_TURN_END_CONF	7
# define BT_TURN_END_STATUS	8
# define BT_TURN_END_EXP	9
# define BT_END				10
# define BT_WON				11
# define BT_REWARD			12
# define BT_PUNITION		13
# define BT_QUIT			14

# define BA_NO_ACTION		0
# define BA_MOVE_USED		1
# define BA_MOVE_SELF		2
# define BA_ITEM_USED		3
# define BA_MOVE_MISSED		4
# define BA_STATUS_EFFECT	5

typedef struct s_BA_d
{
	t_image			*overlay;
	t_ent			*pk[2];
	t_vec2			pk_p[2];
	t_vec2			pk_basep[2];
	t_vec2			pk_sz;
	t_vec2			but_sz;
	t_vec2			hp_pos[2];
	char			log_message[60];
	t_ent			*opponent;
	int				bstate;
	t_image			*buttons[4];
	t_vec2			pk_slotsz;
	t_vec2			but_ps[4];
	t_move			* last_move_used;
	t_move			* cur_move_used;
	int				action_type;
	int				action_ended;
	int				but_i;
	int				sub_i;
	int				opt_i;
	int				in_sub;
	double			action_dur;
	double			trans_dur;
	double			trans_start;
	int				turn;
	int				active;
	int				quitting;
	int				stored_dealt[2];
}	t_BA_d;

#endif