/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 08:10:59 by jayun             #+#    #+#             */
/*   Updated: 2020/12/26 04:31:32 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	buf_init(t_info *info)
{
	int	i;
	int	j;

	i = 0;
	while (i < info->winsize_h)
	{
		j = 0;
		while (j < info->winsize_w)
		{
			info->buf[i][j] = 0;
			j++;
		}
		i++;
	}
}

int			buf_alloc_init(t_info *info, t_map *m)
{
	int	i;
	int	j;

	if ((info->buf = (int **)malloc(sizeof(int *) * info->winsize_h)) == 0)
	{
		map_array_free(m, m->map_x, 0);
		free(info->mlx);
		return (0);
	}
	i = -1;
	while (++i < info->winsize_h)
	{
		if ((info->buf[i] = (int *)malloc(sizeof(int) * info->winsize_w)) == 0)
		{
			map_array_free(m, m->map_x, 0);
			j = -1;
			while (++j < i)
				free(info->buf[j]);
			free(info->buf);
			free(info->mlx);
			return (0);
		}
	}
	buf_init(info);
	return (1);
}
