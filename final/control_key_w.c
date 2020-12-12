/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_key_w.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 05:59:41 by jayun             #+#    #+#             */
/*   Updated: 2020/12/12 08:17:46 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	control_key_w_x(t_info *info, int n)
{
	if (n == 0)
		info->pos_x += info->dir_x * info->movespeed;
	else if (n == 4 || n == 5 || n == 6)
	{
		info->map[(int)(info->pos_x + info->dir_x * info->movespeed)]
			[(int)(info->pos_y)] = 0;
		remove_sprite(info, (int)(info->pos_x + info->dir_x * info->movespeed),
				(int)(info->pos_y));
		info->pos_x += info->dir_x * info->movespeed;
		if (n == 4)
			info->hp += 10;
		else if (n == 5)
			info->hp -= 10;
		else
			info->flag_weap = 1;
	}
}

static void	control_key_w_y(t_info *info, int n)
{
	if (n == 0)
		info->pos_y += info->dir_y * info->movespeed;
	else if (n == 4 || n == 5 || n == 6)
	{
		info->map[(int)(info->pos_x)]
			[(int)(info->pos_y + info->dir_y * info->movespeed)] = 0;
		remove_sprite(info, (int)(info->pos_x),
				(int)(info->pos_y + info->dir_y * info->movespeed));
		info->pos_y += info->dir_y * info->movespeed;
		if (n == 4)
			info->hp += 10;
		else if (n == 5)
			info->hp -= 10;
		else
			info->flag_weap = 1;
	}
}

void		control_key_w(t_info *info)
{
	int	n;

	n = info->map[(int)(info->pos_x + info->dir_x * info->movespeed)]
		[(int)(info->pos_y)];
	control_key_w_x(info, n);
	n = info->map[(int)(info->pos_x)]
		[(int)(info->pos_y + info->dir_y * info->movespeed)];
	control_key_w_y(info, n);
}
