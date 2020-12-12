/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_mouse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 08:15:40 by jayun             #+#    #+#             */
/*   Updated: 2020/12/12 08:27:14 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	mouse_move_left(int x, t_info *info)
{
	double	olddir_x;
	double	oldplane_x;

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
	info->mouse_x = x;
}

static void	mouse_move_right(int x, t_info *info)
{
	double olddir_x;
	double oldplane_x;

	olddir_x = info->dir_x;
	info->dir_x = info->dir_x * cos(info->rotspeed) +
		info->dir_y * sin(info->rotspeed);
	info->dir_y = -olddir_x * sin(info->rotspeed) +
		info->dir_y * cos(info->rotspeed);
	oldplane_x = info->plane_x;
	info->plane_x = info->plane_x * cos(info->rotspeed) +
		info->plane_y * sin(info->rotspeed);
	info->plane_y = -oldplane_x * sin(info->rotspeed) +
		info->plane_y * cos(info->rotspeed);
	info->mouse_x = x;
}

int			mouse_move(int x, int y, t_info *info)
{
	if (x >= 0 && y >= 0 && x <= info->winsize_w &&
			y <= info->winsize_h && x < info->mouse_x)
		mouse_move_left(x, info);
	else if (x >= 0 && y >= 0 && x <= info->winsize_w &&
			y <= info->winsize_h && x > info->mouse_x)
		mouse_move_right(x, info);
	return (0);
}
