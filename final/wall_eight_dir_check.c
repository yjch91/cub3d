/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_eight_dir_check.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 09:09:51 by jayun             #+#    #+#             */
/*   Updated: 2020/12/12 09:15:24 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	eight_dir_check_init(int *i, int *k, int x, int y)
{
	*i = x - 1;
	*k = y - 1;
	if (*i < 0)
		*i = 0;
	if (*k < 0)
		*k = 0;
}

static int	eight_dir_check(t_map *m, int x, int y)
{
	int i;
	int j;
	int k;
	int	p;
	int	q;

	eight_dir_check_init(&i, &k, x, y);
	p = x + 1;
	q = y + 1;
	if (p > m->map_x - 1)
		p = m->map_x - 1;
	if (q > m->map_y - 1)
		q = m->map_y - 1;
	while (i <= p)
	{
		j = k;
		while (j <= q)
		{
			if (!(m->map[i][j] == 32 || m->map[i][j] == 1))
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int			wall_eight_dir_check(t_map *m)
{
	int	i;
	int	j;

	i = 0;
	while (i < m->map_x)
	{
		j = 0;
		while (j < m->map_y)
		{
			if (m->map[i][j] == 32)
			{
				if (eight_dir_check(m, i, j) == 0)
				{
					write(1, "wall check eight dir check error\n", 33);
					return (0);
				}
			}
			j++;
		}
		i++;
	}
	return (1);
}
