#include "./mini_opengl/mlx.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#define mapWidth 24
#define mapHeight 24
#define screenWidth 640
#define screenHeight 480
#define WIDTH 24 * 32
#define HEIGHT 24 * 32

# define TO_COORD(X, Y) ((int)floor(Y) * WIDTH + (int)floor(X))

typedef struct	s_img
{
	void	*img;
	int		*data;

	int		bpp;
	int		size_l;
	int		endian;
}				t_img;

typedef struct	s_game
{
	void	*mlx;
	void	*win;
	t_img	img;

	int		map[24][24];
}				t_game;

int	key_press(int keycode, int x) // parameter check
{
	if (keycode == 53)
		exit(0);
	return (0);
}

void		verLine(t_game *game, int x, int y1, int y2, int color)
{
	double deltaX;
	double deltaY;
	double step;

	deltaX = 0;
	deltaY = y2 - y1;
	step = (fabs(deltaX) > fabs(deltaY)) ? fabs(deltaX) : fabs(deltaY);
	deltaY /= step;
	while (abs(y2 - y1) > 0.01)
	{
		game->img.data[TO_COORD(x, y1)] = 0xFFFFFF;;
		x += deltaX;
		y1 += deltaY;
	}
}

void	game_init(t_game *game)
{

	int map[mapWidth][mapHeight]=
	{
	  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
	memcpy(game->map, map, sizeof(int) * 24 * 24);
}

int	done(void *mlx, void *win)
{
	mlx_hook(win, 2, 0, &key_press, NULL);
	mlx_loop(mlx);
	return (1);
}

void	window_init(t_game *game)
{
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, 640, 480, "mlx 42");
}

void	img_init(t_game *game)
{
	game->img.img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	game->img.data = (int *)mlx_get_data_addr(game->img.img, &game->img.bpp, &game->img.size_l, &game->img.endian);
}

int	main(int argc, char **argv)
{
	double posX = 22, posY = 12;
	double dirX = -1, dirY = 0;
	double planeX = 0, planeY = 0.66;

	double time = 0;
	double oldtime = 0;
	
	t_game game;

	game_init(&game);
	window_init(&game);

	while (done(game.mlx, game.win))
	{
		for (int x = 0; x < 640; x++)
		{
			double cameraX = 2 * x / (double)640 - 1;
			double rayDirX = dirX + planeX * cameraX;
			double rayDirY = dirY + planeY * cameraX;
	
			int	mapX = (int)posX;
			int	mapY = (int)posY;

			double sideDistX;
			double sideDistY;

			double deltaDistX = fabs(1 / rayDirX);
			double deltaDistY = fabs(1 / rayDirY);
			double perpWallDist;

			int	stepX;
			int stepY;

			int hit = 0;
			int side;

			if (rayDirX < 0)
			{
				stepX = -1;
				sideDistX = (posX - mapX) * deltaDistX;
			}
			else
			{
				stepX = 1;
				sideDistX = (mapX + 1.0 - posX) * deltaDistX;
			}
			if (rayDirY < 0)
			{
				stepY = -1;
				sideDistY = (posY - mapY) * deltaDistY;
			}
			else
			{
				stepY = 1;
				sideDistY = (mapY + 1.0 - posY) * deltaDistY;
			}

			while (hit == 0)
			{
				if (sideDistX < sideDistY)
				{
					sideDistX += deltaDistX;
					mapX += stepX;
					side = 0;
				}
				else
				{
					sideDistY += deltaDistY;
					mapY += stepY;
					side = 1;
				}
				if (game.map[mapX][mapY] > 0)
					hit = 1;
			}

			if (side == 0)
				perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
			else
				perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

			int	lineHeight = (int)(480 / perpWallDist);

			int drawStart = -lineHeight / 2 + 480 / 2;
			if (drawStart < 0)
				drawStart = 0;
			int drawEnd = lineHeight / 2 + 480 / 2;
			if (drawEnd >= 480)
				drawEnd = 480 - 1;

			int color = 0xFFFFFF;
			if (side == 1)
				color = color / 2;
			verLine(&game, x, drawStart, drawEnd, color);
		}

		oldTime = time;
		time = geTicks();
		double frameTime = (time - oldtime) / 1000.0;
		print(1.0 / frameTime);
		redraw();
		cls();

		double moveSpeed = frameTime * 5.0;
		double rotSpeed = frameTime * 3.0;
	}
}
