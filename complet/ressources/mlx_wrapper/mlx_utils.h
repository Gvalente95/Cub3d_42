/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 16:32:42 by gvalente          #+#    #+#             */
/*   Updated: 2025/06/11 19:42:58 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_UTILS_H
# define MLX_UTILS_H

# include "headers/vectors.h"
# include "headers/entity.h"
# include "headers/colors.h"
# include "headers/game.h"
# include "headers/rays.h"
# include "headers/textures.h"
# include "headers/math.h"
# include "headers/math_2.h"
# include "headers/audio.h"
# include "headers/threads.h"

# include "libft/libft.h"
# include "../lists/lists.h"
# include "mlx_linux/mlx.h"
# include "mlx_mac/mlx.h"
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>
# include <time.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
//# include <X11/X.h>

# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>

# ifdef IS_LINUX
#  define LIN 1
#  include "headers/Keys_lnx.h"
# else
#  define LIN 0
#  include "headers/Keys_mac.h"
# endif

# define MOUSE_SENSITIVITY	.2
# define MAPPED_ENT_MAX		150
# define MOUSE_SMTH			0.2f
# define _PI				3.14159265358979323846

typedef struct s_mmap
{
	t_image		*bg;
	t_image		*img;
	t_image		*sphere;
	t_image		*full_map;
	t_vec2		size;
	int			cmps;
	int			comps_scl;
	int			ic_scl;
	int			active;
	int			mray_len;
	int			bgr_color;
	int			revealed_cur;
	int			revealed_len;
}	t_mmap;

typedef struct s_hud
{
	t_image		*overlay;
	t_image		*rgun;
	t_image		*base_sky;
	t_image		*sky;
	t_image		*sky_flipy;
	t_image		*sky_buffer;
	t_image		*floor;
	t_image		*wall;
	t_image		*ceiling;
	t_image		*floor2d;
	t_image		*lock_x_icon;
	t_image		*lock_y_icon;
	t_image		*amm_icon;
	t_image		*amm2_icon;
	t_image		*key_icon;
	t_image		*key2_icon;
	t_image		*hp_icon;
	t_image		*center;
	int			bgr_color;
	int			floor_color;
	int			ceiling_color;
	int			fog_color;
	int			floor_start;
	int			keys;
	int			hp;
}	t_hud;

typedef struct s_post_fx_data
{
	t_vec4f			hue;
	t_image			*vignette;
	float			rgb_distortion;
	float			scanlines;
	float			bloom_threshold;
	float			color_band;
	float			chrom_amount;
	float			contrast;
	float			barrel_amount;
	float			glow_intensity;
	float			fog;
	float			noise;
	int				anti_alias;
}	t_fx_data;

typedef struct s_parameters
{
	int				resolution;
	int				show_rays;
	int				view_2d;
	int				debug_mode;
	int				use_thrd;
	int				show_fps;
	int				fly_cam;
	int				ent_mode;
	int				max_view_sprite;
	int				au_on;
	float			cap_fps;
	int				show_ceiling;
	int				show_floor;
	int				super_view;
	int				show_grass;
	int				show_wmap;
	int				show_walls;
	int				show_hud;
	int				show_sky;
	int				alternate_draw;
	float			ray_mod;
	float			bob_amount;
	float			sun_x;
	float			sun_y;
	t_vec2f			grass_sz;
	float			fe_speed;
	float			ray_depth;
	float			height;
	float			plr_speed;
	float			rot_speed;
	float			res_value;
	float			txt_sc;
	float			zoom;
	float			floor_glide;
	float			mov_drift;
	float			fov;
	float			win_x;
	float			floor_fov;
	float			win_y;
}	t_parameters;

typedef struct s_mouse
{
	t_image			*cursor;
	t_image			*curs_dtc;
	t_image			*curs_grb;
	t_vec3f			pos;
	t_vec2			real;
	t_vec2			prev;
	t_vec2			delta_raw;
	t_vec2f			delta;
	t_vec2			world;
	t_vec2			grid_pos;
	t_vec2			lock_rot;
	t_vec2f			scroll_delta;
	t_vec2			scroll_raw;
	int				focus;
	int				pressed;
	int				click;
	int				locked;
	int				hide;
}	t_mouse;

typedef struct s_cam
{
	t_vec3f			input_mov;
	t_vec2			input_offst;
	t_vec3f			rot;
	t_vec3f			pos;
	t_vec3f			ofst;
	t_vec3f			wrd_mv_offst;
	t_vec3f			plr_wrd_mv;
	t_ent			*pointed;
	t_ent			*prv_pointed;
	t_ent			*pointed_door;
	t_ent			*prv_door;
	t_ent			*pointed_ent;
	t_ent			*prv_pointed_ent;
	t_vec2			last_pointed_ent_pos;
	float			bob_time;
	int				is_moving;
	int				closest_x;
	int				plr_map_i;
	int				x_dir_start;

}	t_cam;

typedef struct s_text_data
{
	int		x;
	int		y;
	int		color;
	int		scale;
	t_image	*onto;
}	t_txtd;

typedef struct s_logs
{
	int		*out_cond;
	double	duration;
	float	timer_start;
	char	buffer[256];
	t_txtd	txt_d;
}	t_log;

typedef struct s_cube_draw_data
{
	t_vec3f	cnt;
	t_vec3f	angle;
	t_vec3f	cube_pts[8];
	int		faces[6][4];
	t_vec2	projected[8];
	int		color;
}	t_cube_drawd;

typedef struct s_md
{
	void			*mlx;
	void			*win;
	char			plr_name[15];
	int				plr_name_indx;
	t_log			alert;
	t_image			*screen;
	t_vec2			win_sz;
	t_ent			plr;
	t_ent			*map_ents[MAPPED_ENT_MAX][MAPPED_ENT_MAX];
	t_dblst			*entities;
	t_hud			hud;
	t_menu			menu;
	t_map			map;
	t_portal		portal;
	t_mmap			mmap;
	t_autocam		autocam;
	t_battle_d		battle_d;
	t_timer			timer;
	t_fx_data		fx;
	t_cube_drawd	cube_d;
	t_mouse			mouse;
	t_ray			rays[MAX_RAYS];
	t_texture_data	txd;
	t_vec2			input_to_char[34];
	t_cam			cam;
	t_parameters	prm;
	t_thrd_manager	thrd_manager;
	t_au_manager	au;
	t_env_manager	env;
	t_inventory		inv;
	char			base_map_path[50];
	char			*out_map;
	unsigned int	r_seed;
	int				plr_in_house;
	int				switch_interior;
	int				plr_was_on_door;
	int				key_prs[65536];
	int				key_click;
	int				last_key;
	int				init_steps;
	int				rgb[20];
	int				t_len;
	int				var;
	int				score;
	int				is_linux;
	int				(*mlx_put)(void *mlx, void *win, void *img, int x, int y);
	void			*(*mlx_make)(void *mlx, char *name, int *with, int *height);
}	t_md;

//		input/input_tools.c
void	wrap_mouse(t_md *md, int delta_x, int delta_y);
void	lock_mouse_center(t_md *md);

//		input/input_mouse.c
int		mouse_event_handler(int button, int x, int y, void *param);
int		mouse_release_handler(int button, int x, int y, void *param);
int		mouse_motion_handler(int x, int y, void *param);
int		update_mouse(t_md *md);

//		input/input.c
int		handle_key_press(int keycode, t_md *md);
int		handle_key_release(int keycode, t_md *md);
void	reset_mlx_values(t_md *md);
int		close_window(t_md *md);

//		free/free_image.c
int		free_image_data(t_md *md, t_image *img_data);
int		free_images_data(t_md *md, t_image **images, const char *label);
int		free_images_array(t_md *md, t_image ***arr, const char *label);
int		free_mob_images(t_md *md, t_ent *e, char *label);

//		free/free_elements.c
int		free_hud(t_md *md, t_hud *hud);
int		free_txd(t_md *md, t_texture_data *txd);
int		free_var(t_md *md, t_mmap *mmap, t_fx_data *fx, t_mouse *mouse);
int		free_menu(t_md *md, t_menu *menu);
int		free_ents(t_md *md);
int		free_inv(t_md *md, t_inventory *inv);
int		free_env(t_md *md, t_env_manager *env);
int		free_battle_data(t_md *md, t_battle_d *bd);

//		free/free.c
int		safe_free(void *item);
int		free_void_array(void **elements);
int		free_void(void *elem);
int		free_md(t_md *md, int quit);
int		free_and_quit(t_md *d, const char *msg, const char *attribute);

//		time/time.c
void	resume_timer(t_timer *timer);
double	get_total_time(t_timer *timer);
double	get_time_in_seconds(void);
void	start_timer(double *timer);
void	init_timer(t_md *md, t_timer *timer);
void	reset_fps_timer(t_timer *timer);

//		math/math_tools_3.c
int		r_range_seed(unsigned int *g_seed, int min, int max);
int		r_range(int min, int max);
float	f_range(float min, float max);

//		init_wrapper.c
void	init_wrapper(t_md *md, t_vec2 win_sz, char *win_name, int res);

//		string/string_3.c
int		same_str(const char *a, const char *b);
int		char_in_str(char c, const char *txt);
int		chr_amnt(const char *str, char c);
int		get_arr_len(void **arr);
char	*truncate_at_end(const char *str, char cut_letter);

//		string/string.c
char	*md_strjoin(t_md *d, char const *s1, char const *s2);
char	*md_strdup(t_md *d, const char *s1);
void	*md_malloc(t_md *d, ssize_t size);
void	*md_realloc(t_md *d, void *ptr, size_t new_size);
char	*md_strndup(t_md *d, const char *s1, ssize_t n);

//		string/string_2.c
void	setstr(char **str, char *new);
int		only_contains(char *str, char *to_contain);
char	*ft_megajoin(const char *a, const char *b, \
	const char *c, const char *d);
int		get_char_index(const char *str, char to_check);
char	*ft_strndup(const char *s1, ssize_t n);

t_vec3	get_grid_posf(t_md *md, t_vec3f pos);
t_vec3	get_grid_pos(t_md *md, t_vec3 pos);

void	show_vec2(t_md *md, char *label, t_vec2 vec, t_vec2 pos);
void	show_float(t_md *md, char *label, float value, t_vec2 pos);
void	show_int(t_md *md, char *label, int value, t_vec2 pos);
void	show_vec3f(t_md *md, char *label, t_vec3f vec, t_vec2 pos);
void	show_vec3(t_md *md, char *label, t_vec3 vec, t_vec2 pos);
void	show_vec2(t_md *md, char *label, t_vec2 vec, t_vec2 pos);
int		is_in_screen(t_md *md, t_vec3 pos, t_vec2 size);
void	upd_timer(double *tmr, double cur_tm, double incr, int *event);
void	update_fe(t_md *md, t_fe *fe, t_floor_draw_d *d);

//				camera.c
void	update_cam(t_md *md, t_cam *cam);
t_vec3f	update_fly_cam(t_md *md, t_cam *cam, float spd);
int		color_diff(int c1, int c2);
void	clean_img(t_image *img);

void	swap_vec2(t_vec2 *a, t_vec2 *b);
void	draw_quad(t_image *screen, t_vec2 p[4], int color, int contour_clr);
void	draw_filled_triangle(t_image *img, t_vec2 v[3], int clr);
void	draw_cube(t_image *screen, t_vec3 pos, t_vec2 size, t_vec3f rot);
t_vec3f	rotate_vec3f(t_vec3f v, t_vec3f rot);
t_vec2f	rotate_vec2f(t_vec2f v, float angle);
t_vec2	project(t_vec3f p, t_vec2 origin);
int		is_in_shape(t_vec2 z, t_vec2 tri[][3], int amount);
t_vec3f	cross_vec3f(t_vec3f a, t_vec3f b);
float	dist2(t_vec2 a, t_vec2 b);
void	draw_quad_contour(t_image *screen, t_vec2 p[4], int contour_clr);
int		is_flat_tri(t_vec2 v[3], int treshold);
void	hsv_to_rgb(float h, float s, float v, t_vec4f *rgb);
void	set_wheel(t_clrp *w, t_vec2 mouse_pos, t_vec2 img_center);
void	draw_quad(t_image *screen, t_vec2 p[4], int color, int contour_clr);
void	draw_img_contour(t_md *md, t_image *src, t_vec2 pos, t_vec2 clr_thk);
int		wrap_int(int *number, int min, int max, int increment);
int		get_feet_offset(t_image *img);
int		rnd_fast_txt(t_md *md, t_txtd data, const char *format, ...);
int		rnd_txt_simple(t_md *md, t_vec2 pos, const char *format, ...);
void	init_fonts(t_md *md);
void	flush_gradient(t_image *src, int color_a, int color_b, float transp);
void	lock_mouse_center(t_md *md);
void	show_cursor(t_md *md);
void	hide_cursor(t_md *md);
void	draw_img_except(t_image *src, t_image *dst, t_vec2 pos, int excpt);
int		draw_smooth_rec(t_image *dst, t_vec2 pos, t_vec2 draw_size, int color);
void	randomize_sliders(t_md *md, t_menu *menu, int is_reset);
int		update_slider(t_md *md, t_slider *sld);

#endif