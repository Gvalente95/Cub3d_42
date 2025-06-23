SRC_UTILS	= \
c_files/menu/menu_render.c c_files/menu/menu_tools.c c_files/menu/menu_update.c c_files/tools/debug.c \
c_files/tools/ftoa.c c_files/tools/image_tools.c c_files/tools/map_gen.c c_files/tools/map_gen_2.c c_files/tools/map_gen_tools.c \
c_files/tools/parse_tools.c c_files/pkmn_battle/actions.c c_files/pkmn_battle/init.c c_files/pkmn_battle/render.c \
c_files/pkmn_battle/render_combat.c c_files/pkmn_battle/update.c c_files/pkmn_battle/utils.c c_files/update/collisions.c \
c_files/update/collisions_portal.c c_files/update/mapped_ents.c c_files/update/update.c c_files/update/update_autocam.c \
c_files/update/update_autocam_2.c c_files/update/update_camera.c c_files/update/update_ents.c c_files/update/update_input.c \
c_files/update/update_plr.c c_files/update/update_plr_movement.c c_files/update/update_time_events.c \
c_files/update/update_var.c c_files/update/update_ent_movement.c c_files/init/init_.c c_files/init/init_background.c \
c_files/init/init_ent_frames.c c_files/init/init_entities.c c_files/init/init_frames.c c_files/init/init_frames2.c \
c_files/init/init_grass.c c_files/init/init_hud.c c_files/init/init_labels.c c_files/init/init_map.c \
c_files/init/init_map_data.c c_files/init/init_map_validator.c c_files/init/init_menu.c c_files/init/init_menu_elements.c \
c_files/init/init_minimap.c c_files/init/init_out_map.c c_files/init/init_threads.c c_files/raycasting/draw_ceiling.c \
c_files/raycasting/draw_fe.c c_files/raycasting/draw_floor.c c_files/raycasting/draw_sprite_column.c \
c_files/raycasting/draw_wall_column.c c_files/raycasting/portal_draw.c c_files/raycasting/portal_raycast.c \
c_files/raycasting/ray_cast.c c_files/raycasting/ray_cast_draw_ents.c c_files/raycasting/ray_cast_pointed.c \
c_files/raycasting/ray_cast_threads.c c_files/raycasting/ray_cast_threads_Loop.c c_files/raycasting/ray_move.c \
c_files/raycasting/ray_move_dda.c c_files/raycasting/ray_tools.c c_files/raycasting/ray_tools_2.c c_files/items/held_item.c \
c_files/items/init_inventory.c c_files/items/inventory.c c_files/items/render_held_item.c c_files/items/render_inv_team.c \
c_files/items/render_inventory.c c_files/items/update_inventory.c c_files/render/filters_0.c c_files/render/filters_1.c \
c_files/render/filters_2.c c_files/render/render.c c_files/render/render_background.c c_files/render/render_hud.c \
c_files/render/render_minimap.c c_files/render/render_minimap_cmp.c c_files/menu/menu_update_sliders.c

RMAP_SRC	= rmap.c
SRC			= main.c
GAME_NAME	= cube
GOF_SRC		= game_of_life.c
PUZ_SRC		= puzzle.c

GNL_DIR		= ressources/gnl/
GNL			= $(GNL_DIR)get_next_line.a

LISTS_DIR	= ressources/lists/
LISTS		= $(LISTS_DIR)lists.a

LIBFT_DIR	= ressources/mlx_wrapper/libft/
LIBFT		= $(LIBFT_DIR)libft.a

MLX_WRP_DIR = ressources/mlx_wrapper/
MLX_WRAPPER = $(MLX_WRP_DIR)mlx_wrapper.a

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	PLATFORM = linux
	MLXFLAGS = -L $(MLX_WRP_DIR)mlx_linux -lmlx -lX11 -lXext -lXcursor -lGL -lz -lm
	CFLAGS += -DIS_LINUX
else
	PLATFORM = mac
	MLXFLAGS = -L $(MLX_WRP_DIR)mlx_mac -lmlx -framework OpenGL -framework AppKit -lz
