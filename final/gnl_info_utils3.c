/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_info_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 10:02:23 by jayun             #+#    #+#             */
/*   Updated: 2020/12/12 10:03:23 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	get_color_n(char *line, int n, int i)
{
	while ((line[i] >= '0' && line[i] <= '9') || line[i] == 32)
		i++;
	if (line[i] != ',' || n > 255)
		return (-1);
	while (line[++i] == 32)
		;
	if (line[i] == ',' || line[i] == '\0')
		return (-1);
	return (i);
}

static int	cal_color(t_map *m, char *line, int type)
{
	int	n;
	int	color;
	int	i;

	color = 0;
	n = ft_atoi(line);
	color += n * 256 * 256;
	if ((i = get_color_n(line, n, 0)) == -1)
		return (0);
	n = ft_atoi(line + i);
	color += n * 256;
	if ((i = get_color_n(line, n, i)) == -1)
		return (0);
	n = ft_atoi(line + i);
	color += n;
	while (line[i] >= '0' && line[i] <= '9')
		i++;
	if (line[i] != '\0' || n > 255)
		return (0);
	if (type == 0)
		m->floor_color = color;
	else if (type == 1)
		m->ceil_color = color;
	return (1);
}

int			floor_ceil_check(t_map *m, char *line, int type)
{
	int	i;

	if (line[0] == ',' || line[ft_strlen(line) - 1] == ',')
		return (0);
	i = 0;
	while ((line[i] >= '0' && line[i] <= '9') ||
			line[i] == ',' || line[i] == 32)
		i++;
	if (line[i] == '\0')
	{
		if (cal_color(m, line, type) == 0)
			return (0);
	}
	else if (m->bonus_on == 0)
		return (0);
	return (1);
}
