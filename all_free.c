/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/28 01:37:37 by jayun             #+#    #+#             */
/*   Updated: 2020/12/28 01:57:12 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	all_free2(t_info *info)
{
	int i;

	i = -1;
	while (++i < 14)
	{
		if (info->texture[i].img != 0)
			mlx_destroy_image(info->mlx, info->texture[i].img);
	}
	if (info->win != 0)
		mlx_destroy_window(info->mlx, info->win);
	free(info->mlx);
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
	if (info->img2.img != 0)
		mlx_destroy_image(info->mlx, info->img2.img);
	all_free2(info);
}
