#include "minilibx_mms/mlx.h"
#include "mini_opengl/mlx.h"
#include "key_macos.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define X_EVENT_KEY_PRESS	2
#define X_EVENT_KEY_EXIT	17
#define texWidth 64
#define texHeight 64
#define mapWidth 24
#define mapHeight 24
#define width 1024
#define height 768
#define PI 3.14159265359

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
	int		texture[8][texHeight * texWidth];
	double	movespeed;
	double	rotspeed;
	int		texnum;
	int		tex_x;
	int		tex_y;
	int		color;
	int		winsize_w;
	int		winsize_h;

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

	int		m_y1;
	int		m_y2;

}				t_info;

int	worldMap[mapWidth][mapHeight] =
						{
							{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
							{1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
							{1,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1},
							{1,0,1,0,0,0,0,1,0,1,0,1,0,1,0,1,1,0,0,0,1,1,1,1},
							{1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
							{1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
							{1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1},
							{1,0,0,0,0,0,0,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1},
							{1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1},
							{1,1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1},
							{1,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,1,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,0,1,1},
							{1,0,1,0,1,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,1},
							{1,0,0,1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,0,1,1},
							{1,0,1,0,1,0,0,0,0,1,1,0,1,1,0,0,1,0,0,1,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,0,0,0,1},
							{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
						};

void	draw(t_info *info)
{
	for (int y = 0; y < info->winsize_h; y++)
		for (int x = 0; x < info->winsize_w; x++)
			info->img.data[y * info->winsize_w + x] = info->buf[y][x];
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

			if ((int)(info->floor_x + info->floor_y) % 2 == 0)
				info->texnum = 4;
			else
				info->texnum = 6;
			info->color = info->texture[info->texnum][texWidth * info->tex_x + info->tex_y];
			info->color = (info->color >> 1) & 8355711;

			info->buf[info->winsize_h - y - 1][x] = info->color;

	//		if (((int)info->floor_x +(int) info->floor_y) % 2 == 0)
	//			info->texnum = 5;
	//		else
				info->texnum = 5;
			info->color = info->texture[info->texnum][texWidth * info->tex_x + info->tex_y];
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

	x = 0;
	while (x < info->winsize_w)
	{
		info->camera_x = 2 * x / (double)info->winsize_w - 1;
		info->raydir_x = info->dir_x + info->plane_x * info->camera_x;
		info->raydir_y = info->dir_y + info->plane_y * info->camera_x;
		
		info->map_x = (int)info->pos_x;
		info->map_y = (int)info->pos_y;

		//length of ray from current position to next x or y-side
		
		 //length of ray from one x or y-side to next x or y-side
		info->deltadist_x = fabs(1 / info->raydir_x);
		info->deltadist_y = fabs(1 / info->raydir_y);
		
		//what direction to step in x or y-direction (either +1 or -1)
		
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
			//jump to next map square, OR in x-direction, OR in y-direction
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
			//Check if ray has hit a wall
			if (worldMap[info->map_x][info->map_y] > 0)
				info->hit = 1;
		}
		if (info->side == 0)
			info->perpwalldist = (info->map_x - info->pos_x + (1 - info->step_x) / 2) / info->raydir_x;
		else
			info->perpwalldist = (info->map_y - info->pos_y + (1 - info->step_y) / 2) / info->raydir_y;

		//Calculate height of line to draw on screen
		info->lineheight = (int)(info->winsize_h / info->perpwalldist);

		//calculate lowest and highest pixel to fill in current stripe
		info->drawstart = -(info->lineheight) / 2 + info->winsize_h / 2;
		if(info->drawstart < 0)
			info->drawstart = 0;
		info->drawend = info->lineheight / 2 + info->winsize_h / 2;
		if(info->drawend >= info->winsize_h)
			info->drawend = info->winsize_h - 1;

		// calculate value of wall
		if (info->side == 0)
			info->wall_x = info->pos_y + info->perpwalldist * info->raydir_y;
		else
			info->wall_x = info->pos_x + info->perpwalldist * info->raydir_x;
		info->wall_x -= floor(info->wall_x); // -integer

		// x coordinate on the texture
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

		// How much to increase the texture coordinate perscreen pixel
		info->step = 1.0 * texHeight / info->lineheight;
		// Starting texture coordinate
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
/*
		//FLOOR CASTING (vertical version, directly after drawing the vertical wall stripe for the current x)
		double floorXWall, floorYWall; //x, y position of the floor texel at the bottom of the wall

		//4 different wall directions possible
		if(info->side == 0 && info->raydir_x > 0)
		{
			floorXWall = info->map_x;
			floorYWall = info->map_y + info->wall_x;
		}
		else if(info->side == 0 && info->raydir_x < 0)
		{
			floorXWall = info->map_x + 1.0;
			floorYWall = info->map_y + info->wall_x;
		}
		else if(info->side == 1 && info->raydir_y > 0)
		{
			floorXWall = info->map_x + info->wall_x;
			floorYWall = info->map_y;
		}
		else
		{
			floorXWall = info->map_x + info->wall_x;
			floorYWall = info->map_y + 1.0;
		}

		double distWall, distPlayer, currentDist;

		distWall = info->perpwalldist;
		distPlayer = 0.0;

		if (info->drawend < 0) 
			info->drawend = info->winsize_h; //becomes < 0 when the integer overflows

		//draw the floor from drawEnd to the bottom of the screen
		for(int y = info->drawend + 1; y < info->winsize_h; y++)
		{
			currentDist = info->winsize_h / (2.0 * y - info->winsize_h); //you could make a small lookup table for this instead

			double weight = (currentDist - distPlayer) / (distWall - distPlayer);

			double currentFloorX = weight * floorXWall + (1.0 - weight) * info->pos_x;
			double currentFloorY = weight * floorYWall + (1.0 - weight) * info->pos_y;

			int floorTexX, floorTexY;
			floorTexX = (int)(currentFloorX * texWidth) % texWidth;
			floorTexY = (int)(currentFloorY * texHeight) % texHeight;

			int checkerBoardPattern = ((int)(currentFloorX) + (int)(currentFloorY)) % 2;
			int floorTexture;
			if(checkerBoardPattern == 0) floorTexture = 3;
			else floorTexture = 4;

			//floor
			info->buf[y][x] = (info->texture[floorTexture][texWidth * floorTexY + floorTexX] >> 1) & 8355711;
			//ceiling (symmetrical!)
			info->buf[info->winsize_h - y][x] = info->texture[6][texWidth * floorTexY + floorTexX];
		}
*/
		x++;
	}
}

void	make_imagetexture(t_info *info, char *path, t_img *img, int *texture)
{
	img->img = mlx_xpm_file_to_image(info->mlx, path, &img->img_width, &img->img_height);
	img->data = (int *)mlx_get_data_addr(img->img, &img->bpp, &img->size_l, &img->endian);
	for (int y = 0; y < img->img_height; y++)
		for(int x = 0; x < img->img_width; x++)
			texture[img->img_width * y + x] = img->data[img->img_width * y + x];
	mlx_destroy_image(info->mlx, img->img);
}

int	main_loop(t_info *info)
{
	for (int i = 0; i < info->winsize_h; i++)
	{
		for (int j = 0; j < info->winsize_w; j++)
			info->buf[i][j] = 0x000000;
	}

	floortest(info);
	calc(info);
	draw(info);
	return (0);
}

int	key_press(int key, t_info *info)
{
	if (key == K_W)
	{
		if (!worldMap[(int)(info->pos_x + info->dir_x * info->movespeed)][(int)(info->pos_y)])
			info->pos_x += info->dir_x * info->movespeed;
		if (!worldMap[(int)(info->pos_x)][(int)(info->pos_y + info->dir_y * info->movespeed)])
			info->pos_y += info->dir_y * info->movespeed;
	}
	//move backwards if no wall behind you
	if (key == K_S)
	{
		if (!worldMap[(int)(info->pos_x - info->dir_x * info->movespeed)][(int)(info->pos_y)])
			info->pos_x -= info->dir_x * info->movespeed;
		if (!worldMap[(int)(info->pos_x)][(int)(info->pos_y - info->dir_y * info->movespeed)])
			info->pos_y -= info->dir_y * info->movespeed;
	}
	//rotate to the right
	if (key == K_D)
	{
		//both camera direction and camera plane must be rotated
		double olddir_x = info->dir_x;
		info->dir_x = info->dir_x * cos(-info->rotspeed) - info->dir_y * sin(-info->rotspeed);
		info->dir_y = olddir_x * sin(-info->rotspeed) + info->dir_y * cos(-info->rotspeed);
		double oldplane_x = info->plane_x;
		info->plane_x = info->plane_x * cos(-info->rotspeed) - info->plane_y * sin(-info->rotspeed);
		info->plane_y = oldplane_x * sin(-info->rotspeed) + info->plane_y * cos(-info->rotspeed);
	}
	//rotate to the left
	if (key == K_A)
	{
		//both camera direction and camera plane must be rotated
		double olddir_x = info->dir_x;
		info->dir_x = info->dir_x * cos(info->rotspeed) - info->dir_y * sin(info->rotspeed);
		info->dir_y = olddir_x * sin(info->rotspeed) + info->dir_y * cos(info->rotspeed);
		double oldplane_x = info->plane_x;
		info->plane_x = info->plane_x * cos(info->rotspeed) - info->plane_y * sin(info->rotspeed);
		info->plane_y = oldplane_x * sin(info->rotspeed) + info->plane_y * cos(info->rotspeed);
	}
	if (key == K_ESC)
		exit(0);

	return (0);
}

int button_redcross(t_info *info)
{
	exit(0);
}

int	mouse_move(int y, int x, t_info *info)
{
	info->m_y2 = y;


	if (x >= 0 && y >= 0 && x <= info->winsize_h && y <= info->winsize_w && y < info->m_y1)
	{
		//both camera direction and camera plane must be rotated
		double olddir_x = info->dir_x;
		info->dir_x = info->dir_x * cos(info->rotspeed) - info->dir_y * sin(info->rotspeed);
		info->dir_y = olddir_x * sin(info->rotspeed) + info->dir_y * cos(info->rotspeed);
		double oldplane_x = info->plane_x;
		info->plane_x = info->plane_x * cos(info->rotspeed) - info->plane_y * sin(info->rotspeed);
		info->plane_y = oldplane_x * sin(info->rotspeed) + info->plane_y * cos(info->rotspeed);
		info->m_y1 = y;
	}
	else if (x >= 0 && y >= 0 && x <= info->winsize_h && y <= info->winsize_w && y > info->m_y1)
	{
		//both camera direction and camera plane must be rotated
		double olddir_x = info->dir_x;
		info->dir_x = info->dir_x * cos(info->rotspeed) + info->dir_y * sin(info->rotspeed);
		info->dir_y = -olddir_x * sin(info->rotspeed) + info->dir_y * cos(info->rotspeed);
		double oldplane_x = info->plane_x;
		info->plane_x = info->plane_x * cos(info->rotspeed) + info->plane_y * sin(info->rotspeed);
		info->plane_y = -oldplane_x * sin(info->rotspeed) + info->plane_y * cos(info->rotspeed);
		info->m_y1 = y;
	}
	return (0);
}

int	main(void)
{
	t_info info;
	info.mlx = mlx_init();

	info.pos_x = 11.5;
	info.pos_y = 5.0;
	info.dir_x = -1.0;
	info.dir_y = 0.0;
	info.plane_x = 0.00;
	info.plane_y = 0.66;

	mlx_get_screen_size(info.mlx, &info.winsize_w, &info.winsize_h);

	if (info.winsize_w > width)
		info.winsize_w = width;
	else
		printf("width > maxsize so width = maxsize\n");
	if (info.winsize_h > height)
		info.winsize_h = height;
	else
		printf("height > maxsize so height = maxsize\n");
	if (info.winsize_w < 200)
	{
		info.winsize_w = 200;
		printf("width very little so width = 200\n");
	}
	if (info.winsize_h < 200)
	{
		info.winsize_h = 200;
		printf("height is very little so height = 200\n");
	}

	info.m_y1 = info.winsize_h / 2;

	info.buf = (int **)malloc(sizeof(int *) * info.winsize_h);
	for (int i = 0; i < info.winsize_h; i++)
		info.buf[i] = (int *)malloc(sizeof(int) * info.winsize_w);

	for (int i = 0; i < info.winsize_h; i++)
		for (int j = 0; j < info.winsize_w; j++)
			info.buf[i][j] = 0;

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < texHeight * texWidth; j++)
			info.texture[i][j] = 0;

	make_imagetexture(&info, "textures/wall_n.xpm", &info.img, info.texture[0]);
	make_imagetexture(&info, "textures/wall_s.xpm", &info.img, info.texture[1]);
	make_imagetexture(&info, "textures/wall_e.xpm", &info.img, info.texture[2]);
	make_imagetexture(&info, "textures/wall_w.xpm", &info.img, info.texture[3]);
	make_imagetexture(&info, "textures/redbrick.xpm", &info.img, info.texture[4]);
	make_imagetexture(&info, "textures/wood.xpm", &info.img, info.texture[5]);
	make_imagetexture(&info, "textures/bluestone.xpm", &info.img, info.texture[6]);
	make_imagetexture(&info, "textures/eagle.xpm", &info.img, info.texture[7]);

	info.movespeed = 0.05;
	info.rotspeed = 0.05;
	
	info.win = mlx_new_window(info.mlx, info.winsize_w, info.winsize_h, "mlx");

	info.img.img = mlx_new_image(info.mlx, info.winsize_w, info.winsize_h);
	info.img.data = (int *)mlx_get_data_addr(info.img.img, &info.img.bpp, &info.img.size_l, &info.img.endian);

	mlx_loop_hook(info.mlx, &main_loop, &info);
	mlx_hook(info.win, X_EVENT_KEY_PRESS, 0, &key_press, &info);
	mlx_hook(info.win, X_EVENT_KEY_EXIT, 0, &button_redcross, &info);
	mlx_hook(info.win, 6, 0, &mouse_move, &info);
	mlx_loop(info.mlx);
}
