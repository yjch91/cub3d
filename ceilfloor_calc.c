/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ceilfloor_calc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 08:43:44 by jayun             #+#    #+#             */
/*   Updated: 2020/12/27 02:47:28 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	ceilfloor_calc1(t_info *info, int y)
{
	info->raydir_x0 = info->dir_x - info->plane_x;
	info->raydir_y0 = info->dir_y - info->plane_y;
	info->raydir_x1 = info->dir_x + info->plane_x;
	info->raydir_y1 = info->dir_y + info->plane_y;
	info->is_floor = y > info->winsize_h / 2 + info->pitch;
	if (info->is_floor == 1)
		info->p = y - info->winsize_h / 2 - info->pitch;
	else
		info->p = info->winsize_h / 2 - y + info->pitch;
	if (info->is_floor == 1)
		info->posz = 0.5 * info->winsize_h + info->jump;
	else
		info->posz = 0.5 * info->winsize_h - info->jump;
	info->rowdist = fabs(info->posz / info->p);
	info->fstep_x = info->rowdist * (info->raydir_x1 - info->raydir_x0)
		/ info->winsize_w;
	info->fstep_y = info->rowdist * (info->raydir_y1 - info->raydir_y0)
		/ info->winsize_w;
	info->floor_x = info->pos_x + info->rowdist * info->raydir_x0;
	info->floor_y = info->pos_y + info->rowdist * info->raydir_y0;
	if (info->is_floor == 1)
		info->texnum = 4;
	else
		info->texnum = 5;
}

static void	ceilfloor_calc2(t_info *info)
{
	info->sh = info->rowdist;
	info->cell_x = (int)info->floor_x;
	info->cell_y = (int)info->floor_y;
	info->tex_x = (int)(info->texture[info->texnum].img_width *
			(info->floor_x - info->cell_x))
		& (info->texture[info->texnum].img_width - 1);
	info->tex_y = (int)(info->texture[info->texnum].img_height *
			(info->floor_y - info->cell_y))
		& (info->texture[info->texnum].img_height - 1);
	info->floor_x += info->fstep_x;
	info->floor_y += info->fstep_y;
}

static void	fill_floor_color(t_info *info)
{
	if (info->floor_color == -1)
		info->color = info->texture[info->texnum].data[(info->
				texture[info->texnum].size_l / 4) * info->tex_y + info->tex_x];
	else
		info->color = info->floor_color;
	if (info->flag_sky != 1 && info->bonus_on == 1)
		info->color = shade_color(info->sh, info->color);
}

static void	fill_ceil_color(t_info *info, int x, int y)
{
	int	a;
	int	b;

	if (info->flag_sky == 1)
	{
		a = y * info->texture[5].img_width / info->winsize_h;
		a = info->texture[5].img_width - a - 1;
		b = x * info->texture[5].img_height / info->winsize_w;
		info->color = info->texture[5].data[
				b * (info->texture[5].size_l / 4) + a];
	}
	else if (info->flag_sky != 1)
	{
		if (info->ceil_color == -1)
			info->color = info->texture[info->texnum].data[(info->texture[
					info->texnum].size_l / 4) * info->tex_y + info->tex_x];
		else
			info->color = info->ceil_color;
		if (info->bonus_on == 1)
			info->color = shade_color(info->sh, info->color);
	}
}

void		ceilfloor_calc(t_info *info)
{
	int	x;
	int	y;

	y = 0;
	while (y < info->winsize_h)
	{
		ceilfloor_calc1(info, y);
		x = 0;
		while (x < info->winsize_w)
		{
			ceilfloor_calc2(info);
			if (info->is_floor)
				fill_floor_color(info);
			else
				fill_ceil_color(info, x, y);
			info->buf[y][x] = info->color;
			x++;
		}
		y++;
	}
}
