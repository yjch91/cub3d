#include <stdio.h>
#include <stdlib.h>
#include "mlx.h"

typedef struct	s_param{
	int x;
	int y;
}				t_param;

typedef struct	s_img
{
	void	*img_ptr;
	int		*data;
	int		width;
	int		height;

	int		bpp;
	int		size_l;;
	int		endian;
}				t_img;

typedef struct	s_mlx
{
	void	*mlx_ptr;
	void	*win;
}				t_mlx;

void	param_init(t_param *param)
{
	param->x = 0;
	param->y = 0;
}

int	key_press(int keycode, t_param *param)
{
	if (keycode == 53)
		exit(0);
	return (0);
}

int	main(void)
{
	t_mlx	*mlx;
	int		count_w;
	int		count_h;
	t_img	img1;
	t_img	img2;
	t_img	img3;
	t_param  param;

	param_init(&param);
	mlx->mlx_ptr = mlx_init();
	mlx->win = mlx_new_window(mlx->mlx_ptr,800, 600, "hello");


	img1.img_ptr = mlx_xpm_file_to_image(mlx->mlx_ptr, "wall_n.xpm", &img1.width, &img1.height);	// image print
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win, img1.img_ptr, 12, 12);								// image print
	
	
	img2.img_ptr = mlx_new_image(mlx->mlx_ptr, 400, 300);
	img2.data = (int *)mlx_get_data_addr(img2.img_ptr, &img2.bpp, &img2.size_l, &img2.endian);

	count_h = -1;
	while (++count_h < 300)
	{
		count_w = -1;
		while (++count_w < 400)
		{
			if (count_w % 2)
				img2.data[count_h * 400 + count_w] = 0xFFFFFF;
			else
				img2.data[count_h * 400 + count_w] = 0xFF0000;
		}
	}
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win, img2.img_ptr, 0, 100);

	img3.img_ptr = mlx_xpm_file_to_image(mlx->mlx_ptr, "wall_n.xpm", &img3.width, &img3.height);
	img3.data = (int *)mlx_get_data_addr(img3.img_ptr, &img3.bpp, &img3.size_l, &img3.endian);

	count_h = -1;
	while (++count_h < img3.height)
	{
		count_w = -1;
		while (++count_w < img3.width)
		{
			if (count_w % 2)
				img3.data[count_h * img3.width + count_w] = 0xFFFFFF;
			else
				img3.data[count_h * img3.width + count_w] = 0xFF0000;
		}
	}
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win, img3.img_ptr, 400, 100);


	mlx_hook(mlx->win, 2, 0, &key_press, NULL);
	mlx_loop(mlx->mlx_ptr);
}
