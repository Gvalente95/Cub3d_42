/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 21:53:43 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/11 19:06:37 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE_H
# define CUBE_H

# include "ressources/mlx_wrapper/mlx_utils.h"
# include "ressources/lists/lists.h"
# include "ressources/gnl/get_next_line.h"
# include <math.h>
//# include <X11/X.h>

# define IMG_PATH		"ressources/xpm/"
# define RESOLUTION		25
//		SCREEN
# define SCRN_W			2800
# define SCRN_H			2000
//		RAYS
# define RAY_DEPTH		50
//		PLR
# define PLRSPD			800
# define ACCSPD			0.3
# define HEIGHT			0
# define BOB_AMOUNT		.2
# define BOB_SPD		7
# define GRAVITY		.06
# define ARROW_ROT_SPEED 2
# define MOUSESPD		.5
# define START_HP		10
# define SHOOT_REFRESH	.1
# define WALK_REFRESH	.5
# define ANIM_REFRESH	.05
# define MAX_AMMO		50
# define MAX_KEY		3
//		HUD
# define STARS_AMOUNT	1000
# define CROSS_SCALE	10
//		ENNEMY
# define ENNEMY_SPEED	.1
# define RAY_ZOOM		50

# define THREADS_BATCH	8
# define FLOOR_WORKERS	4
# define FE_PER_TILE	50
# define REVEAL_DISTANCE 5
# define TARGET_FPS		100

//		init/init_menu.c
void	init_menu(t_md *md, t_menu *menu);
int		replace_window(t_md *md, int new_w, int new_h);
void	set_menu_pos(t_md *md, t_menu *menu, \
	t_vec3 sldr_offst, t_vec3 but_ofst);
//	init/init_frames2.c
void	handle_mobs_frames(t_md *md, t_image ****frames, \
	t_image ****mini, t_mob_types type);
void	init_mobs_frames(t_md *md);
t_image	**init_mini(t_md *md, t_image ***mini, char *path);
t_image	**init_weapon(t_md *md, t_image ***mini, char *path);

//	init/init_hud.c
void	init_fx(t_md *md, t_fx_data *fx);
void	init_hud(t_md *md, t_hud *hud);
void	init_fes(t_md *md, t_env_manager *env, int tlen);

//	init/init_map.c
int		init_map(t_md *md, char *file_name);

//	init/init_minimap.c
void	init_minimap(t_md *md, t_mmap *mmap);

//	init/init_labels.c
void	init_labels(t_texture_data *txd);

//	init/init_frames.c
void	init_weapon_frames(t_md *md, t_texture_data *td);
void	init_item_frames(t_md *md, t_texture_data *td);
void	store_entities_sizes(t_texture_data *td, int base);
void	init_ents_data(t_md *md, t_texture_data *txd);

//	init/init_ent_frames.c
void	init_ent_frames(t_md *md, t_texture_data *txd, t_ent *e);

//	init/init_menu_elements.c
void	init_menu_elements(t_md *md, t_menu *menu);

//	init/init_map_data.c
void	init_map_data(t_md *md);

//	init/init_background.c
void	init_background(t_md *md, t_hud *hud, t_vec2 win_sz);

//	init/init_map_validator.c
int		find_breach(t_md *md, t_map *map, int plr_index);
int		validate_map(t_md *md, char *map);

//	init/init_entities.c
t_ent	*init_ent(t_md *md, char c, t_vec2 pos, int map_index);
void	init_entities(t_md *md, t_vec2 pos);

//	init/init_cube.c
int		init_cube(t_md *md, char *file_arg, int start_debug);

//	raycasting/ray_tools_2.c
void	set_ray_color(t_md *md, t_ray *ray);
int		update_ray_grid_pos(t_md *md, t_ray *ray);
int		correct_fisheye(t_md *md, t_ray *ray, t_ent *e, float dist);
int		get_wall_orientation(t_ray *ray);
int		compute_row_start(t_md *md, t_ent *e, float ray_dst);

//	raycasting/ray_cast.c
int		cast_check_ray(t_md *md, t_ray *ray, t_vec3f start_pos, t_ent *check);
int		cast_ray(t_md *md, t_ray *ray, t_vec2 visu_offset);
void	compute_ray_directions(t_md *md, t_vec3f *dir_vals, int rays_amount);
void	cast_rays(t_md *md);
void	paint_ent(t_md *md, t_ent *e, t_vec2 txtr_coord);

