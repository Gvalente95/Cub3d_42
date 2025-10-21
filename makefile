SRC_UTILS	= \
c_files/menu/menu_render.c c_files/menu/menu_tools.c c_files/menu/menu_update.c c_files/tools/debug.c \
c_files/tools/ftoa.c c_files/tools/image_tools.c c_files/tools/map_gen.c c_files/tools/map_gen_2.c c_files/tools/map_gen_tools.c \
c_files/tools/parse_tools.c c_files/pkmn_battle/ba_Actions.c c_files/pkmn_battle/ba_Init.c c_files/pkmn_battle/ba_Render.c \
c_files/pkmn_battle/ba_RenderHud.c c_files/pkmn_battle/ba_Update.c c_files/pkmn_battle/ba_Utils.c c_files/update/collisions.c \
c_files/update/collisions_portal.c c_files/update/mapped_ents.c c_files/update/update.c c_files/update/update_autocam.c \
c_files/update/update_autocam_2.c c_files/update/update_camera.c c_files/update/update_ents.c c_files/update/update_input.c \
c_files/update/update_plr.c c_files/update/update_plr_movement.c c_files/update/update_time_events.c c_files/pkmn_battle/ba_Transition.c \
c_files/update/update_var.c c_files/update/update_ent_movement.c c_files/init/init_.c c_files/init/init_background.c \
c_files/init/init_ent_frames.c c_files/init/init_entities.c c_files/init/init_frames.c c_files/init/init_frames2.c \
c_files/init/init_grass.c c_files/init/init_hud.c c_files/init/init_labels.c c_files/init/init_map.c \
c_files/init/init_map_data.c c_files/init/init_map_validator.c c_files/init/init_menu.c c_files/init/init_menu_elements.c \
c_files/init/init_minimap.c c_files/init/init_out_map.c c_files/init/init_threads.c c_files/raycasting/draw_ceiling.c \
c_files/raycasting/draw_fe.c c_files/raycasting/draw_floor.c c_files/raycasting/draw_sprite_column.c c_files/init/init_pokemon_data.c \
c_files/raycasting/draw_wall_column.c c_files/raycasting/portal_draw.c c_files/raycasting/portal_raycast.c \
c_files/raycasting/ray_cast.c c_files/raycasting/ray_cast_draw_ents.c c_files/raycasting/ray_cast_pointed.c \
c_files/raycasting/ray_cast_threads.c c_files/raycasting/ray_cast_threads_Loop.c c_files/raycasting/ray_move.c \
c_files/raycasting/ray_tools.c c_files/raycasting/ray_tools_2.c c_files/items/held_item.c c_files/tools/get_labels.c \
c_files/items/init_inventory.c c_files/items/inventory.c c_files/items/render_held_item.c c_files/items/render_inv_team.c \
c_files/items/render_inventory.c c_files/items/update_inventory.c c_files/render/filters_0.c c_files/render/filters_1.c \
c_files/render/filters_2.c c_files/render/render.c c_files/render/render_background.c c_files/render/render_hud.c \
c_files/render/render_minimap.c c_files/render/render_minimap_cmp.c c_files/menu/menu_update_sliders.c c_files/init/init_au.c \

BA_RMAP_SRC	= rmap.c
SRC			= main.c
GAME_NAME	= cub3d

GNL_DIR		= ressources/gnl/
GNL			= $(GNL_DIR)get_next_line.a

LISTS_DIR	= ressources/lists/
LISTS		= $(LISTS_DIR)lists.a

LIBFT_DIR	= ressources/mlx_wrapper/libft/
LIBFT		= $(LIBFT_DIR)libft.a

MLX_WRP_DIR = ressources/mlx_wrapper/
MLX_WRAPPER = $(MLX_WRP_DIR)mlx_wrapper.a

OBJDIR := build

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

OBJ_MAIN	= $(SRC:.c=.o)
OBJ_UTILS	= $(SRC_UTILS:.c=.o)
OBJ_ALL		= $(OBJ_MAIN) $(OBJ_UTILS)

INCLUDES	=

all: $(MLX_WRAPPER) $(LISTS) $(GNL) $(LIBFT) $(GAME_NAME)

GREEN = 	\033[0;32m
RESET = 	\033[0m

$(MLX_WRAPPER):
	@$(MAKE) -C $(MLX_WRP_DIR) PLATFORM=$(PLATFORM) --no-print-directory

$(LISTS):
	@$(MAKE) -C $(LISTS_DIR) --no-print-directory

$(GNL):
	@$(MAKE) -C $(GNL_DIR) --no-print-directory

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

SRC_ALL := $(SRC) $(SRC_UTILS)
OBJ     := $(patsubst %.c,$(OBJDIR)/%.o,$(SRC_ALL))
$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(GAME_NAME): $(OBJ) $(MLX_WRAPPER) $(LISTS) $(GNL) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) $(MLX_WRAPPER) $(MLXFLAGS) $(LISTS) $(GNL) $(LIBFT) -o $(GAME_NAME)

normall:
	norminette cube.h c_files ressources/mlx_wrapper/c_files ressources/mlx_wrapper/headers ressources/mlx_wrapper/mlx_utils.h

leaks: all
	leaks --atExit -- ./$(GAME_NAME)

rmap: $(BA_RMAP_SRC) $(SRC_UTILS) $(MLX_WRAPPER) $(LISTS) $(GNL) $(LIBFT)
	$(CC) $(SAN) $(CFLAGS) $(BA_RMAP_SRC) $(SRC_UTILS) $(MLX_WRAPPER) $(MLXFLAGS) $(LISTS) $(GNL) $(LIBFT) -o map
	echo "map Generated on $(UNAME_S)"

debug: $(OBJ) $(MLX_WRAPPER) $(LISTS) $(GNL) $(LIBFT)
	@$(CC) $(SAN) $(CFLAGS) $(OBJ) $(MLX_WRAPPER) $(MLXFLAGS) $(LISTS) $(GNL) $(LIBFT) -o $(GAME_NAME)
	@echo "$(GAME_NAME) debug Generated"

clean:
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory clean
	@$(MAKE) -C $(MLX_WRP_DIR) --no-print-directory clean
	@$(MAKE) -C $(LISTS_DIR) --no-print-directory clean
	@$(MAKE) -C $(GNL_DIR) --no-print-directory clean
	@rm -f $(OBJ_ALL) $(OBJ_MAIN)
	@rm -rf $(OBJDIR) $(GAME_NAME)

quiet_clean:
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory clean QUIET=1
	@$(MAKE) -C $(MLX_WRP_DIR) --no-print-directory clean QUIET=1
	@$(MAKE) -C $(LISTS_DIR) --no-print-directory clean QUIET=1
	@$(MAKE) -C $(GNL_DIR) --no-print-directory clean QUIET=1
	@rm -f $(OBJ_ALL) $(OBJ_MAIN)
	@rm -f $(GAME_NAME)
	@echo "$(GREEN)cub3d clean$(RESET)"

fclean: quiet_clean
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory fclean QUIET=1
	@$(MAKE) -C $(MLX_WRP_DIR) --no-print-directory fclean QUIET=1
	@$(MAKE) -C $(LISTS_DIR) --no-print-directory fclean QUIET=1
	@$(MAKE) -C $(GNL_DIR) --no-print-directory fclean QUIET=1
	@echo "$(GREEN)cub3d fclean$(RESET)"

re: fclean all

.PHONY: all re bonus clean fclean normall leaks debug rmap
