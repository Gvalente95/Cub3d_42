UNAME_S := $(shell uname -s)

CFLAGS = -g
ifeq ($(UNAME_S),Linux)
    MLX_DIR = mlx_linux
    MLX_LIBS = -lX11 -lXext -lXcursor -lGL -lz -lm
	CFLAGS += -DIS_LINUX
else
    MLX_DIR = mlx_mac
    MLX_LIBS = -framework OpenGL -framework AppKit -lz
    CFLAGS += -DMAC
endif

SOURCES = \
c_files/input/input_tools.c c_files/input/input_mouse.c c_files/input/input_key.c c_files/free/free_image.c \
c_files/free/free_elements.c c_files/free/free.c c_files/var/ray_threads_tools.c c_files/var/init_text.c \
c_files/var/text_fast.c c_files/var/text.c c_files/var/audio.c c_files/var/colors.c c_files/vectors/vector_rotate.c \
c_files/vectors/vector_math_scale.c c_files/vectors/vector_2.c c_files/vectors/vectors_show.c c_files/vectors/vector_3floatb.c \
c_files/vectors/vector_3float.c c_files/vectors/vector_color.c c_files/vectors/vector_4.c c_files/vectors/vector_2f_b.c \
c_files/vectors/Vector_3b.c c_files/vectors/vector4float.c c_files/vectors/vector_math_sub.c c_files/vectors/vector_math_add.c \
c_files/vectors/vector_math_f.c c_files/vectors/vector_2f.c c_files/vectors/vector_2b.c c_files/vectors/vector_3.c \
c_files/math/math_tools.c c_files/time/time2.c c_files/time/time.c c_files/init_wrapper.c c_files/string/string_3.c \
c_files/string/string.c c_files/string/string_2.c c_files/draw/image_init.c c_files/draw/draw_pixels_2.c \
c_files/draw/draw_image.c c_files/draw/edit_img.c c_files/draw/draw_tools.c c_files/draw/draw_triangle.c \
c_files/draw/draw_rot.c c_files/draw/image_load.c c_files/draw/image_resize.c c_files/draw/draw_color.c c_files/input/lock_mouse.c \
c_files/draw/draw_shape.c c_files/draw/draw_pixels.c c_files/draw/image_rotate.c c_files/draw/draw_triangle_tools.c \
c_files/draw/draw_cube.c c_files/draw/draw_pixels_3.c

OBJ = $(SOURCES:.c=.o)

LIBNAME = mlx_wrapper.a

INCLUDES = -I mlx_wrapper/$(MLX_DIR) -I libft

MLXFLAGS = -L mlx_wrapper/$(MLX_DIR) $(MLX_LIBS)

all: $(LIBNAME)

$(LIBNAME): $(OBJ)
	ar rcs $(LIBNAME) $(OBJ)
	@echo "$(LIBNAME) compiled"

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(LIBNAME)

re: fclean all

.PHONY: all clean fclean re
.SILENT:
