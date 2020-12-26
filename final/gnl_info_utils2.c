/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_info_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 09:51:58 by jayun             #+#    #+#             */
/*   Updated: 2020/12/26 18:36:23 by jayun            ###   ########.fr       */
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

static int	directory_check(t_map *m)
{
	char	*c;

	c = ft_strnstr(m->north, "./textures", 10);
	if (c == 0)
		return (0);
	c = ft_strnstr(m->south, "./textures", 10);
	if (c == 0)
		return (0);
	c = ft_strnstr(m->west, "./textures", 10);
	if (c == 0)
		return (0);
	c = ft_strnstr(m->east, "./textures", 10);
	if (c == 0)
		return (0);
	c = ft_strnstr(m->item, "./textures", 10);
	if (c == 0)
		return (0);
	c = ft_strnstr(m->floor, "./textures", 10);
	if (m->floor_color == -1 && c == 0)
		return (0);
	c = ft_strnstr(m->ceil, "./textures", 10);
	if (m->ceil_color == -1 && c == 0)
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
	if (info_empty_check(m) == 0 || xpmfile_check(m) == 0 || directory_check(m) == 0)
	{
		write(1, "tex info are empty or not .xpm or wrong directory Error\n", 56);
		cubfile_info_free(m, 0);
		return (0);
	}
	return (1);
}
