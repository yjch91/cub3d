/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_key_left_right.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 06:16:31 by jayun             #+#    #+#             */
/*   Updated: 2020/12/28 01:37:30 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	control_key_left(t_info *info)
{
	double olddir_x;
	double oldplane_x;

	olddir_x = info->dir_x;
	info->dir_x = info->dir_x * cos(info->rotspeed) -
		info->dir_y * sin(info->rotspeed);
	info->dir_y = olddir_x * sin(info->rotspeed) +
		info->dir_y * cos(info->rotspeed);
	oldplane_x = info->plane_x;
	info->plane_x = info->plane_x * cos(info->rotspeed) -
		info->plane_y * sin(info->rotspeed);
	info->plane_y = oldplane_x * sin(info->rotspeed) +
		info->plane_y * cos(info->rotspeed);
}

void	control_key_right(t_info *info)
{
	double olddir_x;
	double oldplane_x;

	olddir_x = info->dir_x;
	info->dir_x = info->dir_x * cos(-info->rotspeed) -
		info->dir_y * sin(-info->rotspeed);
	info->dir_y = olddir_x * sin(-info->rotspeed) +
		info->dir_y * cos(-info->rotspeed);
	oldplane_x = info->plane_x;
	info->plane_x = info->plane_x * cos(-info->rotspeed) -
		info->plane_y * sin(-info->rotspeed);
	info->plane_y = oldplane_x * sin(-info->rotspeed) +
		info->plane_y * cos(-info->rotspeed);
}
