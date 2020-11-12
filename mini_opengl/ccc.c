#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "mlx.h"

#define X_EVENT_KEY_PRESS		2
#define X_EVENT_KEY_EXIT		17 //Exit program key code

#define KEY_ESC			53
#define KEY_A			0
#define KEY_S			1
#define KEY_D			2
#define KEY_W			13

# define TILE_SIZE 64
# define ROWS 8
# define COLS 8
# define WIDTH COLS * TILE_SIZE * 2
# define HEIGHT ROWS * TILE_SIZE
# define PI 3.1415926535
# define P2 PI / 2
# define P3 3 * PI / 2
# define DR 0.0174533 // one degree in radians

# define TO_COORD(X, Y) ((int)floor(Y) * WIDTH + (int)floor(X))


int	count = 0;
double pdx, pdy, pa;
int		linecolor = 0xFFFFFF;

typedef struct	s_img
{
	void	*img;
	int		*data;

	int		bpp;
	int		size_l;
	int		endian;
}				t_img;

typedef	struct	s_game
{
	void	*mlx;
	void	*win;
	t_img	img;
	double	playerx;
	double	playery;

	int		map[ROWS][COLS];
}				t_game;

//Draw the line by DDA algorithm
void	draw_line(t_game *game, double x1, double y1, double x2, double y2)
{
	double	deltaX;
	double	deltaY;
	double	step;

	deltaX = x2 - x1;
	deltaY = y2 - y1;
	step = (fabs(deltaX) > fabs(deltaY)) ? fabs(deltaX) : fabs(deltaY);
	if (step != 0)
	{
		deltaX /= step;
		deltaY /= step;
		while (fabs(x2 - x1) > 0.01 || fabs(y2 - y1) > 0.01)
		{
			game->img.data[TO_COORD(x1, y1)] = linecolor;;
			x1 += deltaX;
			y1 += deltaY;
		}
	}
}

void 	draw_lines(t_game *game)
{
	int		i;
	int		j;

	i = 0;
	while (i < COLS)
	{
		draw_line(game, i * TILE_SIZE, 0, i * TILE_SIZE, HEIGHT);
		i++;
	}
	draw_line(game, COLS * TILE_SIZE - 1, 0, COLS * TILE_SIZE - 1, HEIGHT);
	j = 0;
	while (j < ROWS)
	{
		draw_line(game, 0, j * TILE_SIZE, WIDTH / 2, j * TILE_SIZE);
		j++;
	}
	draw_line(game, 0, ROWS * TILE_SIZE - 1, WIDTH / 2, ROWS * TILE_SIZE - 1);
	// WIDTH / 2 <<<<<< !!!!!!!!!!!!!!!!!!!
}

double dist(double ax, double ay, double bx, double by, double ang)
{
	return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)) );
}

void	draw_Ray(t_game *game)
{
	int	r, mx, my, mp, dof; double rx, ry, ra, xo, yo, disT;

	ra = pa - DR * 30;
	if (ra < 0)
		ra += 2 * PI;
	if (ra > 2 * PI)
		ra -= 2 * PI;

	for (r = 0; r < 60; r++)
	{
		// horizontal Line
		dof = 0;
		double disH = 1000000;
	    double hx = game->playerx;
	   	double hy = game->playery;
		double aTan = -1 / tan(ra);
		if (ra > PI)
		{
			ry = (((int)game->playery >> 6) << 6) - 0.0001;
			rx = (game->playery - ry) * aTan + game->playerx;
			yo = -64;
			xo = -yo * aTan;
		}
		if (ra < PI)
		{
			ry = (((int)game->playery >> 6) << 6) + 64;
			rx = (game->playery - ry) * aTan + game->playerx;
			yo = 64;
			xo = -yo * aTan;
		}
		if (ra == 0 || ra == PI)
		{
			rx = game->playerx;
			ry = game->playery;;
			dof = 8;
		}
		while (dof < 8)
		{
			mx = ((int)(rx) >> 6);
			my = ((int)(ry) >> 6);
			mp = my * 8 + mx;
			if (mp > 0 && mp < 8 * 8 && game->map[my][mx] == 1)
			{
				hx = rx;
				hy = ry;
				disH = dist(game->playerx, game->playery, hx, hy, ra);
				dof = 8;
			}
			else
			{
				rx += xo;
				ry += yo;
				dof += 1;
			}
		}
		//linecolor = 0x000000;
		// draw_line(game, (int)game->playerx, (int)game->playery, (int)rx, (int)ry);
		//linecolor = 0xFFFFFF;

		// vertical line
		dof = 0;
		double disV = 1000000;
	    double vx = game->playerx;
	    double vy = game->playery;
		double nTan = -tan(ra);
		if (ra > P2 && ra < P3)
		{
			rx = (((int)game->playerx >> 6) << 6) - 0.0001;
			ry = (game->playerx - rx) * nTan + game->playery;
			xo = -64;
			yo = -xo * nTan;
		}
		if (ra < P2 || ra > P3)
		{
			rx = (((int)game->playerx >> 6) << 6) + 64;
			ry = (game->playerx - rx) * nTan + game->playery;
			xo = 64;
			yo = -xo * nTan;
		}
		if (ra == 0 || ra == PI)
		{
			rx = game->playerx;
			ry = game->playery;;
			dof = 8;
		}
		while (dof < 8)
		{
			mx = ((int)(rx) >> 6);
			my = ((int)(ry) >> 6);
			mp = my * 8 + mx;
			if (mp > 0 && mp < 8 * 8 && game->map[my][mx] == 1)
			{
				dof = 8;
				vx = rx;
				vy = ry;
				disV = dist(game->playerx, game->playery, vx, vy, ra);
			}
			else
			{
				rx += xo;
				ry += yo;
				dof += 1;
			}
		}

		
		if (disV < disH)
		{
			rx = vx; ry = vy; disT = disV;
			linecolor = 0xFF0000;
		}
		if (disH < disV)
		{
			rx = hx, ry = hy; disT = disH;
			linecolor = 0xAA0000;
		}
	//	printf("rx = %.3f ry = %.3f mx = %d my = %d mp = %d\n", rx, ry, mx, my, mp);

		draw_line(game, (int)game->playerx, (int)game->playery, (int)rx, (int)ry);

		double ca = pa - ra;
		if (ca < 0)
			ca += 2 * PI;
		else if (ca > 2 * PI)
			ca -= 2 * PI;
		disT = disT * cos(ca);

		double lineH = 64 * 420 / disT;
		double lineO = 160 - (lineH / 2);
		
		if (lineH > 420)
			lineH = 420;
		if (lineO < 0)
			lineO = 0;
		//printf("lineH = %f lineO = %f %f\n", lineH, lineO, lineH + lineO);		
		//printf("%d %d %d %d\n", r * 8 + 530, (int)lineO, r * 8 + 530, (int)(lineH + lineO));
		for(int j = 0; j <= 8; j++)
			draw_line(game, r * 8 + 530 + j, (int)(lineO), r * 8 + 530 + j, (int)(lineH + lineO));
		//draw_line(game, 530, 0, (int)game->playerx, (int)game->playery);
		linecolor = 0xFFFFFF;

		ra += DR;
		if (ra < 0)
			ra += 2 * PI;
		if (ra > 2 * PI)
			ra -= 2 * PI;
	}
}

