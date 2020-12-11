#include "cub3d.h"
#include <signal.h>

#define X_EVENT_KEY_PRESS	2
#define X_EVENT_KEY_RELEASE 3
#define X_EVENT_MOTION_NOTIFY 6
#define X_EVENT_KEY_EXIT	17

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
	int		bonus_on;
	int		pid;
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
	t_img	texture[14];
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

	double	pitch;
	double	jump;

	// key
	int		flag_w;
	int		flag_a;
	int		flag_s;
	int		flag_d;
	int		flag_c;
	int		flag_sp;
	int		flag_sky;
	int		flag_weap;
	int		flag_shot;

	// bonus
	double	hp;
	double	shot_x;
	double	shot_y;

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
	double	sh;
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
	int		is_floor;
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
	int		mouse_x1;
	int		mouse_x2;

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

int	shade_color(double dist, int c)
{
	int result;
	double	n;

	n = dist / 1.5;
	if (n <= 1.0)
		n = 1.0;
	if (n >= 20.0)
		n = 20.0;
	result = ((int)(((0xFF0000 & c) >> 16) / n) << 16) + ((int)(((0x00FF00 & c) >> 8) / n) << 8) + ((int)((0x0000FF & c) / n));
	return (result);
}
		
void	floor_calc(t_info *info)
{
	int	x;
	int	y;

	y = 0;
	while (y < info->winsize_h)
	{
		info->raydir_x0 = info->dir_x - info->plane_x;
		info->raydir_y0 = info->dir_y - info->plane_y;
		info->raydir_x1 = info->dir_x + info->plane_x;
		info->raydir_y1 = info->dir_y + info->plane_y;

		info->is_floor = y > info->winsize_h / 2 + info->pitch;
		
		if (info->is_floor == 1)
			info->p = y - info->winsize_h / 2 - info->pitch;
		else
			info->p = info->winsize_h / 2 - y + info->pitch;
		
		if (info->is_floor == 1)
			info->posz = 0.5 * info->winsize_h + info->jump;
		else
			info->posz = 0.5 * info->winsize_h - info->jump;
		
		info->rowdist = fabs(info->posz / info->p); // -1 ~ 1

		info->fstep_x = info->rowdist * (info->raydir_x1 - info->raydir_x0) / info->winsize_w;
		info->fstep_y = info->rowdist * (info->raydir_y1 - info->raydir_y0) / info->winsize_w;

		info->floor_x = info->pos_x	+ info->rowdist * info->raydir_x0;
		info->floor_y = info->pos_y + info->rowdist * info->raydir_y0;

		x = 0;

		if (info->is_floor == 1)
			info->texnum = 4;
		else
			info->texnum = 5;

		while (x < info->winsize_w)
		{
		//	info->sh = sqrt((info->floor_x - info->pos_x) * (info->floor_x - info->pos_x) + (info->floor_y - info->pos_y) * (info->floor_y - info->pos_y));
			info->sh = info->rowdist;
			info->cell_x = (int)info->floor_x;
			info->cell_y = (int)info->floor_y;

			info->tex_x = (int)(info->texture[info->texnum].img_width * (info->floor_x - info->cell_x)) & (info->texture[info->texnum].img_width - 1);
			info->tex_y = (int)(info->texture[info->texnum].img_height * (info->floor_y - info->cell_y)) & (info->texture[info->texnum].img_height - 1);
			
			info->floor_x += info->fstep_x;
			info->floor_y += info->fstep_y;
			if (info->is_floor)
			{
				if (info->floor_color == -1)
					info->color = info->texture[info->texnum].data[info->texture[info->texnum].img_width * info->tex_y + info->tex_x];
				else
					info->color = info->floor_color;
				if (info->flag_sky != 1)
					info->color = shade_color(info->sh, info->color);
				info->buf[y][x] = info->color;
			}
			else
			{
				if (info->ceil_color == -1)
					info->color = info->texture[info->texnum].data[info->texture[info->texnum].img_width * info->tex_y + info->tex_x];
				else
					info->color = info->ceil_color;
				if (info->flag_sky == 1)
				{
					int a, b;
					a = y * info->texture[5].img_width / info->winsize_h;
					a = info->texture[5].img_width - a + 1;
					b = x * info->texture[5].img_height / info->winsize_w;
					info->color = info->texture[5].data[b * info->texture[5].img_width + a];
				}
				if (info->flag_sky != 1)
					info->color = shade_color(info->sh, info->color);
				info->buf[y][x] = info->color;
			}
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

		info->drawstart = -(info->lineheight) / 2 + info->winsize_h / 2 + info->pitch + (info->jump / info->perpwalldist);
		if(info->drawstart < 0)
			info->drawstart = 0;
		info->drawend = info->lineheight / 2 + info->winsize_h / 2 + info->pitch  + (info->jump / info->perpwalldist);
		if(info->drawend >= info->winsize_h)
			info->drawend = info->winsize_h - 1;

		if (info->side == 0)
			info->wall_x = info->pos_y + info->perpwalldist * info->raydir_y;
		else
			info->wall_x = info->pos_x + info->perpwalldist * info->raydir_x;
		info->wall_x -= floor(info->wall_x); // -integer

		if (info->side == 0 && info->raydir_x < 0)
			info->texnum = 1;
		if (info->side == 0 && info->raydir_x > 0)
			info->texnum = 0;
		if (info->side == 1 && info->raydir_y > 0)
			info->texnum = 3;
		if (info->side == 1 && info->raydir_y < 0)
			info->texnum = 2;

		info->tex_x = (int)(info->wall_x * (double)info->texture[info->texnum].img_width);
		if (info->side == 0 && info->raydir_x > 0)
			info->tex_x = info->texture[info->texnum].img_width - info->tex_x - 1;
		if (info->side == 1 && info->raydir_y < 0)
			info->tex_x = info->texture[info->texnum].img_width - info->tex_x - 1;

		info->step = 1.0 * info->texture[info->texnum].img_height / info->lineheight;
		info->texpos = (info->drawstart - info->pitch - (info->jump / info->perpwalldist) - info->winsize_h / 2 + info->lineheight / 2) * info->step; // look up
		
		y = info->drawstart;
		while (y < info->drawend)
		{
			info->tex_y = (int)(info->texpos) & (info->texture[info->texnum].img_height - 1);
			info->texpos += info->step;

		//	info->sh = sqrt((info->map_x + 0.5 - info->pos_x) * (info->map_x + 0.5 - info->pos_x) + (info->map_y + 0.5 - info->pos_y) * (info->map_y + 0.5 - info->pos_y));
			info->sh = info->perpwalldist;
			info->color = info->texture[info->texnum].data[info->texture[info->texnum].img_width * info->tex_y + info->tex_x];
			if (info->flag_sky != 1)
				info->color = shade_color(info->sh, info->color);
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
		if (info->sprite[info->sprite_order[i]].texture != -1)
		{
			info->sprite_x = info->sprite[info->sprite_order[i]].x - info->pos_x;
			info->sprite_y = info->sprite[info->sprite_order[i]].y - info->pos_y;
	
			info->invdet = 1.0 / (info->plane_x * info->dir_y - info->dir_x * info->plane_y);
			info->transform_x = info->invdet * (info->dir_y * info->sprite_x - info->dir_x * info->sprite_y);
			info->transform_y = info->invdet * (-info->plane_y * info->sprite_x + info->plane_x * info->sprite_y);
	
			info->spritescreen_x = (int)((info->winsize_w / 2) * (1 + info->transform_x / info->transform_y));
	
			info->sprite_h = fabs(info->winsize_h / info->transform_y);
			info->drawstart_y = -info->sprite_h / 2 + info->winsize_h / 2 + info->pitch + (info->jump / info->transform_y);
			if (info->drawstart_y < 0)
				info->drawstart_y = 0;
			info->drawend_y = info->sprite_h / 2 + info->winsize_h / 2 + info->pitch + (info->jump / info->transform_y);
			if (info->drawend_y >= info->winsize_h)
				info->drawend_y = info->winsize_h - 1;
	
			info->sprite_w = fabs(info->winsize_h / info->transform_y);
			info->drawstart_x = -info->sprite_w / 2 + info->spritescreen_x;
			if (info->drawstart_x < 0)
				info->drawstart_x = 0;
			info->drawend_x = info->sprite_w / 2 + info->spritescreen_x;
			if (info->drawend_x >= info->winsize_w)
				info->drawend_x = info->winsize_w - 1;
			
			int texw = info->texture[info->sprite[info->sprite_order[i]].texture].img_width;////////////////////////////// !!!!!!!!!!!!!

			x = info->drawstart_x;
			while (x < info->drawend_x)
			{
				info->tex_x = (int)(256 * (x - (-info->sprite_w / 2 + info->spritescreen_x)) * texw / info->sprite_w) / 256;
				if (info->transform_y > 0 && x > 0 && x < info->winsize_w && info->transform_y < info->zbuffer[x])
				{
					info->sh = sqrt((info->sprite_x * info->sprite_x) + (info->sprite_y * info->sprite_y));
					y = info->drawstart_y;
					while (y < info->drawend_y)
					{
						info->tex_y = ((int)(256 * (y - info->pitch - (info->jump / info->transform_y) - info->winsize_h / 2 + info->sprite_h / 2)) * texw / info->sprite_h) / 256;
						if (info->sprite[info->sprite_order[i]].texture == 6 || info->sprite[info->sprite_order[i]].texture == 7)
							info->color = info->texture[info->sprite[info->sprite_order[i]].texture].data[texw * info->tex_y + info->tex_x];
						else
							info->color = info->texture[info->sprite[info->sprite_order[i]].texture].data[texw * info->tex_x + info->tex_y];
						if ((info->color & 0x00FFFFFF) != 0 && (info->color != 0x0000FF00) != 0)
						{
							if (info->flag_sky != 1)
								info->color = shade_color(info->sh, info->color);
							info->buf[y][x] = info->color;
						}
						y++;
					}
				}
				x++;
			}
		}
		i++;
	}
}

void	all_free(t_info *info, t_map *m);

void	get_imagedata(t_info *info, char *path, int n)
{
	if ((info->texture[n].img = mlx_xpm_file_to_image(info->mlx, path, &info->texture[n].img_width, &info->texture[n].img_height)) == 0)
	{
		perror(path);
		all_free(info, &info->m);
		exit(0);
	}
	if ((info->texture[n].data = (int *)mlx_get_data_addr(info->texture[n].img, &info->texture[n].bpp, &info->texture[n].size_l, &info->texture[n].endian)) == 0)
	{
		write(1, "error : mlx_get_data_addr result = NULL\n", 39);
		all_free(info, &info->m);
		exit(0);
	}
}

void	bitmap_setting(t_info *info);
void	buf_init(t_info *info);

void	draw_point(t_info *info, int size)
{
	int	i;
	int	j;
	double	x;
	double	y;
	int color;

	int	 p = info->winsize_h;
	if (p > info->winsize_w)
		p = info->winsize_w;

	int q = info->map_h;
	if (q < info->map_w)
		q = info->map_w;
	p = p / q;

	y = info->pos_x * p;
	x = info->pos_y * p;
	
	size = p;
	color = 0xAA0000FF;
	i = y - (size / 2);
	while (i < y + (size / 2))
	{
		j = x - (size / 2);
		while (j < x + (size / 2))
		{
			info->img.data[i * info->winsize_w + j] = color;
			j++;
		}
		i++;
	}
}

static	int	get_atoi_size(int n)
{
	int	count;

	if (n == 0)
		return (1);
	count = 0;
	while (n != 0)
	{
		count++;
		n /= 10;
	}
	return (count);
}

char		*ft_itoa(int n)
{
	char	*temp;
	int		size;
	int		minus;

	size = get_atoi_size(n);
	minus = 1;
	if (n < 0)
		size++;
	if (n < 0)
		minus = -1;
	temp = (char *)malloc(sizeof(char) * (size + 1));
	if (temp == 0)
		return (0);
	temp[size] = '\0';
	if (n == 0)
		temp[0] = '0';
	if (n < 0)
		temp[0] = '-';
	while (--size >= 0 && n != 0)
	{
		temp[size] = n % 10 * minus + '0';
		n /= 10;
	}
	return (temp);
}

void	draw_lifebar(t_info *info)
{
	int	x;
	int y;
	int	w;
	int	h;
	char	*hp_str;

	w = info->winsize_w / 20;
	if (w < 40)
		w = 40;
	h = info->winsize_h / 3;

	hp_str = ft_itoa((int)info->hp);
	x = 0;
	while (x < w)
	{
		y = (info->winsize_h * 2 / 3) + ((100 - info->hp) / 100 * h );;
		while (y < info->winsize_h)
		{
			info->img.data[y * info->winsize_w + x] = 0xAAFF0000;
			y++;
		}
		x++;
	}
	mlx_put_image_to_window(info->mlx, info->win, info->img.img, 0, 0);
	if (((info->winsize_h * 2 / 3) + ((100 - info->hp) / 100) * h) < info->winsize_h - 20)
		mlx_string_put(info->mlx, info->win, 0, (info->winsize_h * 2 / 3) + ((100 - info->hp) / 100) * h, 0x00FF00, hp_str);
	else
		mlx_string_put(info->mlx, info->win, 0, info->winsize_h - 20, 0x00FF00, hp_str);
	free(hp_str);
}

void	draw_rectangle(t_info *info, int x, int y)
{
	int i;
	int j;
	int	color;

	if (info->map[y][x] == 1)
		color = 0xAAFFFFFF;
	else if (info->map[y][x] >= 2)
		color = 0xAAFF0000;
	else
		color = 0xAA000000;

	int	 p = info->winsize_h;
	if (p > info->winsize_w)
		p = info->winsize_w;

	int q = info->map_h;
	if (q < info->map_w)
		q = info->map_w;
	p = (p / q);

	x *= p;
	y *= p;
	i = 0;
	while (i < p)
	{
		j = 0;
		while (j < p)
		{
			info->img.data[(y + i) * info->winsize_w + x + j] = color;
			j++;
		}
		i++;
	}
}

void	draw_rectangles(t_info *info)
{
	int		i;
	int		j;

	i = 0;
	while (i < info->map_h)
	{
		j = 0;
		while (j < info->map_w)
		{
			draw_rectangle(info, j, i);
			j++;
		}
		i++;
	}
}

void	remove_sprite(t_info *info, int x, int y)
{
	int	i;

	i = 0;
	while (i < info->sprite_count)
	{
		if ((int)info->sprite[i].x == x && (int)info->sprite[i].y == y)
		{
		/*	int pid = fork();
			if (pid == 0)
			{
				if (info->sprite[i].texture == 7)
					system("afplay ./sound/hit.wav");
				else if (info->sprite[i].texture == 8)
					system("afplay ./sound/heal.wav");
				else if (info->sprite[i].texture == 9)
					system("afplay ./sound/poison.wav");
				info->pid = -1;
				all_free(info, &info->m);
				exit(0);
			}*/
			// bgm ver2
			
			if (info->sprite[i].texture == 7)
				system("afplay ./sound/hit.wav &");
			else if (info->sprite[i].texture == 8)
				system("afplay ./sound/heal.wav &");
			else if (info->sprite[i].texture == 9)
				system("afplay ./sound/poison.wav &");
			info->sprite[i].texture = -1;
			break ;
		}
		i++;
	}
}

int	main_loop(t_info *info)
{
	int	fd;
	int	n;

	floor_calc(info);
	calc(info);
	draw(info);
	if (info->bonus_on == 1)
	{
		draw_rectangles(info);
		draw_point(info, 0);
		draw_lifebar(info);
	}
	if (info->hp <= 0)
	{
		write(1, "game over\n", 10);
		all_free(info, &info->m);
		exit(0);
	}

	if (info->flag_weap == 1)
	{
		int	a, b;

		a = info->winsize_w - 460;
		b = info->winsize_h - 300;
		if (a < 0 || a < info->winsize_w / 2)
			a = info->winsize_w / 4;
		if (b < 0 || b < info->winsize_h / 2)
			b = info->winsize_h / 2;
		mlx_put_image_to_window(info->mlx, info->win, info->texture[12 + info->flag_shot].img, a, b);
		mlx_put_image_to_window(info->mlx, info->win, info->texture[11].img, info->winsize_w / 2, info->winsize_h / 2);
	}
	if (info->flag_w == 1)
	{
		n = info->map[(int)(info->pos_x + info->dir_x * info->movespeed)][(int)(info->pos_y)];
		if (n == 0)
			info->pos_x += info->dir_x * info->movespeed;
		else if (n == 4 ||n == 5 || n == 6)
		{
			info->map[(int)(info->pos_x + info->dir_x * info->movespeed)][(int)(info->pos_y)] = 0;
			remove_sprite(info, (int)(info->pos_x + info->dir_x * info->movespeed), (int)(info->pos_y));
			info->pos_x += info->dir_x * info->movespeed;
			if (n == 4)
				info->hp += 10;
			else if (n == 5)
				info->hp -= 10;
			else
				info->flag_weap = 1;
		}
		n = info->map[(int)(info->pos_x)][(int)(info->pos_y + info->dir_y * info->movespeed)];
		if (n == 0)
			info->pos_y += info->dir_y * info->movespeed;
		else if (n == 4 || n == 5 || n == 6)
		{
			info->map[(int)(info->pos_x)][(int)(info->pos_y + info->dir_y * info->movespeed)] = 0;
			remove_sprite(info, (int)(info->pos_x), (int)(info->pos_y + info->dir_y * info->movespeed));
			info->pos_y += info->dir_y * info->movespeed;
			if (n == 4)
				info->hp += 10;
			else if (n == 5)
				info->hp -= 10;
			else
				info->flag_weap = 1;
		}
	}
	if (info->flag_s == 1)
	{
		n = info->map[(int)(info->pos_x - info->dir_x * info->movespeed)][(int)(info->pos_y)];
		if (n == 0)
			info->pos_x -= info->dir_x * info->movespeed;
		else if (n == 4 || n == 5 || n == 6)
		{
			info->map[(int)(info->pos_x - info->dir_x * info->movespeed)][(int)(info->pos_y)] = 0;
			remove_sprite(info, (int)(info->pos_x - info->dir_x * info->movespeed), (int)(info->pos_y));
			info->pos_x -= info->dir_x * info->movespeed;
			if (n == 4)
				info->hp += 10;
			else if (n == 5)
				info->hp -= 10;
			else
				info->flag_weap = 1;
		}
		n = info->map[(int)(info->pos_x)][(int)(info->pos_y - info->dir_y * info->movespeed)];
		if (n == 0)
			info->pos_y -= info->dir_y * info->movespeed;
		else if (n == 4 || n == 5 || n == 6)
		{
			info->map[(int)(info->pos_x)][(int)(info->pos_y - info->dir_y * info->movespeed)] = 0;
			remove_sprite(info, (int)(info->pos_x), (int)(info->pos_y - info->dir_y * info->movespeed));
			info->pos_y -= info->dir_y * info->movespeed;
			if (n == 4)
				info->hp += 10;
			else if (n == 5)
				info->hp -= 10;
			else
				info->flag_weap = 1;
		}
	}
	if (info->flag_d == 1)
	{
		double olddir_x = info->dir_x;
		info->dir_x = info->dir_x * cos(-info->rotspeed) - info->dir_y * sin(-info->rotspeed);
		info->dir_y = olddir_x * sin(-info->rotspeed) + info->dir_y * cos(-info->rotspeed);
		double oldplane_x = info->plane_x;
		info->plane_x = info->plane_x * cos(-info->rotspeed) - info->plane_y * sin(-info->rotspeed);
		info->plane_y = oldplane_x * sin(-info->rotspeed) + info->plane_y * cos(-info->rotspeed);
	}
	if (info->flag_a == 1)
	{
		double olddir_x = info->dir_x;
		info->dir_x = info->dir_x * cos(info->rotspeed) - info->dir_y * sin(info->rotspeed);
		info->dir_y = olddir_x * sin(info->rotspeed) + info->dir_y * cos(info->rotspeed);
		double oldplane_x = info->plane_x;
		info->plane_x = info->plane_x * cos(info->rotspeed) - info->plane_y * sin(info->rotspeed);
		info->plane_y = oldplane_x * sin(info->rotspeed) + info->plane_y * cos(info->rotspeed);
	}
	if (info->bitmap_check == 1)
	{
		info->bitmap_check = 0;
		bitmap_setting(info);
		fd = open("cub3D.bmp", O_WRONLY | O_CREAT, S_IRWXU);
		write(fd, &info->bit, 54);
		write(fd, info->img.data, info->bit.sizeimage);
		close(fd);
		all_free(info, &info->m);
		exit(0);
	}
	if (info->jump > 0)
	{
		if (0 > info->jump - info->winsize_h / 4 * info->movespeed)
			info->jump = 0;
		else
			info->jump = info->jump - info->winsize_h / 4 * info->movespeed;
	}
	if (info->jump < 0 && info->flag_c == 0)
	{
		if (0 > info->jump + info->winsize_h / 3 * info->movespeed)
			info->jump = 0;
		else
			info->jump = info->jump + info->winsize_h / 3 * info->movespeed;
	}
	else if (info->jump == 0)
		info->flag_sp = 0;
	return (0);
}

int	key_release(int key, t_info *info)
{
	if (key == K_W)
		info->flag_w = 0;
	if (key == K_S)
		info->flag_s = 0;
	if (key == K_D)
		info->flag_d = 0;
	if (key == K_A)
		info->flag_a = 0;
	if (key == K_C && info->flag_c == 1)
	{
		info->flag_c = 0;
		info->movespeed *= 3;
	}
	if (key == K_V)
		info->flag_shot = 0;
	return (0);
}

void	gun_shot(t_info *info)
{
	int	x;
	int	y;

	info->shot_x = info->pos_x;
	info->shot_y = info->pos_y;
	x = (int)info->shot_x;
	y = (int)info->shot_y;

	while ((x >= 0 && x < info->map_h) && (y >= 0 && y < info->map_w) && info->shot_x < info->pos_x + 5.0 && info->shot_y < info->pos_y + 5.0)
	{
		info->shot_x += info->dir_x * 0.2;
		info->shot_y += info->dir_y * 0.2;
		x = (int)info->shot_x;
		y = (int)info->shot_y;
		if ((x >= 0 && x < info->map_h) && (y >= 0 && y < info->map_w) && info->map[x][y] == 3)
		{
			info->map[x][y] = 0;
			remove_sprite(info, x, y);
			break ;
		}
		else if ((x >= 0 && x < info->map_h) && (y >= 0 && y < info->map_w) && (info->map[x][y] == 1 || info->map[x][y] == 2))
			break ;
	}
}

int	key_press(int key, t_info *info)
{
	if (key == K_W)
		info->flag_w = 1;
	else if (key == K_S)
		info->flag_s = 1;
	else if (key == K_D)
		info->flag_d = 1;
	else if (key == K_A)
		info->flag_a = 1;
	else if (key == K_ESC)
	{
		all_free(info, &info->m);
		exit(0);
	}
	if (key == K_T && info->bonus_on == 1)
	{
		info->pitch += 400 * info->movespeed;
		if (info->pitch > 400)
			info->pitch = 400;
	}
	if (key == K_G && info->bonus_on == 1)
	{
		info->pitch -= 400 * info->movespeed;
		if (info->pitch < -400)
			info->pitch = -400;
	}
	if (key == K_SP && info->bonus_on == 1 && info->flag_sp == 0)
	{
		info->jump = info->winsize_h / 2;
		info->flag_sp = 1;
	}
	if (key == K_C && info->bonus_on == 1 && info->flag_c == 0)
	{
		info->jump = -info->winsize_h / 3;
		info->flag_c = 1;
		info->movespeed /= 3;
	}
	if (key == K_V && info->flag_weap == 1 && info->flag_shot == 0)
	{
		info->flag_shot = 1;
		gun_shot(info);
	/*	int pid = fork();
		if (pid == 0)
		{
			system("afplay ./sound/attack.wav");
			info->pid = -1;
			all_free(info, &info->m);
			exit(0);
		}*/
		// bgm ver2
		system("afplay ./sound/attack.wav &");
	}
	return (0);
}

int button_redcross(t_info *info)
{
	all_free(info, &info->m);
	exit(0);
}

int	mouse_move(int x, int y, t_info *info)
{
	info->mouse_x2 = x;

	if (x >= 0 && y >= 0 && x <= info->winsize_w && y <= info->winsize_h && x < info->mouse_x1)
	{
		double olddir_x = info->dir_x;
		info->dir_x = info->dir_x * cos(info->rotspeed) - info->dir_y * sin(info->rotspeed);
		info->dir_y = olddir_x * sin(info->rotspeed) + info->dir_y * cos(info->rotspeed);
		double oldplane_x = info->plane_x;
		info->plane_x = info->plane_x * cos(info->rotspeed) - info->plane_y * sin(info->rotspeed);
		info->plane_y = oldplane_x * sin(info->rotspeed) + info->plane_y * cos(info->rotspeed);
		info->mouse_x1 = x;
	}
	else if (x >= 0 && y >= 0 && x <= info->winsize_w && y <= info->winsize_h && x > info->mouse_x1)
	{
		double olddir_x = info->dir_x;
		info->dir_x = info->dir_x * cos(info->rotspeed) + info->dir_y * sin(info->rotspeed);
		info->dir_y = -olddir_x * sin(info->rotspeed) + info->dir_y * cos(info->rotspeed);
		double oldplane_x = info->plane_x;
		info->plane_x = info->plane_x * cos(info->rotspeed) + info->plane_y * sin(info->rotspeed);
		info->plane_y = -oldplane_x * sin(info->rotspeed) + info->plane_y * cos(info->rotspeed);
		info->mouse_x1 = x;
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
		free(info->mlx);
		return (0);
	}
	i = -1;
	while (++i < info->winsize_h)
	{
		if ((info->buf[i] = (int *)malloc(sizeof(int) * info->winsize_w)) == 0)
		{
			map_array_free(m, m->map_x, 0);
			j = -1;
			while (++j < i)
				free(info->buf[j]);
			free(info->buf);
			free(info->mlx);
			return (0);
		}
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
//	if (info->pid != -1)
//	{
	if (info->img.img != 0)
		mlx_destroy_image(info->mlx, info->img.img);
	i = -1;
	while (++i < 14)
		if (info->texture[i].img != 0)
			mlx_destroy_image(info->mlx, info->texture[i].img);
	if (info->win != 0)
		mlx_destroy_window(info->mlx, info->win);
	free(info->mlx);
	// bgm ver2
	system("killall afplay");
//	kill(info->pid + 1, SIGTERM);
//	}
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

	info.pitch = 0;
	info.jump = 0;
	info.hp = 100;
	info.img.img = 0;
	info.pid = m->pid;
	info.win = 0;
	info.flag_w = 0;
	info.flag_a = 0;
	info.flag_s = 0;
	info.flag_d = 0;
	info.flag_c = 0;
	info.flag_sp = 0;
	info.flag_sky = 0;
	info.flag_weap = 0;
	info.flag_shot = 0;
	info.bonus_on = m->bonus_on;
	info.movespeed = 0.05;
	info.rotspeed = 0.025;
	if ((info.mlx = mlx_init()) == 0)
	{
		write(1, "error : mlx_init result = NULL\n", 31);
		map_array_free(m, m->map_x, 1);
		return ;
	}
	info.pos_x = m->start_x + 0.5;
	info.pos_y = m->start_y + 0.5;
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
		write(1, "width > maxsize so width = maxsize\n", 35);

	if (info.winsize_h > m->h)
		info.winsize_h = m->h;
	else
		write(1, "height > maxsize so height = maxsize\n", 37);

	if (info.winsize_w < 200)
	{
		info.winsize_w = 200;
		write(1, "width is very little so width = 200\n", 36);
	}
	if (info.winsize_h < 200)
	{
		info.winsize_h = 200;
		write(1, "height is very little so height = 200\n", 38);
	}

	info.mouse_x1 = info.winsize_w / 2;

	if (buf_alloc_init(&info, m) == 0)
		return ;
	if (sprite_alloc(&info, m) == 0)
		return ;

	i = -1;
	while (++i < 12)
		info.texture[i].img = 0;

	get_imagedata(&info, m->north, 0);
	get_imagedata(&info, m->south, 1);
	get_imagedata(&info, m->east, 2);
	get_imagedata(&info, m->west, 3);
	if (info.floor_color == -1)
		get_imagedata(&info, m->floor, 4);
	if (info.ceil_color == -1)
	{
		if (ft_strnstr(m->ceil, "./textures/skybox", 17) != 0)
			info.flag_sky = 1;
		get_imagedata(&info, m->ceil, 5);
	}
	get_imagedata(&info, m->item, 6);
	get_imagedata(&info, "./textures/sp1.xpm", 7);
	get_imagedata(&info, "./textures/health_s.xpm", 8);
	get_imagedata(&info, "./textures/health_b.xpm", 9);
	get_imagedata(&info, "./textures/weap_spr.xpm", 10);
	get_imagedata(&info, "./textures/crosshair.xpm", 11);
	get_imagedata(&info, "./textures/M2GFB0.xpm", 12);
	get_imagedata(&info, "./textures/M2GFA0.xpm", 13);

	snum = 0;
	i = 0;
	while (i < info.map_h)
	{
		j = 0;
		while (j < info.map_w)
		{
			if (info.map[i][j] >= 2 && info.map[i][j] <= 6)
			{
				info.sprite[snum].x = i + 0.5;
				info.sprite[snum].y = j + 0.5;
				info.sprite[snum].texture = 4 + info.map[i][j];
				snum++;
			}
			j++;
		}
		i++;
	}

	if ((info.win = mlx_new_window(info.mlx, info.winsize_w, info.winsize_h, "cub3D")) == 0)
	{
		all_free(&info, m);
		write(1, "error : mlx_new_window return = NULL\n", 37);
		return ;
	}
	if ((info.img.img = mlx_new_image(info.mlx, info.winsize_w, info.winsize_h)) == 0)
	{
		all_free(&info, m);
		write(1, "error : mlx_new_image return = NULL\n", 36);
		return ;
	}
	if ((info.img.data = (int *)mlx_get_data_addr(info.img.img, &info.img.bpp, &info.img.size_l, &info.img.endian)) == 0)
	{
		all_free(&info, m);
		write(1, "error : mlx_get_data_addr return = NULL\n", 40);
		return ;
	}
	
	mlx_loop_hook(info.mlx, &main_loop, &info);

	mlx_hook(info.win, X_EVENT_KEY_PRESS, 0, &key_press, &info);
	mlx_hook(info.win, X_EVENT_KEY_RELEASE, 0, &key_release, &info);
	mlx_hook(info.win, X_EVENT_KEY_EXIT, 0, &button_redcross, &info);
	if (info.bonus_on == 1)
		mlx_hook(info.win, X_EVENT_MOTION_NOTIFY, 0, &mouse_move, &info);
	mlx_loop(info.mlx);
}