endif

CC			= gcc
CFLAGS		+= -Wall -Wextra -Werror -g
SAN			= -fsanitize=address

$(MLX_WRAPPER):
	make -C $(MLX_WRP_DIR) PLATFORM=$(PLATFORM) --no-print-directory

$(LISTS):
	make -C $(LISTS_DIR) --no-print-directory

$(GNL):
	make -C $(GNL_DIR) --no-print-directory

$(LIBFT):
	make -C $(LIBFT_DIR) --no-print-directory

$(GAME_NAME): $(SRC) $(SRC_UTILS) $(MLX_WRAPPER) $(LISTS) $(GNL) $(LIBFT)
	$(CC) $(CFLAGS) $(SRC) $(SRC_UTILS) $(MLX_WRAPPER) $(MLXFLAGS) $(LISTS) $(GNL) $(LIBFT) -o $(GAME_NAME)
	@echo "$(GAME_NAME) Generated on $(UNAME_S)"

normall:
	norminette cube.h c_files ressources/mlx_wrapper/c_files ressources/mlx_wrapper/headers ressources/mlx_wrapper/mlx_utils.h

leaks: all
	leaks --atExit -- ./cube

all: $(GAME_NAME)

rmap: $(RMAP_SRC) $(SRC_UTILS) $(MLX_WRAPPER) $(LISTS) $(GNL) $(LIBFT)
	$(CC) $(SAN) $(CFLAGS) $(RMAP_SRC) $(SRC_UTILS) $(MLX_WRAPPER) $(MLXFLAGS) $(LISTS) $(GNL) $(LIBFT) -o map
	@echo "map Generated on $(UNAME_S)"

gof: $(GOF_SRC) $(MLX_WRAPPER) $(LISTS) $(GNL) $(LIBFT)
	$(CC) $(SAN) $(CFLAGS) $(GOF_SRC) $(MLX_WRAPPER) $(MLXFLAGS) $(LISTS) $(GNL) $(LIBFT) -o gof

puz: $(PUZ_SRC) $(MLX_WRAPPER) $(LISTS) $(GNL) $(LIBFT)
	$(CC) $(SAN) $(CFLAGS) $(PUZ_SRC) $(MLX_WRAPPER) $(MLXFLAGS) $(LISTS) $(GNL) $(LIBFT) -o puz


debug: $(GAME_NAME) $(MLX_WRAPPER) $(LISTS) $(GNL) $(LIBFT) $(SRC) $(SRC_UTILS)
	$(CC) $(SAN) $(CFLAGS) $(SRC) $(SRC_UTILS) $(MLX_WRAPPER) $(MLXFLAGS) $(LISTS) $(GNL) $(LIBFT) -o $(GAME_NAME)
	@echo "$(GAME_NAME) debug Generated"

clean:
	make -C $(LIBFT_DIR) --no-print-directory clean
	make -C $(MLX_WRP_DIR) --no-print-directory clean
	make -C $(LISTS_DIR) --no-print-directory clean
	make -C $(GNL_DIR) --no-print-directory clean
	rm -f $(GAME_NAME)
	@echo "$(GAME_NAME) $(CYAN)cleaned$(RESET)"

quiet_clean:
	make -C $(LIBFT_DIR) --no-print-directory clean QUIET=1
	make -C $(MLX_WRP_DIR) --no-print-directory clean QUIET=1
	make -C $(LISTS_DIR) --no-print-directory clean QUIET=1
	make -C $(GNL_DIR) --no-print-directory clean QUIET=1
	rm -f $(GAME_NAME)

fclean: quiet_clean
	make -C $(LIBFT_DIR) --no-print-directory fclean QUIET=1
	make -C $(MLX_WRP_DIR) --no-print-directory fclean QUIET=1
	make -C $(LISTS_DIR) --no-print-directory fclean QUIET=1
	make -C $(GNL_DIR) --no-print-directory fclean QUIET=1
	@echo "$(GAME_NAME) $(CYAN)force cleaned$(RESET)"

re: fclean all

.PHONY: all re bonus clean fclean normall leaks debug rmap