void	draw_point(t_game *game, int size)
{
	int	i;
	int	j;
	int	x;
	int	y;
	int color;

	x = game->playerx;
	y = game->playery;
	color = 0xFFFAFA;
	i = y - (size / 2);
	while (i < y + (size / 2))
	{
		j = x - (size / 2);
		while (j < x + (size / 2))
		{
			game->img.data[i * WIDTH + j] = color;
			j++;
		}
		i++;
	}
}

void	draw_rectangle(t_game *game, int x, int y)
{
	int i;
	int j;
	int	color;

	if (game->map[y][x] == 1)
		color = 0xFF0000;
	else
		color = 0x0000FF;

	x *= TILE_SIZE;
	y *= TILE_SIZE;
	i = 0;
	while (i < TILE_SIZE)
	{
		j = 0;
		while (j < TILE_SIZE)
		{
			game->img.data[(y + i) * WIDTH + x + j] = color;
			j++;
		}
		i++;
	}
}

void	draw_rectangles(t_game *game)
{
	int		i;
	int		j;

	i = 0;
	while (i < ROWS)
	{
		j = 0;
		while (j < COLS)
		{
			if (game->map[i][j] == 1)
				draw_rectangle(game, j, i);
			else
				draw_rectangle(game, j, i);
			j++;
		}
		i++;
	}
}

int		deal_key(int key_code, t_game *game)
{
	if (key_code == KEY_ESC)
		exit(0);
	else if (key_code == KEY_W)
	{
		if (game->playerx + pdx < WIDTH)
			game->playerx += pdx;
		if (game->playery + pdy < HEIGHT)
			game->playery += pdy;
	}
	else if (key_code == KEY_S)
	{
		if (game->playerx - pdx > 0)
			game->playerx -= pdx;
		if (game->playery - pdy > 0)
			game->playery -= pdy;
	}
	else if (key_code == KEY_A)
	{
		pa -= 0.1;
		if (pa < 0)
			pa += 2 * PI;
		pdx = cos(pa) * 5;
		pdy = sin(pa) * 5;
	}
	else if (key_code == KEY_D)
	{
		pa += 0.1;
		if (pa > 2 * PI)
			pa -= 2 * PI;
		pdx = cos(pa) * 5;
		pdy = sin(pa) * 5;
	}
	return (0);
}

int 	close(t_game *game)
{
		exit(0);
}

void	game_init(t_game *game)
{
	game->playerx = 256.0;
	game->playery = 256.0;

	int map[ROWS][COLS] = {
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 1, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1},
	};
	memcpy(game->map, map, sizeof(int) * ROWS * COLS);
}

void	window_init(t_game *game)
{
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "mlx 42");
}

void	img_init(t_game *game)
{
	game->img.img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	game->img.data = (int *)mlx_get_data_addr(game->img.img, &game->img.bpp, &game->img.size_l, &game->img.endian);
}

int		main_loop(t_game *game)
{
	for(int i = 0; i <= 1024 * 512; i++)
		game->img.data[i] = 0x000000;

	draw_rectangles(game);
	draw_lines(game);
	draw_point(game, 10);
	draw_Ray(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
	return (0);
}

int		main(void)
{
	t_game game;

	pa = 0.0;
	pdx = cos(pa) * 5;
	pdy = sin(pa) * 5;

	game_init(&game);
	window_init(&game);
	img_init(&game);

//	mlx_hook(game.win, X_EVENT_KEY_PRESS, 0, &deal_key, &game);
	mlx_hook(game.win, X_EVENT_KEY_EXIT, 0, &close, &game);

//	draw_point(&game, 10);
//	mlx_put_image_to_window(game.mlx, game.win, game.img.img, 0, 0);

	mlx_hook(game.win, X_EVENT_KEY_PRESS, 0, &deal_key, &game);

	mlx_loop_hook(game.mlx, &main_loop, &game);
	mlx_loop(game.mlx);
}
