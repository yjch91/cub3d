/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_key_s.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 06:09:43 by jayun             #+#    #+#             */
/*   Updated: 2020/12/27 20:47:23 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	control_key_s_x(t_info *info, int n)
{
	if (n == 0)
		info->pos_x -= info->dir_x * info->movespeed;
	else if (n == 4 || n == 5 || n == 6)
	{
		info->map[(int)(info->pos_x - info->dir_x * info->movespeed)]
			[(int)(info->pos_y)] = 0;
		remove_sprite(info, (int)(info->pos_x - info->dir_x * info->movespeed),
				(int)(info->pos_y));
		info->pos_x -= info->dir_x * info->movespeed;
		if (n == 4 && (info->hp + 10) <= 150)
			info->hp += 10;
		else if (n == 5)
			info->hp -= 10;
		else if (n == 6)
			info->flag_weap = 1;
	}
}

static void	control_key_s_y(t_info *info, int n)
{
	if (n == 0)
		info->pos_y -= info->dir_y * info->movespeed;
	else if (n == 4 || n == 5 || n == 6)
	{
		info->map[(int)(info->pos_x)]
			[(int)(info->pos_y - info->dir_y * info->movespeed)] = 0;
		remove_sprite(info, (int)(info->pos_x),
				(int)(info->pos_y - info->dir_y * info->movespeed));
		info->pos_y -= info->dir_y * info->movespeed;
		if (n == 4 && (info->hp + 10) <= 150)
			info->hp += 10;
		else if (n == 5)
			info->hp -= 10;
		else if (n == 6)
			info->flag_weap = 1;
	}
}

void		control_key_s(t_info *info)
{
	int	n;

	n = info->map[(int)(info->pos_x - info->dir_x * info->movespeed)]
		[(int)(info->pos_y)];
	control_key_s_x(info, n);
	n = info->map[(int)(info->pos_x)]
		[(int)(info->pos_y - info->dir_y * info->movespeed)];
	control_key_s_y(info, n);
}
