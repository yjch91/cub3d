CC = gcc

CFLAGS = -Wall -Werror -Wextra

CFLAGS_MLX = -framework OpenGL -framework Appkit

SRCS = gnl.c gnl_utils.c mycube.c

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
			cp ./mini_opengl/libmlx.a ./
			cp ./minilibx_mms/libmlx.dylib ./
			${CC} -lmlx ${CFLAGS_MLX} ${OBJS} ${MAINOBJS} ${LIBMMS} ${LIBOPENGL} -o ${NAME}
			
clean:
		rm -f ${OBJS} ${MAINOBJS} ${BMAINOBJS} ${LIBMMS} ${LIBOPENGL}

fclean:	clean
		rm -f ${NAME}
		${MAKE} -C ./minilibx_mms clean
		${MAKE} -C ./mini_opengl clean

re:		fclean ${NAME}

bonus:	${OBJS} ${BMAINOBJS} ${LIBMMS} ${LIBOPENGL}
		cp ./mini_opengl/libmlx.a ./
		cp ./minilibx_mms/libmlx.dylib ./
		${CC} -lmlx ${CFLAGS_MLX} ${OBJS} ${BMAINOBJS} ${LIBMMS} ${LIBOPENGL} -o ${NAME}

.PHONY:	all clean fclean re