//	raycasting/portal_raycast.c
int		dir_to_rotation(t_wrd_dir a, t_wrd_dir b);
void	rotate_90_deg(t_vec3f *vec, int times);
int		get_portal_index(t_md *md, t_ray *ray, t_ent *portal);
int		translate_ray(t_md *md, t_ray *ray, t_ent *portal, int view_index);

//	raycasting/portal_draw.c
void	draw_portal(t_md *md, t_ent *e, t_vec2 pos);

//	raycasting/ray_move.c
t_ent	*search_in_grid(t_md *md, t_ray *ray, float distance);
int		ray_move(t_md *md, t_ray *ray, t_vec2 visu_offset);

//	raycasting/draw_wall_column.c
int		draw_wall_line(t_md *md, float dist, t_ent *wall, t_ray *ray);

//	raycasting/ray_tools.c
void	draw_blood(t_md *md, t_image *img, t_vec2 pos, int color);
t_vec2	get_2d_ray_pos(t_md *md);
int		render_ray(t_md *md, t_ray *ray, t_vec2 visu_offset);
void	init_base_ray(t_ray *ray, int index, t_vec3f start_pos, float distance);
int		validate_check_hit(t_md *md, t_ray *ray, t_ent *ent, t_ent_type type);

//	raycasting/ray_cast_threads.c
void	update_ray_data(t_md *md, t_ray *ray, t_vec3f dir_val);
void	*cast_thread_batch(void *content);

//	tools/map_gen.c
char	*get_cmps_map(t_vec2 size, int rects_amount, int i);
char	*get_new_map(int difficulty, t_vec2 *size, char *data_info);

//	tools/parse_tools.c
void	trim_excess_spaces(char **line);
int		trim_excess_newlines(char **map, int len);
void	remove_chars(t_md *md, char **txt, const char *to_remove);
int		get_to_find_index(char *str, char *to_find);

//	tools/map_gen_2.c
void	set_doors(char *map, int doors_amount);
void	set_characters(char *map, int difficulty);
char	*set_map_with_base(char *map);

//	tools/debug.c
void	show_init_information(t_md *md);
void	show_fps(t_md *md, t_vec2 pos);
void	show_update_information(t_md *md);
void	print_color(int color, const char *label);

//	tools/map_gen_tools.c
char	*get_rectangle(t_vec2 size);
int		get_char_amount(char *buffer, char c);
void	close_map(char *map, t_vec2 size, int len);

//	tools/text.c
char	*get_img_path(char c);

//	tools/image_tools.c
void	color_img(void *frame, t_vec2 size, int col, t_vec4 d);
int		str_to_color(const char *line);

//	tools/ftoa.c
char	*ftoa(float num, int precision);

//	update/update_plr.c
int		update_player(t_md *md, t_ent *plr);

//	update/update_input.c
void	update_input(t_md *md);

//	update/update.c
int		set_menu_mode(t_md *md, t_menu *menu, int mode);
int		update_and_render(t_md *md);

//	update/movement.c
void	move_ent_to_target(t_md *md, t_ent *e);
void	update_mob_actions(t_md *md, t_ent *e);

//	update/update_menu.c
int		update_menu(t_md *md, t_menu *menu);

//	update/update_ents.c
void	update_ent_frame(t_ent *e);
int		update_ents(t_md *md);

//	update/collisions.c
int		is_collision(t_ent *a, t_ent *b, t_vec2 a_size);
int		set_collisions(t_md *md, t_ent *e, t_vec2 e_size);

//	update/update_projectile.c

//	update/update_plr_movement.c
int		move_player(t_md *md, t_ent *e);
int		update_player_mov(t_md *md, t_ent *plr);

//	update/collisions_portal.c
int		dir_to_angle(t_wrd_dir dir);
int		get_portal_angle_offset(t_wrd_dir start_dir, t_wrd_dir end_dir);
int		validate_portal_collision(t_md *md, t_ent *b);

//	render/render_minimap.c
void	render_minimap_ray(t_md *md);
void	show_minimap_entity(t_md *md, t_ent *e, t_image *screen, int no_redraw);
void	render_minimap(t_md *md, t_mmap *mp);

