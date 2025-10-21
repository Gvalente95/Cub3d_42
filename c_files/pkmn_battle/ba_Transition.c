#include "../../cube.h"

void	render_trans_screen(t_md *md, t_BA_d *bd, double elapsed)
{
	int				offst;
	int				i;
	t_vec2			s;
	const int		slh = md->win_sz.y / 50;
	const double	t = minf(1.0, elapsed / bd->trans_dur);

	bd->pk_basep[0].x = max(bd->pk_p[0].x, 0 + (bd->pk_p[0].x - 0) * t);
	bd->pk_basep[BME].x = \
		min(bd->pk_p[1].x, md->win_sz.x + (bd->pk_p[1].x - md->win_sz.x) * t);
	render_battle(md, bd);
	offst = minmax(0, md->win_sz.x, (elapsed * md->win_sz.x) / bd->trans_dur);
	s = (t_vec2){md->win_sz.x - offst, slh};
	i = -1;
	while (++i < 50)
	{
		if (i % 2 == 0)
			draw_pixels(md->screen, (t_vec2){0, slh * i}, s, _BLACK);
		else
			draw_pixels(md->screen, \
				(t_vec2){md->win_sz.x - s.x, slh * i}, s, _BLACK);
	}
	mlx_put_image_to_window(md->mlx, md->win, md->screen->img, 0, 0);
	reset_mlx_values(md);
}

void	set_bTransition(t_md* md, int type, const char* format, ...) {
	md->BA_d.bstate = type;
	md->BA_d.trans_start = md->timer.cur_tm;
	if (format) {
		va_list	args;
		va_start(args, format);
		vsnprintf(md->BA_d.log_message, sizeof(md->BA_d.log_message), format, args);
		va_end(args);
	}
}

int	update_bstate(t_md* md, t_BA_d* bd, double elapsed)
{
	if (bd->bstate == BT_START) {
		render_trans_screen(md, bd, elapsed);
		if (elapsed > bd->trans_dur) {
			if (bd->opponent->is_trainer)
				set_bTransition(md, BT_OPPPKSET, "%s wants to fight!", bd->opponent->label);
			else
				set_bTransition(md, BT_MYPKSET, "A wild %s appeared!", bd->opponent->label);
		}
		return (1);
	}
	if (md->mouse.click != MOUSE_RELEASE) return (1);
	switch (bd->bstate)
	{
		case BT_OPPPKSET:
			change_pokemon(md, get_valid_pkmn(bd->opponent->pk_team, bd->opponent->team_sz), 0, 0);
			bd->bstate = BT_MYPKSET;
			break;
		case BT_MYPKSET:
			change_pokemon(md, get_valid_pkmn(md->inv.pokemon_team, md->inv.team_size), BME, 0);
			bd->turn = true;
			break;
		case BT_SWITCH_TURN:
			switch_turn(md, bd);
			bd->bstate = BT_ON;
			break;
		case BT_PUNITION:
			bd->turn = BME;
			md->plr.pos = md->plr.start_pos;
			set_bTransition(md, BT_QUIT, "you gave %s a %s", bd->opponent->label, get_item_name(md, md->inv.items[0]));
			break;
		case BT_REWARD:
		{
				bd->turn = BME;
				int item = r_range(0, PCKP_TYPE_LEN - 1);
				add_item(md, item, 1);
				set_bTransition(md, BT_QUIT, "%s gave you a %s", bd->opponent->label, get_item_name(md, item));
				break;
			}
		break;
		case BT_QUIT: case BT_WON:
			exit_battle(md, bd);
			break;
		default:
			set_BA_text(bd, "");
			set_bTransition(md, BT_ON, NULL);
		break;
	}
	bd->stored_dealt[BME] = 0;
	bd->stored_dealt[BFOE] = 0;
	return (1);
}
