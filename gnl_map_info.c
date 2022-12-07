/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_map_info.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 09:38:35 by jayun             #+#    #+#             */
/*   Updated: 2020/12/27 19:01:05 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	gnl_map_info_push_list(t_map *m, char *line)
{
	t_list *temp;

	if (g_map_empty == 1)
	{
		write(1, "map : emptyline Error\n", 22);
		cubfile_info_lst_free(m, 0);
		free(line);
		return (0);
	}
	g_map_empty = 0;
	if ((temp = ft_lstnew(line)) == 0)
	{
		write(1, "malloc allocation fail Error\n", 29);
		cubfile_info_lst_free(m, 0);
		free(line);
		return (0);
	}
	ft_lstadd_back(&m->lst, temp);
	return (1);
}

static int	space_check(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] != 32)
			return (0);
		i++;
	}
	return (1);
}

static int	map_line_check(t_map *m, char *c, int x)
{
	int	i;

	i = -1;
	while (c[++i] != '\0')
	{
		if (c[i] == 'N' || c[i] == 'S' || c[i] == 'E' || c[i] == 'W')
		{
			if (m->start_dir == 0)
			{
				m->start_dir = c[i];
				m->start_x = x;
				m->start_y = i;
				c[i] = '0';
			}
			else
				return (0);
		}
		else if (m->bonus_on == 0 &&
				!((c[i] >= '0' && c[i] <= '2') || c[i] == 32))
			return (0);
		else if (m->bonus_on == 1 &&
				!((c[i] >= '0' && c[i] <= '6') || c[i] == 32))
			return (0);
	}
	return (1);
}

static int	gnl_map_error_check(t_map *m, char *line)
{
	if ((space_check(line) == 1 || line[0] == '\0') && g_map_empty == 0)
		g_map_empty = 1;
	else if ((space_check(line) == 1 || line[0] == '\0') && g_map_empty != 0)
		;
	else if (line[0] != '\0')
	{
		write(1, line, ft_strlen(line));
		write(1, " : this line map Error\n", 23);
		cubfile_info_lst_free(m, 0);
		free(line);
		return (0);
	}
	free(line);
	return (1);
}

int			gnl_map_info(t_map *m, int n)
{
	char	*line;
	int		x;

	x = -1;
	while ((n = get_next_line(m->fd, &line)) > 0)
	{
		if (space_check(line) == 0 && map_line_check(m, line, ++x) == 1)
		{
			if (gnl_map_info_push_list(m, line) == 0)
				return (0);
		}
		else if (gnl_map_error_check(m, line) == 0)
			return (0);
	}
	if (n != -1 && m->start_dir == 0)
		write(1, "no starting direction Error\n", 28);
	if (n == -1 || m->start_dir == 0)
	{
		cubfile_info_lst_free(m, 0);
		if (n == -1)
			write(1, "get_next_line return value -1 Error\n", 36);
		return (0);
	}
	free(line);
	return (1);
}
