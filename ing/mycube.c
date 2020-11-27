#include "minilibx_mms/mlx.h"
#include "mini_opengl/mlx.h"
#include "key_macos.h"
#include "bitmap.h"
#include "cub3d.h"

#define X_EVENT_KEY_PRESS	2
#define X_EVENT_KEY_EXIT	17
#define texWidth 64
#define texHeight 64

#define	uDiv 1
#define	vDiv 1
#define	vMove 0.0

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

typedef struct	s_info
{
	double pos_x;
	double pos_y;
	double dir_x;
	double dir_y;
	double plane_x;
	double plane_y;
	void	*mlx;
	void	*win;
	t_img	img;
	int		**buf;
	int		texture[9][texHeight * texWidth];
	double	movespeed;
	double	rotspeed;
	int		texnum;
	int		tex_x;
	int		tex_y;
	int		color;
	int		winsize_w;
	int		winsize_h;
	int		**map;
	int		map_w;
	int		map_h;
	int		bitmap_check;
	t_bit	bit;
	t_map	m;

	//calc
	double	camera_x;
	double	raydir_x;
	double	raydir_y;
	int		map_x;
	int		map_y;
	double	sidedist_x;
	double	sidedist_y;
	double	deltadist_x;
	double	deltadist_y;
	double	perpwalldist;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	int		lineheight;
	int		drawstart;
	int		drawend;
	double	wall_x;
	double	step;
	double	texpos;

	//floor
	double	raydir_x0;
	double	raydir_y0;
	double	raydir_x1;
	double	raydir_y1;
	int		p;
	double	posz;
	double	rowdist;
	double	fstep_x;
	double	fstep_y;
	double	floor_x;
	double	floor_y;
	int		cell_x;
	int		cell_y;
	int		floor_color;
	int		ceil_color;

	// mouse
	int		mouse_y1;
	int		mouse_y2;

	// sprite
	t_sprite	*sprite;
	double	*zbuffer;
	int		*sprite_order;
	double	*sprite_dist;
	int		sprite_count;
	double	sprite_x;
	double	sprite_y;
	double	invdet;
	double	transform_x;
	double	transform_y;
	int		spritescreen_x;
	int		sprite_h;
	int		sprite_w;
	int		drawstart_x;
	int		drawstart_y;
	int		drawend_x;
	int		drawend_y;
}				t_info;

void	sort_sprite(t_info *info, int amount)
{
	int		temp_o;
	double	temp_d;
	int	i;
	int	j;

	i = 0;
	while (i < amount - 1)
	{
		j = 0;
		while (j < amount - 1 - i)
		{
			if (info->sprite_dist[j] < info->sprite_dist[j + 1])
			{
				temp_d = info->sprite_dist[j];
				info->sprite_dist[j] = info->sprite_dist[j + 1];
				info->sprite_dist[j + 1] = temp_d;
				temp_o = info->sprite_order[j];
				info->sprite_order[j] = info->sprite_order[j + 1];
				info->sprite_order[j + 1] = temp_o;
			}
			j++;
		}
		i++;
	}
}

