/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifebar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 08:40:06 by jayun             #+#    #+#             */
/*   Updated: 2020/12/12 08:40:58 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	put_image_map_lifebar(t_info *info, int h)
{
	char*hp_str;

	hp_str = ft_itoa((int)info->hp);
	mlx_put_image_to_window(info->mlx, info->win, info->img.img, 0, 0);
	if (((info->winsize_h * 2 / 3) + ((100 - info->hp) / 100) * h) <
			info->winsize_h - 20)
		mlx_string_put(info->mlx, info->win, 0,
				(info->winsize_h * 2 / 3) + ((100 - info->hp) / 100) * h,
				0x00FF00, hp_str);
	else
		mlx_string_put(info->mlx, info->win, 0,
				info->winsize_h - 20, 0x00FF00, hp_str);
	free(hp_str);
}

void		draw_lifebar(t_info *info)
{
	int	x;
	int y;
	int	w;
	int	h;

	if (info->hp <= 0)
	{
		write(1, "game over\n", 10);
		all_free(info, &info->m);
		exit(0);
	}
	w = info->winsize_w / 20;
	if (w < 40)
		w = 40;
	h = info->winsize_h / 3;
	x = -1;
	while (++x < w)
	{
		y = (info->winsize_h * 2 / 3) + ((100 - info->hp) / 100 * h) - 1;
		while (++y < info->winsize_h)
			info->img.data[y * info->winsize_w + x] = 0xAAFF0000;
	}
	put_image_map_lifebar(info, h);
}
