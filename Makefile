# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: aplat <aplat@student.le-101.fr>            +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2018/11/13 00:29:26 by pduhard-     #+#   ##    ##    #+#        #
#    Updated: 2020/01/27 18:23:16 by aplat       ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

#__________CONFIG__________#

NAME		=	rt

CC			=	gcc
FLAGS		=	-Wall -Werror -Wextra -O3 -march=native -flto -ffast-math
FRAMEWORK	=	-framework OpenGL -framework AppKit -framework SDL2 -F ./frameworks  -I./frameworks/sdl_image_framework/Headers/ -framework SDL2_image -rpath ./frameworks
LIB_FLAGS	=	-L$(LIB_PATH) $(LIB_FLAG)
MLX_FLAGS	=	-L$(MLX_PATH) $(MLX_FLAG)
INCLUDES	=	rt.h	\

SRC_PATH	=	./srcs/
BIN_PATH	=	./bins/
INC_PATH	=	./includes/
LIB_PATH	=	./libft/
MLX_PATH	=	./minilibx_macos/

SRC			=	main.c			\
				init.c			\
				matrix.c		\
				vector.c		\
				render.c		\
				angles_utils.c	\
				conf.c			\
				texture.c		\
				perlin.c		\
				sphere.c		\
				plane.c			\
				cone.c			\
				loop.c			\
				key_hook.c		\
				cylinder.c		\

BIN			=	$(SRC:.c=.o)

LIB_FLAG			=	-lft
LIB					=	libft.a
MLX_FLAG			=	-lmlx
MLX					=	libmlx.a

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
	@rm -rf $(LIB_PATH)/libft.a
	@echo "\n${R}[REMOVING "libmlx.a"]"
	@rm -rf $(MLX_PATH)/libmlx.a
	@echo "\n${R}[REMOVING "libpng.a"]\n"
	@rm -f $(NAME)

re: fclean all