//	render/filters.c
void	apply_antialiasing(t_image *img);
void	apply_scanlines(t_image *img, float factor);
void	apply_noise(t_md *md, t_image *img, float factor, float colors_amount);

//	render/render_hud.c
void	draw_locks_icons(t_md *md, t_hud *hud, t_vec2 winsz);
void	draw_info(t_md *md, t_vec2 *screen_pos, t_image *icon, int value);
void	draw_game_info(t_md *md, t_hud *hud, t_vec2 winsz);
void	draw_hud_weapon(t_md *md, t_hud *hud, t_vec2 winsz);
void	render_hud_elements(t_md *md, t_hud *hud);

//	render/render.c
void	render_cursor(t_md *md, t_image *screen, int has_hov);
void	render_2d_ent(t_md *md, t_ent *e, t_vec2 centr);
void	apply_fx(t_md *md, t_image *screen, t_fx_data *fx);
void	render(t_md *md);

//	render/filters_2.c
void	apply_barrel_fx(t_image *img, t_vec2 sz, t_vec2 c, float intensity);
void	apply_color_banding(t_image *img, float intensity);
void	apply_bloom(t_image *img, float intensity);
void	apply_vignette(t_image *img, float intensity, int color);
void	set_hue(t_image *img, t_vec4f rgb_factors);

//	render/render_background.c
void	render_2d_floor(t_md *md);
void	render_background(t_md *md);

//	render/render_minimap_cmp.c
void	show_cmps_mmap(t_md *md, t_vec2 center, int view_dist);
void	draw_sprite_thread(t_md *md, t_ent *e, int had_door, float fogalpha);
int		is_in_list(t_dblst *lst, t_ent *e);
void	draw_found_ents(t_md *md, t_thrd_manager *mon);
void	render_menu(t_md *md, t_menu *menu);
void	render_slider(t_md *md, t_slider *sldr, t_image *screen, float alpha);

int		update_key_input(t_md *md, t_menu *m, t_inventory *i, unsigned int c);

int		draw_wall_line_dda(t_md *md, float dist, t_ent *hit, t_ray *ray);
int		ray_move_dda(t_md *md, t_ray *ray);
int		update_and_render_fe(t_md *md, t_floor_draw_d *d, t_vec2 t_crd);
void	draw_raycast_background(t_md *md, t_ray *ray);
void	draw_ceiling(t_md *md, t_floor_draw_d d, float pitch_offset);
int		draw_stored_sprite_hits(t_md *md, t_ray *ray);
int		display_quick_letter(t_md *md, char c, t_txtd data);
int		update_autocam(t_md *md, t_autocam *autocam);
void	update_audio(t_md *md, t_au_manager *au);
void	init_fade_intro(t_md *md, double duration);
void	render_autocam_text(t_md *md, float t);
int		add_ent_at_cord(t_md *md, t_ent *e, t_vec2 new_cord);

//		map_ents
void	init_mapped_ent(t_md *md);
t_ent	*get_mapped_at_cord(t_md *md, t_vec2 cord);
t_ent	*get_mapped_at_pos(t_md *md, t_vec2f pos);

void	render_logo_cube(t_md *md, t_menu *menu);
void	update_logo_cube(t_md *md, t_mouse mouse, t_menu *menu);
void	update_menu_element_end(t_menu *menu, int *hov, int cur_hov);
void	center_menu_txt(t_md *md, t_vec2 pos_ofst, int scale, char *name);
int		render_sky(t_md *md);
void	cap_fps(t_timer *tm, double frame_duration);
void	init_pk_framess(t_md *md, t_texture_data *txd);
void	show_pointed_data(t_md *md, t_vec2 p, t_ent *e);

//		INVENTORY
void	collect_item(t_md *md, t_inventory *inv, t_ent *e);
void	init_inventory(t_md *md, t_inventory *inv);
int		grab_item(t_md *md, t_inventory *inv, int item_index);
void	refresh_inv_bgr(t_md *md, t_inventory *inv);
void	set_inventory(t_md *md, t_inventory *inv, int active);
void	render_inventory(t_md *md, t_inventory *inv);
void	refresh_inv_opt_bgr(t_md *md, t_inventory *inv);
void	update_inventory(t_md *md, t_inventory *inv);
void	close_option_pannel(t_inventory *inv);
int		update_inv_input(t_md *md, t_inventory *inv, unsigned int c);
void	draw_pokemon_team(t_md *md, t_inventory *inv, t_txtd td, int brdsz);
void	refresh_inv_map(t_md *md, t_inventory *inv, t_image *map_bg);

