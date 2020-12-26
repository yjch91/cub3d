/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_info.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 09:42:59 by jayun             #+#    #+#             */
/*   Updated: 2020/12/26 17:49:58 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	entering_info(char **info, char *data)
{
	*info = data;
	if (*info == 0)
	{
		write(1, "malloc allocation fail Error\n", 29);
		return (-1);
	}
	return (1);
}

static int	gnl_info2(t_map *m, char *line)
{
	int	n;

	if (ft_strnstr(line, "NO ", 3) == line && m->north == 0)
		n = entering_info(&m->north, ft_strtrim(line + 2, " "));
	else if (ft_strnstr(line, "SO ", 3) == line && m->south == 0)
		n = entering_info(&m->south, ft_strtrim(line + 2, " "));
	else if (ft_strnstr(line, "WE ", 3) == line && m->west == 0)
		n = entering_info(&m->west, ft_strtrim(line + 2, " "));
	else if (ft_strnstr(line, "EA ", 3) == line && m->east == 0)
		n = entering_info(&m->east, ft_strtrim(line + 2, " "));
	else if (ft_strnstr(line, "S ", 2) == line && m->item == 0)
		n = entering_info(&m->item, ft_strtrim(line + 1, " "));
	else if (ft_strnstr(line, "F ", 2) == line && m->floor == 0)
		n = entering_info(&m->floor, ft_strtrim(line + 1, " "));
	else if (ft_strnstr(line, "C ", 2) == line && m->ceil == 0)
		n = entering_info(&m->ceil, ft_strtrim(line + 1, " "));
	else if (ft_strnstr(line, "R ", 2) == line && m->size == 0)
		n = entering_info(&m->size, ft_strtrim(line + 1, " "));
	else
		return (0);
	if (n == -1)
		cubfile_info_free(m, 0);
	free(line);
	return (n);
}

int			gnl_info(t_map *m, int n)
{
	char	*line;
	int		r;

	while (info_null_check(m) == 0 && (n = get_next_line(m->fd, &line)) > 0)
	{
		if ((r = gnl_info2(m, line)) == 0)
		{
			if (line[0] != '\0')
			{
				write(1, line, ft_strlen(line));
				write(1, " : this line Error\n", 19);
				cubfile_info_free(m, 0);
				r = -1;
			}
			free(line);
		}
		if (r == -1)
			return (0);
	}
	if (n == -1)
		cubfile_info_free(m, -1);
	if (n == -1 || info_full_check(m) == 0 ||
			info_check(m) == 0 || resolution_check(m) == 0)
		return (0);
	return (1);
}
