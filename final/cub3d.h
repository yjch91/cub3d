/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 03:40:50 by jayun             #+#    #+#             */
/*   Updated: 2020/12/27 22:19:36 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H

# define CUB3D_H

# include "minilibx_mms/mlx.h"
# include "mini_opengl/mlx.h"
# include "key_macos.h"
# include "bitmap.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <math.h>
# define BUFFER_SIZE	1
# define X_EVENT_KEY_PRESS	2
# define X_EVENT_KEY_RELEASE	3
# define X_EVENT_MOTION_NOTIFY	6
# define X_EVENT_KEY_EXIT	17

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

typedef struct	s_img
{
	void	*img;
	int		*data;
	int		size_l;
	int		bpp;
	int		endian;
	int		img_width;
	int		img_height;
}				t_img;

typedef struct	s_sprite
{
	double	x;
	double	y;
	int		texture;
}				t_sprite;

typedef struct	s_info
{
	void		*mlx;
	void		*win;
	t_img		img;
	t_img		img2;
	int			**buf;
	t_img		texture[14];
	t_map		m;
	int			texnum;
	int			winsize_w;
	int			winsize_h;
	int			map_w;
	int			map_h;
	int			**map;
	int			bonus_on;
	int			bitmap_check;
	t_bit		bit;
	double		pos_x;
	double		pos_y;
	double		dir_x;
	double		dir_y;
	double		plane_x;
	double		plane_y;
	double		movespeed;
	double		rotspeed;
	double		camera_x;
	double		raydir_x;
	double		raydir_y;
	int			map_x;
	int			map_y;
	double		sidedist_x;
	double		sidedist_y;
	double		deltadist_x;
	double		deltadist_y;
	double		perpwalldist;
	int			side;
	int			lineheight;
	int			drawstart;
	int			drawend;
	double		wall_x;
	double		step;
	int			tex_x;
	int			tex_y;
	double		texpos;
	int			color;
	int			is_floor;
	double		raydir_x0;
	double		raydir_y0;
	double		raydir_x1;
	double		raydir_y1;
	int			p;
	double		posz;
	double		rowdist;
	double		fstep_x;
	double		fstep_y;
	double		floor_x;
	double		floor_y;
	int			cell_x;
	int			cell_y;
	int			step_x;
	int			step_y;
	int			floor_color;
	int			ceil_color;
	t_sprite	*sprite;
	double		*zbuffer;
	int			*sprite_order;
	double		*sprite_dist;
	int			sprite_count;
	double		sprite_x;
	double		sprite_y;
	double		invdet;
	double		transform_x;
	double		transform_y;
	int			spritescreen_x;
	int			sprite_h;
	int			sprite_w;
	int			drawstart_x;
	int			drawstart_y;
	int			drawend_x;
	int			drawend_y;
	int			flag_w;
	int			flag_a;
	int			flag_s;
	int			flag_d;
	int			flag_c;
	int			flag_sp;
	int			flag_sky;
	int			flag_weap;
	int			flag_shot;
	double		pitch;
	double		jump;
	double		hp;
	double		shot_x;
	double		shot_y;
	double		sh;
	int			mouse_x;
}				t_info;

int				g_map_empty;
static char		*g_save;
int				get_next_line(int fd, char **line);
char			*ft_strdup(const char *s1);
int				ft_strlen(const char *s);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strnstr(const char *haystack,
		const char *needle, size_t len);
char			*ft_strtrim(char const *s1, char const *set);
int				ft_atoi(const char *str);
void			cub_play(t_map *m);
void			map_array_free(t_map *m, int n, int type);
char			*ft_itoa(int n);
void			all_free(t_info *info, t_map *m);
void			remove_sprite(t_info *info, int x, int y);
void			control_key_w(t_info *info);
void			control_key_s(t_info *info);
void			control_key_a(t_info *info);
void			control_key_d(t_info *info);
void			save_bitmap(t_info *info);
void			control_jump(t_info *info);
int				key_press(int key, t_info *info);
int				key_release(int key, t_info *info);
int				button_redcross(t_info *info);
void			weap_shot(t_info *info);
void			control_weap(t_info *info);
void			info_init(t_info *info, t_map *m);
void			winsize_init(t_info *info, t_map *m);
void			info_init2(t_info *info);
void			imagedata_init(t_info *info, t_map *m);
void			win_init(t_info *info, t_map *m);
void			img_init(t_info *info, t_map *m, t_img *img);
void			fill_sprite_info(t_info *info);
int				sprite_alloc(t_info *info, t_map *m);
int				buf_alloc_init(t_info *info, t_map *m);
int				mouse_move(int x, int y, t_info *info);
void			remove_sprite(t_info *info, int x, int y);
void			sort_sprite(t_info *info, int n);
void			draw_rectangles(t_info *info);
int				get_point_size(t_info *info);
void			draw_point(t_info *info, int size);
void			draw_lifebar(t_info *info);
void			ceilfloor_calc(t_info *info);
int				shade_color(double dist, int c);
void			wall_calc(t_info *info);
void			find_hit_wall(t_info *info);
void			sprite_calc(t_info *info);
void			get_sprite_count(t_map *m);
int				wall_check(t_map *m);
int				wall_eight_dir_check(t_map *m);
int				fill_map_array(t_map *m);
void			cubfile_info_lst_free(t_map *m, int n);
t_list			*ft_lstlast(t_list *lst);
void			ft_lstadd_back(t_list **lst, t_list *new);
void			ft_lstdelone(t_list *lst);
void			ft_lstclear(t_list **lst);
t_list			*ft_lstnew(void *content);
int				ft_lstsize(t_list *lst);
int				gnl_map_info(t_map *m, int n);
int				gnl_info(t_map *m, int n);
int				info_check(t_map *m);
int				info_null_check(t_map *m);
int				resolution_check(t_map *m);
void			cubfile_info_free(t_map *m, int n);
int				info_full_check(t_map *m);
int				floor_ceil_check(t_map *m, char *line, int type);

#endif