void	draw(t_info *info)
{
	int	y;
	int	x;

	y = 0;
	while (y < info->winsize_h)
	{
		x = 0;
		while (x < info->winsize_w)
		{
			info->img.data[y * info->winsize_w + x] = info->buf[y][x];
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(info->mlx, info->win, info->img.img, 0, 0);
}

void	floortest(t_info *info)
{
	int	x;
	int	y;

	y = 0;
	while (y < info->winsize_h / 2)
	{
		info->raydir_x0 = info->dir_x - info->plane_x;
		info->raydir_y0 = info->dir_y - info->plane_y;
		info->raydir_x1 = info->dir_x + info->plane_x;
		info->raydir_y1 = info->dir_y + info->plane_y;

		info->p = y - info->winsize_h / 2;
		info->posz = 0.5 * info->winsize_h;
		info->rowdist = fabs(info->posz / info->p); // -1 ~ 1

		info->fstep_x = info->rowdist * (info->raydir_x1 - info->raydir_x0) / info->winsize_w;
		info->fstep_y = info->rowdist * (info->raydir_y1 - info->raydir_y0) / info->winsize_w;

		info->floor_x = info->pos_x	+ info->rowdist * info->raydir_x0;
		info->floor_y = info->pos_y + info->rowdist * info->raydir_y0;

		x = 0;
		while (x < info->winsize_w)
		{
			info->cell_x = (int)info->floor_x;
			info->cell_y = (int)info->floor_y;

			info->tex_x = (int)(texWidth * (info->floor_x - info->cell_x)) & (texWidth - 1);
			info->tex_y = (int)(texHeight * (info->floor_y - info->cell_y)) & (texHeight - 1);
			
			info->floor_x += info->fstep_x;
			info->floor_y += info->fstep_y;

			info->texnum = 4;
			if (info->floor_color == -1)
				info->color = info->texture[info->texnum][texWidth * info->tex_x + info->tex_y];
			else
				info->color = info->floor_color;
			info->color = (info->color >> 1) & 8355711;
			info->buf[info->winsize_h - y - 1][x] = info->color;

			info->texnum = 5;
			if (info->ceil_color == -1)
				info->color = info->texture[info->texnum][texWidth * info->tex_x + info->tex_y];
			else
				info->color = info->ceil_color;
			info->color = (info->color >> 1) & 8355711;

			info->buf[y][x] = info->color;
			x++;
		}
		y++;
	}
}

void	calc(t_info *info)
{
	int	x;
	int	y;
	int	i;

	x = 0;
	while (x < info->winsize_w)
	{
		info->camera_x = 2 * x / (double)info->winsize_w - 1;
		info->raydir_x = info->dir_x + info->plane_x * info->camera_x;
		info->raydir_y = info->dir_y + info->plane_y * info->camera_x;
		
		info->map_x = (int)info->pos_x;
		info->map_y = (int)info->pos_y;

		info->deltadist_x = fabs(1 / info->raydir_x);
		info->deltadist_y = fabs(1 / info->raydir_y);
		
		if (info->raydir_x < 0)
		{
			info->step_x = -1;
			info->sidedist_x = (info->pos_x - info->map_x) * info->deltadist_x;
		}
		else
		{
			info->step_x = 1;
			info->sidedist_x = (info->map_x + 1.0 - info->pos_x) * info->deltadist_x;
		}
		if (info->raydir_y < 0)
		{
			info->step_y = -1;
			info->sidedist_y = (info->pos_y - info->map_y) * info->deltadist_y;
		}
		else
		{
			info->step_y = 1;
			info->sidedist_y = (info->map_y + 1.0 - info->pos_y) * info->deltadist_y;
		}

		info->hit = 0;

		while (info->hit == 0)
		{
			if (info->sidedist_x < info->sidedist_y)
			{
				info->sidedist_x += info->deltadist_x;
				info->map_x += info->step_x;
				info->side = 0;
			}
			else
			{
				info->sidedist_y += info->deltadist_y;
				info->map_y += info->step_y;
				info->side = 1;
			}
			if (info->map[info->map_x][info->map_y] == 1)
				info->hit = 1;
		}
		if (info->side == 0)
			info->perpwalldist = (info->map_x - info->pos_x + (1 - info->step_x) / 2) / info->raydir_x;
		else
			info->perpwalldist = (info->map_y - info->pos_y + (1 - info->step_y) / 2) / info->raydir_y;

		info->lineheight = (int)(info->winsize_h / info->perpwalldist);

		info->drawstart = -(info->lineheight) / 2 + info->winsize_h / 2;
		if(info->drawstart < 0)
			info->drawstart = 0;
		info->drawend = info->lineheight / 2 + info->winsize_h / 2;
		if(info->drawend >= info->winsize_h)
			info->drawend = info->winsize_h - 1;

		if (info->side == 0)
			info->wall_x = info->pos_y + info->perpwalldist * info->raydir_y;
		else
			info->wall_x = info->pos_x + info->perpwalldist * info->raydir_x;
		info->wall_x -= floor(info->wall_x); // -integer

		info->tex_x = (int)(info->wall_x * (double)texWidth);
		if (info->side == 0 && info->raydir_x > 0)
			info->tex_x = texWidth - info->tex_x - 1;
		if (info->side == 1 && info->raydir_y < 0)
			info->tex_x = texWidth - info->tex_x - 1;

		if (info->side == 0 && info->raydir_x < 0)
			info->texnum = 1;
		if (info->side == 0 && info->raydir_x > 0)
			info->texnum = 0;
		if (info->side == 1 && info->raydir_y > 0)
			info->texnum = 3;
		if (info->side == 1 && info->raydir_y < 0)
			info->texnum = 2;

		info->step = 1.0 * texHeight / info->lineheight;
		info->texpos = (info->drawstart - info->winsize_h / 2 + info->lineheight / 2) * info->step;
		
		y = info->drawstart;
		while (y < info->drawend)
		{
			info->tex_y = (int)(info->texpos) & (texHeight - 1);
			info->texpos += info->step;
		
			info->color = info->texture[info->texnum][texHeight * info->tex_y + info->tex_x];
			// make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
			if (info->side == 1)
				info->color = (info->color >> 1) & 8355711;
			info->buf[y][x] = info->color;
			y++;
		}
		info->zbuffer[x] = info->perpwalldist;
		x++;
	}

	i = 0;
	while (i < info->sprite_count)
	{
		info->sprite_order[i] = i;
		info->sprite_dist[i] = (info->pos_x - info->sprite[i].x) * (info->pos_x - info->sprite[i].x) + (info->pos_y - info->sprite[i].y) * (info->pos_y - info->sprite[i].y);
		i++;
	}
	
	sort_sprite(info, info->sprite_count);

	i = 0;
	while (i < info->sprite_count)
	{
		info->sprite_x = info->sprite[info->sprite_order[i]].x - info->pos_x;
		info->sprite_y = info->sprite[info->sprite_order[i]].y - info->pos_y;

		info->invdet = 1.0 / (info->plane_x * info->dir_y - info->dir_x * info->plane_y);
		info->transform_x = info->invdet * (info->dir_y * info->sprite_x - info->dir_x * info->sprite_y);
		info->transform_y = info->invdet * (-info->plane_y * info->sprite_x + info->plane_x * info->sprite_y);

		info->spritescreen_x = (int)((info->winsize_w / 2) * (1 + info->transform_x / info->transform_y));

		int vmovescreen = (int)(vMove / info->transform_y);

		info->sprite_h = abs((int)(info->winsize_h / info->transform_y)) / vDiv;	// !!!!!!!!!!!!!
		info->drawstart_y = -info->sprite_h / 2 + info->winsize_h / 2 + vmovescreen; // !!!!!!!!!!!!!!!!!
		if (info->drawstart_y < 0)
			info->drawstart_y = 0;
		info->drawend_y = info->sprite_h / 2 + info->winsize_h / 2 + vmovescreen;  //!!!!!!!!!!!!!!!!
		if (info->drawend_y >= info->winsize_h)
			info->drawend_y = info->winsize_h - 1;

		info->sprite_w = abs((int)(info->winsize_h / info->transform_y)) / uDiv; // !!!!!!!!!!!!!
		info->drawstart_x = -info->sprite_w / 2 + info->spritescreen_x;
		if (info->drawstart_x < 0)
			info->drawstart_x = 0;
		info->drawend_x = info->sprite_w / 2 + info->spritescreen_x;
		if (info->drawend_x >= info->winsize_w)
			info->drawend_x = info->winsize_w - 1;

		x = info->drawstart_x;;
		while (x < info->drawend_x)
		{
			info->tex_x = (int)(256 * (x - (info->sprite_w / 2 + info->spritescreen_x)) * texWidth / info->sprite_w) / 256;

			if (info->transform_y > 0 && x > 0 && x < info->winsize_w && info->transform_y < info->zbuffer[x])
			{
				y = info->drawstart_y;
				while (y < info->drawend_y)
				{
					info->tex_y = ((int)(256 * ((y - vmovescreen) - info->winsize_h / 2 + info->sprite_h / 2)) * texHeight / info->sprite_h) / 256;	// !!!!!!!!!!!
					info->color = info->texture[info->sprite[info->sprite_order[i]].texture][texWidth * info->tex_y + info->tex_x];
					if ((info->color & 0x00FFFFFF) != 0)
						info->buf[y][x] = info->color;
					y++;
				}
			}
			x++;
		}
		i++;
	}
}

void	all_free(t_info *info, t_map *m);

void	make_imagetexture(t_info *info, char *path, t_img *img, int *texture)
{
	int	y;
	int	x;

	img->img = mlx_xpm_file_to_image(info->mlx, path, &img->img_width, &img->img_height);
	if (img->img == 0)
	{
		perror(path);
		all_free(info, &info->m);
		exit(0);
	}
	img->data = (int *)mlx_get_data_addr(img->img, &img->bpp, &img->size_l, &img->endian);
	y = 0;
	while (y < img->img_height)
	{
		x = 0;
		while (x < img->img_width)
		{
			texture[img->img_width * y + x] = img->data[img->img_width * y + x];
			x++;
		}
		y++;
	}
	mlx_destroy_image(info->mlx, img->img);
}

void	bitmap_setting(t_info *info);

int	main_loop(t_info *info)
{
	int	fd;

	floortest(info);
	calc(info);
	draw(info);
	if (info->bitmap_check == 1)
	{
		info->bitmap_check = 0;
		bitmap_setting(info);
		fd = open("cub3d.bmp", O_WRONLY | O_CREAT, S_IRWXU);
		write(fd, &info->bit, 54);
		write(fd, info->img.data, info->bit.sizeimage);
		close(fd);
		all_free(info, &info->m);
		exit(0);
	}
	return (0);
}

int	key_press(int key, t_info *info)
{
	if (key == K_W)
	{
		if (!info->map[(int)(info->pos_x + info->dir_x * info->movespeed)][(int)(info->pos_y)])
			info->pos_x += info->dir_x * info->movespeed;
		if (!info->map[(int)(info->pos_x)][(int)(info->pos_y + info->dir_y * info->movespeed)])
			info->pos_y += info->dir_y * info->movespeed;
	}
	if (key == K_S)
	{
		if (!info->map[(int)(info->pos_x - info->dir_x * info->movespeed)][(int)(info->pos_y)])
			info->pos_x -= info->dir_x * info->movespeed;
		if (!info->map[(int)(info->pos_x)][(int)(info->pos_y - info->dir_y * info->movespeed)])
			info->pos_y -= info->dir_y * info->movespeed;
	}
	if (key == K_D)
	{
		double olddir_x = info->dir_x;
		info->dir_x = info->dir_x * cos(-info->rotspeed) - info->dir_y * sin(-info->rotspeed);
		info->dir_y = olddir_x * sin(-info->rotspeed) + info->dir_y * cos(-info->rotspeed);
		double oldplane_x = info->plane_x;
		info->plane_x = info->plane_x * cos(-info->rotspeed) - info->plane_y * sin(-info->rotspeed);
		info->plane_y = oldplane_x * sin(-info->rotspeed) + info->plane_y * cos(-info->rotspeed);
	}
	if (key == K_A)
	{
		double olddir_x = info->dir_x;
		info->dir_x = info->dir_x * cos(info->rotspeed) - info->dir_y * sin(info->rotspeed);
		info->dir_y = olddir_x * sin(info->rotspeed) + info->dir_y * cos(info->rotspeed);
		double oldplane_x = info->plane_x;
		info->plane_x = info->plane_x * cos(info->rotspeed) - info->plane_y * sin(info->rotspeed);
		info->plane_y = oldplane_x * sin(info->rotspeed) + info->plane_y * cos(info->rotspeed);
	}
	if (key == K_ESC)
	{
		all_free(info, &info->m);
		exit(0);
	}
	return (0);
}

int button_redcross(t_info *info)
{
	all_free(info, &info->m);
	exit(0);
}

int	mouse_move(int y, int x, t_info *info)
{
	info->mouse_y2 = y;

	if (x >= 0 && y >= 0 && x <= info->winsize_h && y <= info->winsize_w && y < info->mouse_y1)
	{
		double olddir_x = info->dir_x;
		info->dir_x = info->dir_x * cos(info->rotspeed) - info->dir_y * sin(info->rotspeed);
		info->dir_y = olddir_x * sin(info->rotspeed) + info->dir_y * cos(info->rotspeed);
		double oldplane_x = info->plane_x;
		info->plane_x = info->plane_x * cos(info->rotspeed) - info->plane_y * sin(info->rotspeed);
		info->plane_y = oldplane_x * sin(info->rotspeed) + info->plane_y * cos(info->rotspeed);
		info->mouse_y1 = y;
	}
	else if (x >= 0 && y >= 0 && x <= info->winsize_h && y <= info->winsize_w && y > info->mouse_y1)
	{
		double olddir_x = info->dir_x;
		info->dir_x = info->dir_x * cos(info->rotspeed) + info->dir_y * sin(info->rotspeed);
		info->dir_y = -olddir_x * sin(info->rotspeed) + info->dir_y * cos(info->rotspeed);
		double oldplane_x = info->plane_x;
		info->plane_x = info->plane_x * cos(info->rotspeed) + info->plane_y * sin(info->rotspeed);
		info->plane_y = -oldplane_x * sin(info->rotspeed) + info->plane_y * cos(info->rotspeed);
		info->mouse_y1 = y;
	}
	return (0);
}

void	dir_setting(t_info *info, t_map *m)
{
	if (m->start_dir == 'N' || m->start_dir == 'S')
	{
		info->dir_x = 1.0;
		info->dir_y = 0.0;
		info->plane_x = 0.0;
		info->plane_y = 0.66;
		if (m->start_dir == 'N')
			info->dir_x = -info->dir_x;
		else if (m->start_dir == 'S')
			info->plane_y = -info->plane_y;
	}
	else if (m->start_dir == 'E' || m->start_dir == 'W')
	{
		info->dir_x = 0.0;
		info->dir_y = 1.0;
		info->plane_x = 0.66;
		info->plane_y = 0.0;
		if (m->start_dir == 'W')
		{
			info->dir_y = -info->dir_y;
			info->plane_x = -info->plane_x;
		}
	}
}

void	bitmap_setting(t_info *info)
{
	info->bit.ftype = 0x4d42;
	info->bit.fsize = 54 + 4 * info->winsize_w * info->winsize_h;
	info->bit.freserved1 = 0;
	info->bit.freserved2 = 0;
	info->bit.foffbits = 54;

	info->bit.size = 40;
	info->bit.bit_width = info->winsize_w;
	info->bit.bit_height = -info->winsize_h;
	info->bit.planes = 1;
	info->bit.bitcount = 32;
	info->bit.compression = 0;
	info->bit.sizeimage = 4 * info->winsize_w * info->winsize_h;
	info->bit.x_pelspermeter = info->winsize_w;
	info->bit.y_pelspermeter = info->winsize_h;
	info->bit.colorused = 0xFFFFFF;
	info->bit.colorimportant = 0;
}

void	buf_init(t_info *info)
{
	int	i;
	int	j;

	i = 0;
	while (i < info->winsize_h)
	{
		j = 0;
		while (j < info->winsize_w)
		{
			info->buf[i][j] = 0;
			j++;
		}
		i++;
	}
}

int	buf_alloc_init(t_info *info, t_map *m)
{
	int	i;
	int	j;

	if ((info->buf = (int **)malloc(sizeof(int *) * info->winsize_h)) == 0)
	{
		map_array_free(m, m->map_x, 0);
		return (0);
	}
	i = 0;
	while (i < info->winsize_h)
	{
		if ((info->buf[i] = (int *)malloc(sizeof(int) * info->winsize_w)) == 0)
		{
			map_array_free(m, m->map_x, 0);
			j = -1;
			while (++j < i)
				free(info->buf[j]);
			free(info->buf);
			return (0);
		}
		i++;
	}
	buf_init(info);
	return (1);
}

void	all_free(t_info *info, t_map *m)
{
	int i;

	map_array_free(m, m->map_x, 1);
	i = -1;
	while (++i < info->winsize_h)
		free(info->buf[i]);
	free(info->buf);
	if (info->sprite != 0)
		free(info->sprite);
	if (info->zbuffer != 0)
		free(info->zbuffer);
	if (info->sprite_order != 0)
		free(info->sprite_order);
	if (info->sprite_dist != 0)
		free(info->sprite_dist);
	if (info->img.img != 0)
		mlx_destroy_image(info->mlx, info->img.img);
}

int	sprite_alloc_fail(t_info *info, t_map *m)
{
	all_free(info, m);
	write(1, "malloc allocation fail\n", 23);
	return (0);
}

int	sprite_alloc(t_info *info, t_map *m)
{
	info->sprite = 0;
	info->zbuffer = 0;
	info->sprite_order = 0;
	info->sprite_dist = 0;
	if ((info->sprite = (t_sprite *)malloc(sizeof(t_sprite) * info->sprite_count)) == 0)
		return (sprite_alloc_fail(info, m));
	if ((info->zbuffer = (double *)malloc(sizeof(double) * info->winsize_w)) == 0)
		return (sprite_alloc_fail(info, m));
	if ((info->sprite_order = (int *)malloc(sizeof(int) * info->sprite_count)) == 0)
		return (sprite_alloc_fail(info, m));
	if ((info->sprite_dist = (double *)malloc(sizeof(double) * info->sprite_count)) == 0)
		return (sprite_alloc_fail(info, m));
	return (1);
}

void	cub_play(t_map *m)
{
	t_info info;
	int	i;
	int	j;
	int	snum;

	info.movespeed = 0.05;
	info.rotspeed = 0.05;
	info.mlx = mlx_init();
	info.pos_x = m->start_x;
	info.pos_y = m->start_y;
	dir_setting(&info, m);
	info.map = m->map;
	info.map_w = m->map_y;
	info.map_h = m->map_x;
	info.bitmap_check = m->bitmap_check;
	info.sprite_count = m->sprite_count;
	info.floor_color = -1;
	info.ceil_color = -1;
	info.m = *m;
	if (m->floor_color != -1)
		info.floor_color = m->floor_color;
	if (m->ceil_color != -1)
		info.ceil_color = m->ceil_color;

	mlx_get_screen_size(info.mlx, &info.winsize_w, &info.winsize_h);

	if (info.winsize_w > m->w)
		info.winsize_w = m->w;
	else
		write(1, "width > maxsize so width = maxsize\n", 33);

	if (info.winsize_h > m->h)
		info.winsize_h = m->h;
	else
		write(1, "height > maxsize so height = maxsize\n", 35);

	if (info.winsize_w < 200)
	{
		info.winsize_w = 200;
		write(1, "width is very little so width = 200\n", 33);
	}
	if (info.winsize_h < 200)
	{
		info.winsize_h = 200;
		write(1, "height is very little so height = 200\n", 35);
	}

	info.mouse_y1 = info.winsize_h / 2;

	if (buf_alloc_init(&info, m) == 0)
		return ;

	i = 0;
	while (i < 9)
	{
		j = 0;
		while (j < texHeight * texWidth)
		{
			info.texture[i][j] = 0;
			j++;
		}
		i++;
	}

	if (sprite_alloc(&info, m) == 0)
		return ;

	make_imagetexture(&info, m->north, &info.img, info.texture[0]);
	make_imagetexture(&info, m->south, &info.img, info.texture[1]);
	make_imagetexture(&info, m->east, &info.img, info.texture[2]);
	make_imagetexture(&info, m->west, &info.img, info.texture[3]);
	if (info.floor_color == -1)
		make_imagetexture(&info, m->floor, &info.img, info.texture[4]);
	if (info.ceil_color == -1)
		make_imagetexture(&info, m->ceil, &info.img, info.texture[5]);
	make_imagetexture(&info, "textures/greenlight.xpm", &info.img, info.texture[6]);
	make_imagetexture(&info, "textures/barrel.xpm", &info.img, info.texture[7]);
	make_imagetexture(&info, "textures/pillar.xpm", &info.img, info.texture[8]);

	snum = 0;
	i = 0;
	while (i < info.map_h)
	{
		j = 0;
		while (j < info.map_w)
		{
			if (info.map[i][j] == 2)
			{
				info.sprite[snum].x = i + 0.5;
				info.sprite[snum].y = j + 0.5;
				info.sprite[snum].texture = 7;
				snum++;
			}
			j++;
		}
		i++;
	}
	
	info.win = mlx_new_window(info.mlx, info.winsize_w, info.winsize_h, "mlx");

	info.img.img = mlx_new_image(info.mlx, info.winsize_w, info.winsize_h);
	info.img.data = (int *)mlx_get_data_addr(info.img.img, &info.img.bpp, &info.img.size_l, &info.img.endian);

	mlx_loop_hook(info.mlx, &main_loop, &info);

	mlx_hook(info.win, X_EVENT_KEY_PRESS, 0, &key_press, &info);
	mlx_hook(info.win, X_EVENT_KEY_EXIT, 0, &button_redcross, &info);
	mlx_hook(info.win, 6, 0, &mouse_move, &info);

	mlx_loop(info.mlx);
}
