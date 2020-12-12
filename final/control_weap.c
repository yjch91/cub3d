/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_weap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 07:09:36 by jayun             #+#    #+#             */
/*   Updated: 2020/12/12 07:13:01 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	control_weap(t_info *info)
{
	int	a;
	int	b;

	a = info->winsize_w - 460;
	b = info->winsize_h - 300;
	if (a < 0 || a < info->winsize_w / 2)
		a = info->winsize_w / 4;
	if (b < 0 || b < info->winsize_h / 2)
		b = info->winsize_h / 2;
	mlx_put_image_to_window(info->mlx, info->win,
			info->texture[12 + info->flag_shot].img, a, b);
	mlx_put_image_to_window(info->mlx, info->win,
			info->texture[11].img, info->winsize_w / 2, info->winsize_h / 2);
}

void	weap_shot(t_info *info)
{
	int x;
	int y;

	info->shot_x = info->pos_x;
	info->shot_y = info->pos_y;
	x = (int)info->shot_x;
	y = (int)info->shot_y;
	while ((x >= 0 && x < info->map_h) && (y >= 0 && y < info->map_w) && info->
			shot_x < info->pos_x + 5.0 && info->shot_y < info->pos_y + 5.0)
	{
		info->shot_x += info->dir_x * 0.2;
		info->shot_y += info->dir_y * 0.2;
		x = (int)info->shot_x;
		y = (int)info->shot_y;
		if ((x >= 0 && x < info->map_h) && (y >= 0 && y < info->map_w) &&
				info->map[x][y] == 3)
		{
			info->map[x][y] = 0;
			remove_sprite(info, x, y);
			break ;
		}
		else if ((x >= 0 && x < info->map_h) && (y >= 0 && y < info->map_w) &&
				(info->map[x][y] == 1 || info->map[x][y] == 2))
			break ;
	}
}
