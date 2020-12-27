/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_info_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 09:51:58 by jayun             #+#    #+#             */
/*   Updated: 2020/12/28 01:26:52 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	info_empty_check(t_map *m)
{
	if (ft_strlen(m->north) == 0 || ft_strlen(m->south) == 0 ||
			ft_strlen(m->west) == 0 || ft_strlen(m->east) == 0)
		return (0);
	if (ft_strlen(m->size) == 0 || ft_strlen(m->floor) == 0 ||
			ft_strlen(m->ceil) == 0 || ft_strlen(m->item) == 0)
		return (0);
	return (1);
}

static int	xpmfile_check(t_map *m)
{
	char	*c;

	c = ft_strnstr(m->north, ".xpm", ft_strlen(m->north));
	if (c == 0 || ft_strlen(c) != 4)
		return (0);
	c = ft_strnstr(m->south, ".xpm", ft_strlen(m->south));
	if (c == 0 || ft_strlen(c) != 4)
		return (0);
	c = ft_strnstr(m->west, ".xpm", ft_strlen(m->west));
	if (c == 0 || ft_strlen(c) != 4)
		return (0);
	c = ft_strnstr(m->east, ".xpm", ft_strlen(m->east));
	if (c == 0 || ft_strlen(c) != 4)
		return (0);
	c = ft_strnstr(m->item, ".xpm", ft_strlen(m->item));
	if (c == 0 || ft_strlen(c) != 4)
		return (0);
	c = ft_strnstr(m->floor, ".xpm", ft_strlen(m->floor));
	if (m->floor_color == -1 && (c == 0 || ft_strlen(c) != 4))
		return (0);
	c = ft_strnstr(m->ceil, ".xpm", ft_strlen(m->ceil));
	if (m->ceil_color == -1 && (c == 0 || ft_strlen(c) != 4))
		return (0);
	return (1);
}

static int	texfile_open(char *path)
{
	int	fd;

	if ((fd = open(path, O_RDONLY)) < 0)
	{
		perror(path);
		write("Error\n", 6);
		return (0);
	}
	if (close(fd) < 0)
	{
		write(1, "close Error\n", 12);
		return (0);
	}
	return (1);
}

static int	texfile_open_check(t_map *m)
{
	if (tefile_open(m->north) == 0)
		return (0);
	if (texfile_open(m->south) == 0)
		return (0);
	if (texfile_open(m->west) == 0)
		return (0);
	if (texfile_open(m->east) == 0)
		return (0);
	if (texfile_open(m->item) == 0)
		return (0);
	if (m->floor_color == -1 && texfile_open(m->floor) == 0)
		return (0);
	if (m->floor_color == -1 && texfile_open(m->ceil) == 0)
		return (0);
	return (1);
}

int			info_check(t_map *m)
{
	if (floor_ceil_check(m, m->floor, 0) == 0)
	{
		write(1, "floor info Error\n", 17);
		cubfile_info_free(m, 0);
		return (0);
	}
	if (floor_ceil_check(m, m->ceil, 1) == 0)
	{
		write(1, "ceil info Error\n", 16);
		cubfile_info_free(m, 0);
		return (0);
	}
	if (info_empty_check(m) == 0 || xpmfile_check(m) == 0)
	{
		write(1, "tex info empty or not .xpm or wrong file Error\n", 47);
		cubfile_info_free(m, 0);
		return (0);
	}
	if (texfile_open_check(m) == 0)
		return (0);
	return (1);
}
