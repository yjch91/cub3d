/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_calc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 08:54:32 by jayun             #+#    #+#             */
/*   Updated: 2020/12/12 10:56:24 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	sprite_info_sort(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->sprite_count)
	{
		info->sprite_order[i] = i;
		info->sprite_dist[i] = (info->pos_x - info->sprite[i].x) *
			(info->pos_x - info->sprite[i].x) +
			(info->pos_y - info->sprite[i].y) *
			(info->pos_y - info->sprite[i].y);
		i++;
	}
	sort_sprite(info, info->sprite_count);
}

static void	sprite_calc1(t_info *info, int i)
{
	info->sprite_x = info->sprite[info->sprite_order[i]].x -
		info->pos_x;
	info->sprite_y = info->sprite[info->sprite_order[i]].y -
		info->pos_y;
	info->invdet = 1.0 /
		(info->plane_x * info->dir_y - info->dir_x * info->plane_y);
	info->transform_x = info->invdet *
		(info->dir_y * info->sprite_x - info->dir_x * info->sprite_y);
	info->transform_y = info->invdet *
		(-info->plane_y * info->sprite_x + info->plane_x * info->sprite_y);
	info->spritescreen_x = (int)((info->winsize_w / 2) *
			(1 + info->transform_x / info->transform_y));
}

static void	sprite_calc2(t_info *info)
{
	info->sprite_h = fabs(info->winsize_h / info->transform_y);
	info->drawstart_y = -info->sprite_h / 2 + info->winsize_h / 2 +
		info->pitch + (info->jump / info->transform_y);
	if (info->drawstart_y < 0)
		info->drawstart_y = 0;
	info->drawend_y = info->sprite_h / 2 + info->winsize_h / 2 +
		info->pitch + (info->jump / info->transform_y);
	if (info->drawend_y >= info->winsize_h)
		info->drawend_y = info->winsize_h - 1;
	info->sprite_w = fabs(info->winsize_h / info->transform_y);
	info->drawstart_x = -info->sprite_w / 2 + info->spritescreen_x;
	if (info->drawstart_x < 0)
		info->drawstart_x = 0;
	info->drawend_x = info->sprite_w / 2 + info->spritescreen_x;
	if (info->drawend_x >= info->winsize_w)
		info->drawend_x = info->winsize_w - 1;
}

static void	fill_sprite_color(t_info *info, int i, int x, int texw)
{
	int	y;

	info->sh = sqrt((info->sprite_x * info->sprite_x) +
			(info->sprite_y * info->sprite_y));
	y = info->drawstart_y;
	while (y < info->drawend_y)
	{
		info->tex_y = ((int)(256 * (y - info->pitch -
						(info->jump / info->transform_y) - info->winsize_h / 2 +
						info->sprite_h / 2)) * texw / info->sprite_h) / 256;
		if (info->sprite[info->sprite_order[i]].texture == 6 ||
				info->sprite[info->sprite_order[i]].texture == 7)
			info->color = info->texture[info->sprite[info->sprite_order[i]].
				texture].data[texw * info->tex_y + info->tex_x];
		else
			info->color = info->texture[info->sprite[info->sprite_order[i]].
				texture].data[texw * info->tex_x + info->tex_y];
		if ((info->color & 0x00FFFFFF) != 0 && (info->color != 0x0000FF00) != 0)
		{
			if (info->flag_sky != 1 && info->bonus_on == 1)
				info->color = shade_color(info->sh, info->color);
			info->buf[y][x] = info->color;
		}
		y++;
	}
}

void		sprite_calc(t_info *info)
{
	int	x;
	int i;
	int	texw;

	sprite_info_sort(info);
	i = -1;
	while (++i < info->sprite_count)
	{
		if (info->sprite[info->sprite_order[i]].texture != -1)
		{
			sprite_calc1(info, i);
			sprite_calc2(info);
			texw = info->texture[info->sprite[info->sprite_order[i]].texture].
				img_width;
			x = info->drawstart_x - 1;
			while (++x < info->drawend_x)
			{
				info->tex_x = (int)(256 * (x - (-info->sprite_w / 2 + info->
								spritescreen_x)) * texw / info->sprite_w) / 256;
				if (info->transform_y > 0 && x > 0 && x < info->winsize_w &&
						info->transform_y < info->zbuffer[x])
					fill_sprite_color(info, i, x, texw);
			}
		}
	}
}
