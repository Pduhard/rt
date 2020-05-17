#__________CONFIG__________#


NAME		=	rt

FLAGS		=	-Wall -Wextra -Werror -g -O3 -march=native -flto -ffast-math

LIB_FLAGS	=	-L$(LIB_PATH) $(LIB_FLAG)

INCLUDES	=	rt.h \
				key_linux.h\

SRC_PATH	=	./srcs/
BIN_PATH	=	./bins/
INC_PATH	=	./includes/
LIB_PATH	=	./libft/
EXT_LIB		=	./external_libs

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	INSTALLSDL := $(shell  sudo apt-get -y install libsdl2-dev)\
					$(shell  sudo apt-get -y install libsdl2-image-dev)\
	FLAGS		+=	-fPIC
	FRAMEWORK	=	-lSDL2 -I/usr/include/SDL2 -I/usr/include/SDL -lSDL2_image
	CC		=	clang
	MLX_PATH	=	$(EXT_LIB)/minilibx/
	MLX_FLAGS	=	-L$(MLX_PATH) -lX11 -lXext -lm -lbsd $(MLX_FLAG)
else
	CC		=	gcc
	FRAMEWORK	=	-framework OpenGL -framework AppKit -I./frameworks/SDL2_image.framework/Headers/ -framework SDL2 -F ./frameworks -framework SDL2_image -rpath ./frameworks
	MLX_PATH	=	$(EXT_LIB)/minilibx_macos/
	MLX_FLAGS	=	-L$(MLX_PATH) $(MLX_FLAG)
endif
#MLX_PATH	=	$(EXT_LIB)/minilibx_mms_20191207_beta/ //new lib in beta
 #	loading.c


SRC			=	main.c					\
				init.c					\
				matrix.c				\
				vector.c				\
				render.c				\
				cut.c					\
				cone.c				\
				angles_utils.c			\
				conf.c					\
				parse.c					\
				parse_scene_element.c	\
				parse_object.c			\
				parse_lights.c			\
				parse_composed_object.c	\
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
				perlin_utils.c	\
				init_perlin.c		\
				fbm.c					\
				sphere.c				\
				plane.c					\
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
				free.c					\
				parse_cylinder.c	\
				parse_cone.c			\
				parse_sphere.c		\
				parse_plane.c				\
				parse_triangle.c		\
				parse_horse_saddle.c \
				parse_monkey_saddle.c	\
				parse_cyclide.c			\
				parse_fermat.c			\
				parse_ellipsoid.c		\
				parse_hyperboloid.c	\
				parse_moebius.c			\
				skybox.c						\
				# $(addprefix $(SRC_SHAPES_PATH), $(SHAPES))
				# shapes/cone.c		\

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
	@$(CC) $(FLAGS) -o $@ $^ $(LIB_FLAGS) -lpthread -I $(INC_PATH) $(MLX_FLAGS) $(FRAMEWORK)
#	@$(CC) -I $(INC_PATH) $(FLAGS) -lpthread $(LIB_FLAGS) $(MLX_FLAGS) -o $@ $^
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
