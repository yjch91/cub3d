/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 08:34:13 by jayun             #+#    #+#             */
/*   Updated: 2020/12/27 20:29:13 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int			get_point_size(t_info *info)
{
	int	p;
	int	q;

	p = info->winsize_h;
	if (p > info->winsize_w)
		p = info->winsize_w;
	q = info->map_h;
	if (q < info->map_w)
		q = info->map_w;
	p = p / q;
	return (p);
}

void		draw_point(t_info *info, int size)
{
	double	x;
	double	y;
	int		i;
	int		j;

	y = info->pos_x * size;
	x = info->pos_y * size;
	i = y - (size / 2);
	while (i < y + (size / 2))
	{
		j = x - (size / 2);
		while (j < x + (size / 2))
		{
			info->img2.data[i * (info->img2.size_l / 4) + j] = 0xAA0000FF;
			j++;
		}
		i++;
	}
}

static void	draw_rectangle(t_info *info, int x, int y, int color)
{
	int i;
	int j;
	int	p;

	p = get_point_size(info);
	x *= p;
	y *= p;
	i = 0;
	while (i < p)
	{
		j = 0;
		while (j < p)
		{
			info->img2.data[(y + i) * (info->img2.size_l / 4) + x + j] = color;
			j++;
		}
		i++;
	}
}

static void	map_lifebar_img_init(t_info *info)
{
	int	i;
	int	j;

	i = 0;
	while (i < info->winsize_h)
	{
		j = 0;
		while (j < info->winsize_w)
		{
			info->img2.data[i * (info->img2.size_l / 4) + j] = 0xFFFFFFFF;
			j++;
		}
		i++;
	}
}

void		draw_rectangles(t_info *info)
{
	int	i;
	int	j;
	int	color;

	map_lifebar_img_init(info);
	i = 0;
	while (i < info->map_h)
	{
		j = 0;
		while (j < info->map_w)
		{
			if (info->map[i][j] == 1)
				color = 0xAAFFFFFF;
			else if (info->map[i][j] >= 2 && info->map[i][j] <= 6)
				color = 0xAAFF0000;
			else
				color = 0xAA000000;
			draw_rectangle(info, j, i, color);
			j++;
		}
		i++;
	}
}
