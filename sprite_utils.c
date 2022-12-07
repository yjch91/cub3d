/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 08:28:22 by jayun             #+#    #+#             */
/*   Updated: 2020/12/12 09:06:28 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	get_sprite_count(t_map *m)
{
	int	i;
	int	j;

	m->sprite_count = 0;
	i = 0;
	while (i < m->map_x)
	{
		j = 0;
		while (j < m->map_y)
		{
			if (m->map[i][j] == 2)
				m->sprite_count++;
			else if (m->bonus_on == 1 &&
					(m->map[i][j] > 2 && m->map[i][j] <= 6))
				m->sprite_count++;
			j++;
		}
		i++;
	}
}

void	remove_sprite(t_info *info, int x, int y)
{
	int	i;

	i = 0;
	while (i < info->sprite_count)
	{
		if ((int)info->sprite[i].x == x && (int)info->sprite[i].y == y)
		{
			if (info->sprite[i].texture == 7)
				system("afplay ./sound/hit.wav &");
			else if (info->sprite[i].texture == 8)
				system("afplay ./sound/heal.wav &");
			else if (info->sprite[i].texture == 9)
				system("afplay ./sound/poison.wav &");
			info->sprite[i].texture = -1;
			break ;
		}
		i++;
	}
}

void	sort_sprite(t_info *info, int n)
{
	int		temp_o;
	double	temp_d;
	int		i;
	int		j;

	i = 0;
	while (i < n - 1)
	{
		j = 0;
		while (j < n - 1 - i)
		{
			if (info->sprite_dist[j] < info->sprite_dist[j + 1])
			{
				temp_d = info->sprite_dist[j];
				info->sprite_dist[j] = info->sprite_dist[j + 1];
				info->sprite_dist[j + 1] = temp_d;
				temp_o = info->sprite_order[j];
				info->sprite_order[j] = info->sprite_order[j + 1];
				info->sprite_order[j + 1] = temp_o;
			}
			j++;
		}
		i++;
	}
}
