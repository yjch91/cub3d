/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 07:51:44 by jayun             #+#    #+#             */
/*   Updated: 2020/12/27 22:24:48 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	dir_setting(t_info *info, t_map *m)
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

void		info_init(t_info *info, t_map *m)
{
	if ((info->mlx = mlx_init()) == 0)
	{
		write(1, "mlx_init result = NULL Error\n", 29);
		map_array_free(m, m->map_x, 1);
		if (m->bonus_on == 1 && m->bitmap_check != 1)
			system("killall afplay");
		exit(0);
	}
	info->img.img = 0;
	info->img2.img = 0;
	info->win = 0;
	info->bonus_on = m->bonus_on;
	info->movespeed = 0.05;
	info->rotspeed = 0.025;
	info->pos_x = m->start_x + 0.5;
	info->pos_y = m->start_y + 0.5;
	dir_setting(info, m);
	info->map = m->map;
	info->map_w = m->map_y;
	info->map_h = m->map_x;
	info->bitmap_check = m->bitmap_check;
	info->sprite_count = m->sprite_count;
	info->m = *m;
	info->floor_color = m->floor_color;
	info->ceil_color = m->ceil_color;
}

void		winsize_init(t_info *info, t_map *m)
{
	mlx_get_screen_size(info->mlx, &info->winsize_w, &info->winsize_h);
	if (info->winsize_w > m->w)
		info->winsize_w = m->w;
	else
		write(1, "width > maxsize so width = maxsize\n", 35);
	if (info->winsize_h > m->h)
		info->winsize_h = m->h;
	else
		write(1, "height > maxsize so height = maxsize\n", 37);
	if (info->winsize_w < 200)
	{
		info->winsize_w = 200;
		write(1, "width is very little so width = 200\n", 36);
	}
	if (info->winsize_h < 200)
	{
		info->winsize_h = 200;
		write(1, "height is very little so height = 200\n", 38);
	}
}

void		info_init2(t_info *info)
{
	int i;

	info->pitch = 0;
	info->jump = 0;
	info->hp = 100;
	info->mouse_x = info->winsize_w / 2;
	info->flag_w = 0;
	info->flag_a = 0;
	info->flag_s = 0;
	info->flag_d = 0;
	info->flag_c = 0;
	info->flag_sp = 0;
	info->flag_sky = 0;
	info->flag_weap = 0;
	info->flag_shot = 0;
	i = -1;
	while (++i < 14)
		info->texture[i].img = 0;
}

void		win_init(t_info *info, t_map *m)
{
	if ((info->win = mlx_new_window(
					info->mlx, info->winsize_w, info->winsize_h, "cub3D")) == 0)
	{
		all_free(info, m);
		write(1, "mlx_new_window return = NULL Error\n", 35);
		exit(0);
	}
}
