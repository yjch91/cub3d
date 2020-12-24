CC = gcc

CFLAGS = -Wall -Werror -Wextra

CFLAGS_MLX = -framework OpenGL -framework Appkit

SRCS = gnl.c gnl_utils.c cub_play.c ft_itoa.c control_key_w.c control_key_s.c control_key_a_d.c save_bitmap.c \
	   control_jump.c key_press_release.c control_weap.c info_init.c sprite_info_init.c buffer_init.c \
	   control_mouse.c sprite_utils.c minimap.c lifebar.c ceilfloor_calc.c shade_color.c wall_calc.c \
	   find_hit_wall.c sprite_calc.c imagedata_init.c wall_check.c wall_eight_dir_check.c fill_map_array.c \
	   ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstnew.c ft_lstsize.c ft_strnstr.c \
	   ft_strdup.c ft_strtrim.c ft_atoi.c gnl_map_info.c gnl_info.c gnl_info_utils.c gnl_info_utils2.c \
	   gnl_info_utils3.c cubfile_info_lst_free.c

MAINSRCS = main.c

BMAINSRCS = main_bonus.c

MAINOBJS = ${MAINSRCS:.c=.o}

BMAINOBJS = ${BMAINSRCS:.c=.o}

OBJS = ${SRCS:.c=.o}

NAME = cub3D

LIBMMS = libmlx.dylib

LIBOPENGL = libmlx.a

.c.o:	${LIBMMS} ${LIBOPENGL}
		${CC} ${CFLAGS} -c -I . $< -o $(<:.c=.o)

all:	${NAME}

${LIBMMS}:
			$(MAKE) -C ./minilibx_mms

${LIBOPENGL}:
			${MAKE} -C ./mini_opengl

${NAME}:	${OBJS} ${MAINOBJS} ${LIBMMS} ${LIBOPENGL}
			cp ./minilibx_mms/libmlx.dylib ./
			${CC} -L./mini_opengl -lmlx ${CFLAGS_MLX} ${OBJS} ${MAINOBJS} ${LIBMMS} -o ${NAME}
			
clean:
		rm -f ${OBJS} ${MAINOBJS} ${BMAINOBJS} ${LIBMMS} ${LIBOPENGL}

fclean:	clean
		rm -f ${NAME}
		${MAKE} -C ./minilibx_mms clean
		${MAKE} -C ./mini_opengl clean

re:		fclean ${NAME}

bonus:	${OBJS} ${BMAINOBJS} ${LIBMMS} ${LIBOPENGL}
		cp ./minilibx_mms/libmlx.dylib ./
		${CC} -L./mini_opengl -lmlx ${CFLAGS_MLX} ${OBJS} ${BMAINOBJS} ${LIBMMS} -o ${NAME}

.PHONY:	all clean fclean re
