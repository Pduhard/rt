#__________CONFIG__________#

NAME		=	rt

CC			=	gcc
FLAGS		=	-Wall -Wextra -Werror -O3 -march=native -flto -ffast-math
FRAMEWORK	=	-framework OpenGL -framework AppKit -I./frameworks/SDL2_image.framework/Headers/ -framework SDL2 -F ./frameworks -framework SDL2_image -rpath ./frameworks
LIB_FLAGS	=	-L$(LIB_PATH) $(LIB_FLAG)
MLX_FLAGS	=	-L$(MLX_PATH) $(MLX_FLAG)
INCLUDES	=	rt.h \

SRC_PATH	=	./srcs/
BIN_PATH	=	./bins/
INC_PATH	=	./includes/
LIB_PATH	=	./libft/
EXT_LIB		=	./external_libs
MLX_PATH	=	$(EXT_LIB)/minilibx_macos/
#MLX_PATH	=	$(EXT_LIB)/minilibx_mms_20191207_beta/ //new lib in beta

SRC			=	main.c					\
				init.c					\
				loading.c				\
				matrix.c				\
				vector.c				\
				render.c				\
				cut.c					\
				angles_utils.c			\
				conf.c					\
				parse.c					\
				parse_scene_element.c	\
				parse_object.c			\
				parse_lights.c			\
				parse_composed_object.c	\
				parse_flat_object.c		\
				parse_bumpmap.c			\
				parse_cutting.c			\
				parse_texture.c			\
				parse_proc_img.c		\
				parse_other_element.c	\
				parse_vecf.c			\
				add_object.c			\
				error.c					\
				texture.c				\
				bump_mapping.c			\
				perlin.c				\
				fbm.c					\
				sphere.c				\
				plane.c					\
				cone.c					\
				cube.c					\
				triangle.c				\
				moebius.c				\
				loop.c					\
				key_hook.c				\
				mouse_hook.c			\
				cylinder.c				\
				ellipsoid.c				\
				hyperboloid.c			\
				horse_saddle.c			\
				monkey_saddle.c			\
				cyclide.c				\
				fermat.c				\
				polynomial.c			\
				motion.c				\
				color_filter.c			\
				photon_map.c			\
				global_illumination.c	\
				random.c				\

BIN			=	$(SRC:.c=.o)

LIB_FLAG			=	-lft
LIB					=	libft.a
MLX_FLAG			=	-lmlx
MLX					=	libmlx.a
#MLX					=	libmlx.dylib //new lib in beta

SRCS		=	$(addprefix $(SRC_PATH), $(SRC))
BINS		=	$(addprefix $(BIN_PATH), $(BIN))
LIBS		=	$(addprefix $(LIB_PATH), $(LIB))
INCS		=	$(addprefix $(INC_PATH), $(INCLUDES))
MLXS		=	$(addprefix $(MLX_PATH), $(MLX))

.PHONY: all clean fclean re lib

#__________COLORS__________#

R			=	\033[0;31m
G			=	\033[32;7m
B			=	\033[0;34m
N			=	\33[0m

#__________RULES__________#

all: make_libft $(NAME)

$(NAME): $(LIBS) $(MLXS) $(BINS)

	@$(CC) -I $(INC_PATH) $(FLAGS) -lpthread $(LIB_FLAGS) $(MLX_FLAGS) $(FRAMEWORK) $^ -o $@
	@echo "\n\n$(B)[EXECUTABLE \"$(NAME)\" READY]\n"

make_libft:

	@make -C $(LIB_PATH)

$(LIBS):

	@make -C $(LIB_PATH)

$(MLXS):

	@make -C $(MLX_PATH)

$(PNGS):

	@make -C $(PNG_PATH)

$(BIN_PATH)%.o: $(SRC_PATH)%.c $(INCS)

	@mkdir -p $(BIN_PATH) || true
	@$(CC) $(FLAGS) -I $(INC_PATH) -o $@ -c $< && echo "${G} \c"

clean:

	@make clean -C $(LIB_PATH)
	@make clean -C $(MLX_PATH)
	@echo "${R}[CLEANING $(NAME) BINARIES]"
	@rm -f $(BINS)

fclean: clean

	@echo "\n${R}[REMOVING "libft.a"]"
	@rm -rf $(LIBS)
	@echo "\n${R}[REMOVING "libmlx.a"]"
	@rm -rf $(MLXS)
	@echo "\n${R}[REMOVING "$(NAME)"]\n"
	@rm -f $(NAME)

re: fclean all
