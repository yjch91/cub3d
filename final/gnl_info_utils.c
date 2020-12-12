/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_info_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 09:45:00 by jayun             #+#    #+#             */
/*   Updated: 2020/12/12 10:11:15 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		info_null_check(t_map *m)
{
	if (m->north == 0 || m->south == 0 || m->west == 0 || m->east == 0)
		return (0);
	if (m->size == 0 || m->floor == 0 || m->ceil == 0 || m->item == 0)
		return (0);
	return (1);
}

int		resolution_check(t_map *m)
{
	int	i;

	m->w = ft_atoi(m->size);
	i = 0;
	while (m->size[i] >= '0' && m->size[i] <= '9')
		i++;
	m->h = ft_atoi(m->size + i);
	while (m->size[i] == 32)
		i++;
	while (m->size[i] >= '0' && m->size[i] <= '9')
		i++;
	if (m->size[i] != '\0' || m->w == 0 || m->h == 0)
	{
		write(1, "resolution error\n", 17);
		cubfile_info_free(m, 0);
		return (0);
	}
	return (1);
}

int		info_full_check(t_map *m)
{
	if (info_null_check(m) == 0)
	{
		cubfile_info_free(m, 0);
		write(1, "cub file infomation error\n", 26);
		return (0);
	}
	return (1);
}
