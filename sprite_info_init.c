/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_info_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 08:04:23 by jayun             #+#    #+#             */
/*   Updated: 2020/12/27 22:25:38 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	sprite_alloc_fail(t_info *info, t_map *m)
{
	all_free(info, m);
	write(1, "malloc allocation fail Error\n", 29);
	return (0);
}

int			sprite_alloc(t_info *info, t_map *m)
{
	info->sprite = 0;
	info->zbuffer = 0;
	info->sprite_order = 0;
	info->sprite_dist = 0;
	if ((info->sprite = (t_sprite *)malloc(
					sizeof(t_sprite) * info->sprite_count)) == 0)
		return (sprite_alloc_fail(info, m));
	if ((info->zbuffer = (double *)malloc(
					sizeof(double) * info->winsize_w)) == 0)
		return (sprite_alloc_fail(info, m));
	if ((info->sprite_order = (int *)malloc(
					sizeof(int) * info->sprite_count)) == 0)
		return (sprite_alloc_fail(info, m));
	if ((info->sprite_dist = (double *)malloc(
					sizeof(double) * info->sprite_count)) == 0)
		return (sprite_alloc_fail(info, m));
	return (1);
}

void		fill_sprite_info(t_info *info)
{
	int	i;
	int	j;
	int	snum;

	snum = 0;
	i = 0;
	while (i < info->map_h)
	{
		j = 0;
		while (j < info->map_w)
		{
			if (info->map[i][j] >= 2 && info->map[i][j] <= 6)
			{
				info->sprite[snum].x = i + 0.5;
				info->sprite[snum].y = j + 0.5;
				info->sprite[snum].texture = 4 + info->map[i][j];
				snum++;
			}
			j++;
		}
		i++;
	}
}
