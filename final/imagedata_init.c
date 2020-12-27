/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imagedata_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 08:57:39 by jayun             #+#    #+#             */
/*   Updated: 2020/12/27 22:23:02 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	get_imagedata(t_info *info, char *path, int n)
{
	if ((info->texture[n].img =
				mlx_xpm_file_to_image(info->mlx, path,
					&info->texture[n].img_width,
					&info->texture[n].img_height)) == 0)
	{
		perror(path);
		write(1, "Error\n", 6);
		all_free(info, &info->m);
		exit(0);
	}
	if ((info->texture[n].data =
				(int *)mlx_get_data_addr(info->texture[n].img,
					&info->texture[n].bpp,
					&info->texture[n].size_l,
					&info->texture[n].endian)) == 0)
	{
		write(1, "mlx_get_data_addr result = NULL Error\n", 38);
		all_free(info, &info->m);
		exit(0);
	}
}

static void	imagedata_init2(t_info *info)
{
	get_imagedata(info, "./textures/robot.xpm", 7);
	get_imagedata(info, "./textures/health_s.xpm", 8);
	get_imagedata(info, "./textures/health_b.xpm", 9);
	get_imagedata(info, "./textures/weap_spr.xpm", 10);
	get_imagedata(info, "./textures/crosshair.xpm", 11);
	get_imagedata(info, "./textures/M2GFB0.xpm", 12);
	get_imagedata(info, "./textures/M2GFA0.xpm", 13);
}

void		imagedata_init(t_info *info, t_map *m)
{
	get_imagedata(info, m->north, 0);
	get_imagedata(info, m->south, 1);
	get_imagedata(info, m->east, 2);
	get_imagedata(info, m->west, 3);
	if (info->floor_color == -1)
		get_imagedata(info, m->floor, 4);
	if (info->ceil_color == -1)
	{
		if (ft_strnstr(m->ceil, "./textures/skybox", 17) != 0)
			info->flag_sky = 1;
		get_imagedata(info, m->ceil, 5);
	}
	get_imagedata(info, m->item, 6);
	if (info->bonus_on == 1)
		imagedata_init2(info);
}

void		img_init(t_info *info, t_map *m, t_img *img)
{
	if ((img->img = mlx_new_image(
					info->mlx, info->winsize_w, info->winsize_h)) == 0)
	{
		all_free(info, m);
		write(1, "mlx_new_image return = NULL Error\n", 34);
		exit(0);
	}
	if ((img->data = (int *)mlx_get_data_addr(img->img,
					&img->bpp, &img->size_l, &img->endian)) == 0)
	{
		all_free(info, m);
		write(1, "mlx_get_data_addr return = NULL Error\n", 38);
		exit(0);
	}
}
