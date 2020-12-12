/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_jump.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 06:37:28 by jayun             #+#    #+#             */
/*   Updated: 2020/12/12 06:42:36 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	control_jump(t_info *info)
{
	if (info->jump > 0)
	{
		if (0 > info->jump - info->winsize_h / 4 * info->movespeed)
			info->jump = 0;
		else
			info->jump = info->jump - info->winsize_h / 4 * info->movespeed;
	}
	if (info->jump < 0 && info->flag_c == 0)
	{
		if (0 > info->jump + info->winsize_h / 3 * info->movespeed)
			info->jump = 0;
		else
			info->jump = info->jump + info->winsize_h / 3 * info->movespeed;
	}
	else if (info->jump == 0)
		info->flag_sp = 0;
}
