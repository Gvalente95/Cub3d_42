/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dyodlm <dyodlm@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 07:48:34 by dyodlm            #+#    #+#             */
/*   Updated: 2025/06/19 05:18:21 by dyodlm           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_H
# define CUB_H

# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <stddef.h>
# include <stdio.h>
# include <stdbool.h>
# include "libft.h"
# include "string.h"
# include "string_array.h"
# include <mlx.h>
# include <math.h>
# include <limits.h>

# include "macros.h"
# include "test.h"

# define OK 1
# define KO 0

typedef struct s_display
{
	void	*img;
	void	*addr;
	int		bpp;
	int		offset;
	int		endian;
}	t_display;

typedef struct s_long_float_point
{
	double	x;
	double	y;
}	t_dpoint;

typedef struct s_float_point
{
	float	x;
	float	y;
}	t_fpoint;

typedef struct s_bot
{
	short int	speed[3];
	char		pos[3];
}	t_bot;

typedef struct s_direction_ok
{
	bool	up;
	bool	down;
	bool	left;
	bool	right;
}	t_dir;

typedef struct s_player
{
	char		start;
	float		px;
	float		py;
	float		dx;
	float		dy;
	float		pa;
	t_dir		dir;
	bool		item;
	bool		gun;
}	t_player;

typedef struct s_poinst
{
	int	x;
	int	y;
}	t_point;

typedef struct s_map
{
	char	**map;
	int		**imap;
	int		map_s;
	t_point	max;
}	t_map;

typedef struct s_ray
{
	float	ra;
	float	rx;
	float	ry;
	int		mapx_idx;
	int		mapy_idx;
	float	fx_idx;
	float	fy_idx;
	float	dx;
	float	dy;
	int		depth;
	float	step;
	float	distance;
}	t_ray;

typedef struct s_textures
{
	void	*img;
	int		*pixels;
	float	pos;
	int		wi;
	int		hi;
}	t_texture;

typedef struct s_engine
{
	t_display	frame;
	t_display	frame2;
	t_player	player;
	t_bot		*bots;
	t_map		map;
}	t_engine;

typedef struct s_menu
{
	bool	is_menu;
	char	slot_menu[MENU_SLOTS];
	int		option;
}	t_menu;

# define NUM_TEXTURES 4

typedef enum s_text_type
{
	NORTH,
	SOUTH,
	EAST,
	WEST
}	t_text_type;

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

typedef enum e_tokens
{
	NORTH_TEXT,
	SOUTH_TEXT,
	EAST_TEXT,
	WEST_TEXT,
	FLOOR_COLOR,
	SKY_COLOR,
	WALL_COLOR,
	NO_TOKEN
}	t_etokens;

typedef enum e_object_to_paint
{
	F_COLOR,
	C_COLOR,
	W_COLOR
}	t_topaint;

# define OBJS_TO_PAINT 3

typedef struct s_tokens
{
	t_color		color_objects[OBJS_TO_PAINT];
	int			color[OBJS_TO_PAINT];
	char		*text_path[NUM_TEXTURES];	
}	t_tokens;

typedef struct s_check_list
{
	bool	map;
	bool	tex_no;
	bool	tex_so;
	bool	tex_ea;
	bool	tex_we;
	bool	col_floor;
	bool	col_sky;
}	t_check;

typedef struct s_data
{
	t_menu		menu;
	t_engine	run;
	t_tokens	tokens;
	t_texture	textures[NUM_TEXTURES];
	t_ray		ray;
	bool		check_list[NO_TOKEN];
	char		**error;
	char		*file;
	char		*trimed_file;
	void		*mlx;
	void		*win;
	void		*mlx_cast;
	void		*win_cast;	
	int			key;
}	t_data;

typedef struct s_column_draw
{
	t_data		*data;
	int			ray;
	int			distance;
	int			color;
}	t_column_draw;

typedef struct s_texdraw
{
	t_data		*data;
	int			ray;
	int			start;
	int			end;
	float		distance;
	t_texture	*tex;
	int			tx;
	int			ty;
	int			color;
}	t_texdraw;

//	SETUP
void		init_struct(t_data *data);
void		set_up_loops(t_data *data);
void		init_textures(t_data *data);
void		gather_data(t_data *data, int ac, char **av);
void		check_data(t_data *data);
void		adjust_off_bounds(int ***map, t_point max);

//	CHECKER
bool		check_map(t_data *data);
bool		check_text_path(t_data *data);
bool		check_color_config(t_data *data);

//	FILE PARSE
void		parse_metadata(t_data *data, char **doc);
char		**parse_map(char *doc);
int			**scale_map(char **map, t_data *data);

//	PARSE UTILS
bool		no_char_in_line(char *doc, int start, int end);
void		print_map(char **map);
int			count_char(char *doc, int c);
void		map_get_format(char **map, t_data *data);
int			**allocate_scaled_map(t_point max);

//	MOUSE EVENTS
int			mouse_update_env(int key, int x, int y, t_data *data);
int			mouse_update_view(int key, int x, int y, t_data *data);

//	KEY EVENTS
int			key_update_env(int key, t_data *data);
int			key_update_position(int key, t_data *data, t_player *p);

//	GENERAL EVENTS
int			close_window(void *param);
int			mouse_move(int x, int y, t_data *data);
int			compute_frame(void *param);

//	EVENTS CHECKERS
void		load_length(t_data *data, t_ray *ray, int ra);
void		check_player_direction(t_data *data);

//	UTILS
void		exit_game(t_data *data);
void		free_data(t_data *data);
void		int_array_free(int **array);

//	RAYCASTING
void		raycasting(t_data *data);
void		check_player_direction(t_data *data);
t_texture	load_texture(char *path, t_data *data);

//	RAYCASTING COMPUTING
void		adjust_ray_data(t_ray *ray, t_data *data);
void		update_ray_pos(t_ray *ray, t_map *map);
t_texture	*select_texture(t_data *data, t_ray *ray);
int			wall_hit(int mapXidx, int mapYidx, t_ray *ray, t_map *map);

//	MATH
int			extract_length(t_data *data, int x, int y);

//	DISPLAY
void		my_mlx_pixel_put(t_data *data, int x, int y, int color);
void		my_mlx_pixel_put2(t_data *data, int x, int y, int color);
//void		draw_vertical_line(t_column_draw *cd, int start, int end);
//void		draw_textured_line(t_texdraw *d);
void		draw_line(t_point p0, t_point p1, t_data *data);
void		draw_2d_wall(t_data *data, int x, int y, int color);
void		draw_2d_map(t_data *data);
void		draw_player(t_data *data);
void		draw_minimap(t_data *data);

//	MENU
int			cast_menu(t_data *data, char place[MENU_SLOTS]);
int			key_menu(int key, t_data *data);
void		define_action(t_data *data);
void		show_cmds(t_data *data);

//	UTILS
char		**ft_strsplit(char *str, char *sep);
int			ft_isspace(int c);
void		ft_suppress(char **content, char *to_suppress);
int			ft_atohx(char *n);
char		*copy_from(char **str, size_t here);
char		*ft_strstr(char *str, char *to_find, size_t len);
char		*ft_strrstr(char *str, char *to_find, size_t len);

#endif
