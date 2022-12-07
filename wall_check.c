/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 09:07:29 by jayun             #+#    #+#             */
/*   Updated: 2020/12/26 17:56:35 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	wall_check_up(t_map *m)
{
	int	i;
	int	j;

	i = 0;
	while (i < m->map_y)
	{
		j = 0;
		while (j < m->map_x)
		{
			if (m->map[j][i] == 1)
				break ;
			else if (m->map[j][i] == 0 ||
					(m->map[j][i] >= 2 && m->map[j][i] <= 6))
			{
				write(1, "wall check up Error\n", 20);
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

static int	wall_check_down(t_map *m)
{
	int	i;
	int	j;

	i = 0;
	while (i < m->map_y)
	{
		j = m->map_x - 1;
		while (j >= 0)
		{
			if (m->map[j][i] == 1)
				break ;
			else if (m->map[j][i] == 0 ||
					(m->map[j][i] >= 2 && m->map[j][i] <= 6))
			{
				write(1, "wall check down Error\n", 22);
				return (0);
			}
			j--;
		}
		i++;
	}
	return (1);
}

static int	wall_check_left(t_map *m)
{
	int	i;
	int	j;

	i = 0;
	while (i < m->map_x)
	{
		j = 0;
		while (j < m->map_y)
		{
			if (m->map[i][j] == 1)
				break ;
			else if (m->map[i][j] == 0 ||
					(m->map[i][j] >= 2 && m->map[i][j] <= 6))
			{
				write(1, "wall check left Error\n", 22);
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

static int	wall_check_right(t_map *m)
{
	int	i;
	int	j;

	i = 0;
	while (i < m->map_x)
	{
		j = m->map_y - 1;
		while (j >= 0)
		{
			if (m->map[i][j] == 1)
				break ;
			else if (m->map[i][j] == 0 ||
					(m->map[i][j] >= 2 && m->map[i][j] <= 6))
			{
				write(1, "wall check right Error\n", 23);
				return (0);
			}
			j--;
		}
		i++;
	}
	return (1);
}

int			wall_check(t_map *m)
{
	int	n;

	n = 1;
	if (wall_check_up(m) == 0)
		n = 0;
	if (wall_check_down(m) == 0)
		n = 0;
	if (wall_check_left(m) == 0)
		n = 0;
	if (wall_check_right(m) == 0)
		n = 0;
	if (wall_eight_dir_check(m) == 0)
		n = 0;
	if (n == 0)
		map_array_free(m, m->map_x, 1);
	return (n);
}
