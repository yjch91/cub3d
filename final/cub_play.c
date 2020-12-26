/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_play.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 04:10:35 by jayun             #+#    #+#             */
/*   Updated: 2020/12/26 16:42:24 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	draw(t_info *info)
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

static int	main_loop(t_info *info)
{
	ceilfloor_calc(info);
	wall_calc(info);
	sprite_calc(info);
	draw(info);
	if (info->bonus_on == 1)
	{
		draw_rectangles(info);
		draw_point(info, get_point_size(info));
		draw_lifebar(info);
		control_jump(info);
		if (info->flag_weap == 1)
			control_weap(info);
	}
	if (info->flag_w == 1)
		control_key_w(info);
	if (info->flag_s == 1)
		control_key_s(info);
	if (info->flag_d == 1)
		control_key_d(info);
	if (info->flag_a == 1)
		control_key_a(info);
	if (info->bitmap_check == 1)
		save_bitmap(info);
	return (0);
}

static void	kill_afplay(t_info *info)
{
	if (info->bonus_on == 1 && info->bitmap_check != 1)
		system("killall afplay");
}

void		all_free(t_info *info, t_map *m)
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
	i = -1;
	while (++i < 14)
		if (info->texture[i].img != 0)
			mlx_destroy_image(info->mlx, info->texture[i].img);
	if (info->win != 0)
		mlx_destroy_window(info->mlx, info->win);
	free(info->mlx);
	kill_afplay(info);
}

void		cub_play(t_map *m)
{
	t_info info;

	info_init(&info, m);
	winsize_init(&info, m);
	info_init2(&info);
	if (buf_alloc_init(&info, m) == 0)
	{
		kill_afplay(&info);
		return ;
	}
	if (sprite_alloc(&info, m) == 0)
		return ;
	imagedata_init(&info, m);
	fill_sprite_info(&info);
	win_img_init(&info, m);
	mlx_loop_hook(info.mlx, &main_loop, &info);
	mlx_hook(info.win, X_EVENT_KEY_PRESS, 0, &key_press, &info);
	mlx_hook(info.win, X_EVENT_KEY_RELEASE, 0, &key_release, &info);
	mlx_hook(info.win, X_EVENT_KEY_EXIT, 0, &button_redcross, &info);
	if (info.bonus_on == 1)
		mlx_hook(info.win, X_EVENT_MOTION_NOTIFY, 0, &mouse_move, &info);
	mlx_loop(info.mlx);
}
