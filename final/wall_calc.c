/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_calc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 08:49:58 by jayun             #+#    #+#             */
/*   Updated: 2020/12/12 09:18:40 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	wall_calc_sidedist(t_info *info)
{
	if (info->raydir_x < 0)
	{
		info->step_x = -1;
		info->sidedist_x = (info->pos_x - info->map_x) * info->deltadist_x;
	}
	else
	{
		info->step_x = 1;
		info->sidedist_x = (info->map_x + 1.0 - info->pos_x) *
			info->deltadist_x;
	}
	if (info->raydir_y < 0)
	{
		info->step_y = -1;
		info->sidedist_y = (info->pos_y - info->map_y) * info->deltadist_y;
	}
	else
	{
		info->step_y = 1;
		info->sidedist_y = (info->map_y + 1.0 - info->pos_y) *
			info->deltadist_y;
	}
}

static void	wall_calc1(t_info *info)
{
	if (info->side == 0)
		info->perpwalldist =
			(info->map_x - info->pos_x + (1 - info->step_x) / 2) /
			info->raydir_x;
	else
		info->perpwalldist =
			(info->map_y - info->pos_y + (1 - info->step_y) / 2) /
			info->raydir_y;
	info->lineheight = (int)(info->winsize_h / info->perpwalldist);
	info->drawstart = -(info->lineheight) / 2 + info->winsize_h / 2 +
		info->pitch + (info->jump / info->perpwalldist);
	if (info->drawstart < 0)
		info->drawstart = 0;
	info->drawend = info->lineheight / 2 + info->winsize_h / 2 +
		info->pitch + (info->jump / info->perpwalldist);
	if (info->drawend >= info->winsize_h)
		info->drawend = info->winsize_h - 1;
	if (info->side == 0)
		info->wall_x = info->pos_y + info->perpwalldist * info->raydir_y;
	else
		info->wall_x = info->pos_x + info->perpwalldist * info->raydir_x;
	info->wall_x -= floor(info->wall_x);
}

static void	wall_calc2(t_info *info)
{
	if (info->side == 0 && info->raydir_x < 0)
		info->texnum = 1;
	if (info->side == 0 && info->raydir_x > 0)
		info->texnum = 0;
	if (info->side == 1 && info->raydir_y > 0)
		info->texnum = 3;
	if (info->side == 1 && info->raydir_y < 0)
		info->texnum = 2;
	info->tex_x = (int)(info->wall_x *
			(double)info->texture[info->texnum].img_width);
	if (info->side == 0 && info->raydir_x > 0)
		info->tex_x = info->texture[info->texnum].img_width - info->tex_x - 1;
	if (info->side == 1 && info->raydir_y < 0)
		info->tex_x = info->texture[info->texnum].img_width - info->tex_x - 1;
	info->step = 1.0 * info->texture[info->texnum].img_height /
		info->lineheight;
	info->texpos = (info->drawstart - info->pitch -
			(info->jump / info->perpwalldist) -
			info->winsize_h / 2 + info->lineheight / 2) * info->step;
}

static void	fill_wall_color(t_info *info, int x)
{
	int	y;

	y = info->drawstart;
	while (y < info->drawend)
	{
		info->tex_y = (int)(info->texpos) &
			(info->texture[info->texnum].img_height - 1);
		info->texpos += info->step;
		info->sh = info->perpwalldist;
		info->color = info->texture[info->texnum].data[
			info->texture[info->texnum].img_width * info->tex_y + info->tex_x];
		if (info->flag_sky != 1 && info->bonus_on == 1)
			info->color = shade_color(info->sh, info->color);
		info->buf[y][x] = info->color;
		y++;
	}
	info->zbuffer[x] = info->perpwalldist;
}

void		wall_calc(t_info *info)
{
	int	x;

	x = 0;
	while (x < info->winsize_w)
	{
		info->camera_x = 2 * x / (double)info->winsize_w - 1;
		info->raydir_x = info->dir_x + info->plane_x * info->camera_x;
		info->raydir_y = info->dir_y + info->plane_y * info->camera_x;
		info->map_x = (int)info->pos_x;
		info->map_y = (int)info->pos_y;
		info->deltadist_x = fabs(1 / info->raydir_x);
		info->deltadist_y = fabs(1 / info->raydir_y);
		wall_calc_sidedist(info);
		find_hit_wall(info);
		wall_calc1(info);
		wall_calc2(info);
		fill_wall_color(info, x);
		x++;
	}
}
