#ifndef CUB3D_H

# define CUB3D_H

# include "minilibx_mms/mlx.h"
//# include "mini_opengl/mlx.h"
# include "mlx.h"
# include "key_macos.h"
# include "bitmap.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <math.h>

# define	BUFFER_SIZE 100

typedef struct	s_list
{
	void			*content;
	struct s_list	*next;
}				t_list;

typedef struct	s_map
{
	int		fd;
	int		bonus_on;
	char	*north;
	char	*south;
	char	*west;
	char	*east;
	char	*size;
	int		w;
	int		h;
	char	*item;
	char	*floor;
	int		floor_color;
	char	*ceil;
	int		ceil_color;
	char	start_dir;
	t_list	*lst;
	int		**map;
	int		map_x;
	int		map_y;
	int		start_x;
	int		start_y;
	int		sprite_count;
	int		bitmap_check;
}				t_map;

typedef struct	s_sprite
{
	double	x;
	double	y;
	int		texture;
}				t_sprite;

int		get_next_line(int fd, char **line);
char	*ft_strdup_size(const char *s1, int left, int right);
int		ft_strlen(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
void	cub_play(t_map *m);
void	map_array_free(t_map *m, int n, int type);
#endif