char	*get_out_map(char *map, int width, int len);
void	render_world_map(t_md *md, t_floor_draw_d d, int y_size);
void	apply_contrast(t_image *img, float contrast);
void	show_debug(t_md *md, char *msg, int *value, char *attribute);
int		ent_sort_cmp(void *a, void *b);
void	update_wall_pointed(t_md *md, t_ent *e, t_vec2 draw_limits);
void	capture_pokemon(t_md *md, t_inventory *inv, t_ent *e);
int		use_held_item(t_md *md, t_inventory *inv, t_ent *pointed, int index);
void	show_debug_time(t_md *md, t_txtd txt_data);
void	render_color_wheels(t_md *md, t_menu *menu, \
	t_image *screen, int touch_sz);
void	update_color_wheels(t_md *md, t_menu *menu);
void	draw_strip(t_image *from, t_image *to, t_vec2 x, int stop_y);
int		remove_ent(t_md *md, t_ent *e);

void	render_time_logs(t_md *md, t_timer *timer);
void	add_log_to_queue(t_md *md, int clr, const char *format, ...);
void	add_alert(t_md *md, double duration, int *out_cond, const char *alert);
void	render_held_item(t_md *md, t_inventory *inv, int hand_index);
int		contains_valid_character(char *line, const char *valid_characters);
void	update_pointed(t_md *md, t_vec2 draw_p, t_vec3 sz_scale, t_ent *e);
void	update_pointed_ent(t_md *md);
void	inisld(t_md *md, char *label, t_vec4f data, float *value);
void	render_used_shadow(t_md *md, t_vec2 usd_p, t_vec2 u_sz, double dur);
int		update_inv_selection(t_md *md, t_inventory *inv);

//		battle_data
void	init_battle_data(t_md *md, t_battle_d *bd);
void	start_battle(t_md *md, t_battle_d *bd, t_ent *foe, t_ent *foe_pk);
int		update_battle_scene(t_md *md, t_battle_d *bd);
void	get_bt_butn(t_md *md, t_vec2 sz, t_vec2 pos, const char *label);
void	draw_hp_bar(t_md *md, t_ent *e, t_vec2 pos, t_vec2 size);
void	update_battle_text(t_battle_d *bd, const char *format, ...);
void	refresh_battle_screen(t_md *md, t_battle_d *bd);
void	exit_battle(t_md *md, t_battle_d *bd);
void	render_trans_screen(t_md *md, t_battle_d *bd, double elapsed);
void	wait_action(t_md *md, int incr_hp, const char *msg, int change_turn);
int		set_battle_option(t_md *md, t_battle_d *bd);
void	update_action(t_md *md, t_battle_d *bd);
t_ent	*get_valid_pkmn(t_ent **team, int team_size);
int		change_pokemon(t_md *md, t_ent *new, int side, int change_turn);
int		battle_use_attack(t_md *md, int dmg, const char *atk_name);
char	get_input_char(t_md *md, int key);
void	update_pk_frames(t_inventory *inv, t_ent *e);
void	render_battle_buttons(t_md *md, t_battle_d *bd);
void	render_team_logos(t_md *md, t_battle_d *bd, int side, int team_size);
void	render_batt_pkmn_1(t_md *md, t_battle_d *bd, t_ent *pok, t_vec2 base_p);
void	render_batt_pkmn_0(t_md *md, t_battle_d *bd, t_ent *pok, t_vec2 base_p);
void	refresh_battleground(t_md *md, t_battle_d *bd);
int		move_cam_to_start(t_md *md);
void	update_trans_lvl(t_battle_d *bd, double elapsed);
void	handle_pkmn_ko(t_md *md, t_battle_d *bd, t_ent *pk, int i);
void	draw_grad_pxls(t_image *dst, t_vec2 pos, t_vec2 sz, t_vec3 clr);
void	draw_from_pos(t_image *src, t_image *dst, t_vec2 pos, t_vec2 draw_strt);
void	init_ent_pkteam(t_md *md, t_ent *e, int team_size);
float	get_pitch_offset(t_md *md);
void	update_sliders(t_md *md, t_menu *menu, t_vec2 sz);

#endif