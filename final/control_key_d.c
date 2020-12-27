/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_key_d.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/28 01:44:04 by jayun             #+#    #+#             */
/*   Updated: 2020/12/28 02:18:28 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	control_key_d_x(t_info *info, int n, double vdir_x)
{
	if (n == 0)
		info->pos_x += vdir_x * info->movespeed;
	else if (n == 4 || n == 5 || n == 6)
	{
		info->map[(int)(info->pos_x + vdir_x * info->movespeed)]
			[(int)(info->pos_y)] = 0;
		remove_sprite(info, (int)(info->pos_x + vdir_x * info->movespeed),
				(int)(info->pos_y));
		info->pos_x += vdir_x * info->movespeed;
		if (n == 4 && (info->hp + 10) <= 150)
			info->hp += 10;
		else if (n == 5)
			info->hp -= 10;
		else if (n == 6)
			info->flag_weap = 1;
	}
}

static void	control_key_d_y(t_info *info, int n, double vdir_y)
{
	if (n == 0)
		info->pos_y += vdir_y * info->movespeed;
	else if (n == 4 || n == 5 || n == 6)
	{
		info->map[(int)(info->pos_x)]
			[(int)(info->pos_y + vdir_y * info->movespeed)] = 0;
		remove_sprite(info, (int)(info->pos_x),
				(int)(info->pos_y + vdir_y * info->movespeed));
		info->pos_y += vdir_y * info->movespeed;
		if (n == 4 && (info->hp + 10) <= 150)
			info->hp += 10;
		else if (n == 5)
			info->hp -= 10;
		else if (n == 6)
			info->flag_weap = 1;
	}
}

void		control_key_d(t_info *info)
{
	int		n;
	double	len;
	double	vdir_x;
	double	vdir_y;

	len = sqrt(info->plane_x * info->plane_x + info->plane_y * info->plane_y);
	vdir_x = info->plane_x / len;
	vdir_y = info->plane_y / len;
	n = info->map[(int)(info->pos_x + vdir_x * info->movespeed)]
		[(int)(info->pos_y)];
	control_key_d_x(info, n, vdir_x);
	n = info->map[(int)(info->pos_x)]
		[(int)(info->pos_y + vdir_y * info->movespeed)];
	control_key_d_y(info, n, vdir_y);
}
