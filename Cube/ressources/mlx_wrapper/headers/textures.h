/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:13:20 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/05 13:56:11 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURES_H
# define TEXTURES_H

# include "../mlx_utils.h"

# define ENT_FRAMES_MAX	200

typedef struct s_draw_d
{
	t_image	*src;
	t_image	*dst;
	t_vec2	src_pos;
	t_vec2	dst_pos;
	t_vec2	pos;
	float	blend_factor;
	int		recolor;
}	t_draw_d;

typedef struct s_image_data
{
	t_vec2	size;
	t_vec2	pos;
	char	path[20];
	char	*addr;
	void	*img;
	int		*src;
	int		size_line;
	int		bpp;
	int		bps;
	int		endian;
	int		*scl_d;
}	t_image;

typedef struct s_texture_data
{
	int				e_scales[ENT_TYPE_LEN];
	t_vec2			e_sizes2d[ENT_TYPE_LEN];
	t_image			****mobs_txtrs;
	t_image			****mobs_txtrs_mini;
	t_image			***item_txtr;
	t_image			***wpn_txtr;
	t_image			***item_txtr_mini;
	t_image			***pkmn;
	t_image			**pkmns_mini;
	t_image			***wpn_txtr_2d;
	t_image			**wall_img;
	t_image			**wall_img2d;
	t_image			*door_txtr;
	t_image			**bush_txtr;
	t_image			*ext_wall;
	t_image			*ext_wall_mini;
	t_image			*grass_tile;
	t_image			*grass_mini;
	t_image			**tree_txtr;
	t_image			*door_txtr_mini;
	t_image			*font[256];
	t_image			*floor;
	t_image			*ceiling;
	const char		*ents_tp_map[ENT_TYPE_LEN];
	const char		*ents_types_names[ENT_TYPE_LEN];
	const char		*weapons_names[WEAPON_TYPE_LEN];
	const char		*mob_names[MOB_TYPE_LEN];
	const char		*item_names[PCKP_TYPE_LEN];
	const char		*ents_act_names[ENT_ACTION_LEN];
	const char		*pkmn_names[PKMN_TYPE_LEN];
	const char		*plr_act_names[PLR_ACTION_LEN];
	int				size_2d;
	int				feet_offsets[PKMN_TYPE_LEN];
}	t_texture_data;

typedef enum e_floor_type
{
	fe_grass,
	fe_tree,
	fe_bush,
	fe_type_len,
}	t_fe_type;

typedef struct s_floor_element
{
	t_fe_type		type;
	float			growth_factor;
	int				base_color;
	int				cut_len;
	int				active;
	int				height;
	int				age;
	int				cur_height;
	t_vec2			size;
	t_vec2			pos;
	int				was_drawn;
	int				set;
	int				height_max;
	int				end_y;
	float			fogalpha;
}	t_fe;

typedef struct s_env_manager
{
	t_fe			****grass;
	t_image			*grass_overlay;
	int				overlay_y_start;
	int				last_y_start;
}	t_env_manager;

//		draw/draw_img.c
//				data: x = color | y = intensity | z = overlap_color
void			draw_clr_img(t_image *src, t_image *dst, t_vec2 pos, t_vec3 d);
int				draw_img(t_image *src, t_image *dst, t_vec2 pos, int over);
void			flipx_image_data(t_image *img);
void			flipy_image_data(t_image *img);
void			flush_img(t_image *src, int clr, float alpha, int ign_alph_px);

//		draw/draw_img_2.c
//				end.x = dst_end.x | end.y = dst_end.y | end.z = color over
void			draw_trimmed(t_image *src, t_image *dst, t_vec2 p, t_vec3 end);
void			draw_sphere(t_image *dst, t_vec2 pos, t_vec2 sz, t_vec3 data);
void			draw_rotated(t_md *md, t_image *src, t_image *dst, t_vec3f p);
void			draw_contour(t_image *img, int color, int thick, float opcty);

//		draw/images_data.c
t_image			*init_abs_img(t_md *md, t_vec2 img_size, char *path);
t_image			*init_img(t_md *md, t_vec2 img_size, char *path, int color);
t_image			**init_images(t_md *md, t_vec2 size, char *path);
t_image			*scale_imgd(t_md *md, t_image *imgd, t_vec2 new_sz, int aspect);

//		draw/images_load.c
char			**get_frames(t_md *md, char *dir, int *amount, int max);
void			*ld_txtr(t_md *md, t_vec2 *end_sz, char *path, t_vec2 data);
void			**ld_txtrs(t_md *md, t_vec2 end_sz, char *path, int aspect);
void			copy_image_src(t_md *md, t_image *new_img, t_image *src_img);
t_image			*copy_image(t_md *md, t_image *src_img, t_vec2 size, int color);

//		draw/draw_alpha.c
int				set_alpha(int color, float new_alpha);
float			get_alpha(int color);
void			fill_transparency(t_image *src, int color);
void			draw_alpha_img(t_image *src, t_image *dst, t_vec2 p, float op);
void			init_draw_d(t_draw_d *draw_d, t_image *src, t_image *dst, \
	t_vec2 src_pos);

//		draw/images_b.c
void			set_img_color(t_image *img, t_vec2 size, int col, float str);
void			*get_image_copy(t_md *md, void *src, t_vec2 src_size);

//		draw/draw_pixels.c
int				blend_color(int color_a, int color_b, float opacity);
int				put_pxl_if_vis(t_draw_d *dd, int over, int blend, float alpha);
int				draw_pixel(t_image *dst, t_vec2 pos, int color, float alpha);
int				draw_pixels(t_image *dst, t_vec2 pos, t_vec2 draw_sz, int clr);
void			draw_line(t_image *dst, t_vec2 start, t_vec2 end, t_vec2 clr_d);
int				draw_safe_pxls(t_image *img, t_vec2 p, t_vec2 draw_sz, int clr);
int				draw_safe_pxl(t_image *img, t_vec2 p, t_vec2 ovrcl, float opct);

//		draw/images_a.c
t_vec3f			set_new_size(t_image *q, t_vec2 *old_size, t_vec2 *new_size);
void			*scale_img_keep_ratio(t_md *md, void *img, \
	t_vec2 *old_size, t_vec2 new_size);
void			*resize_img(t_md *md, void *img, t_vec2 *old_sz, t_vec2 new_sz);
void			replace_image(t_md *md, t_image **dst, t_image *src);
void			draw_rot_z(t_md *md, t_image *src, t_image *dst, t_vec3f p);
void			draw_rot_y(t_md *md, t_image *src, t_image *dst, t_vec3f p);
void			draw_rot_yz(t_md *md, t_image *src, t_image *dst, t_vec4f p);
void			draw_img_yaw(t_md *md, t_image *src, t_image *dst, t_vec3f p);
void			draw_img_pitch(t_md *md, t_image *src, t_image *dst, t_vec3f p);
void			remove_img_color(t_image *img, int remove_color);

#endif