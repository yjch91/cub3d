/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 10:24:35 by jayun             #+#    #+#             */
/*   Updated: 2020/12/28 03:51:00 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	map_init(t_map *m)
{
	m->north = 0;
	m->south = 0;
	m->west = 0;
	m->east = 0;
	m->size = 0;
	m->item = 0;
	m->floor = 0;
	m->floor_color = -1;
	m->ceil = 0;
	m->ceil_color = -1;
	m->w = 0;
	m->h = 0;
	m->start_dir = 0;
	m->lst = 0;
	m->bitmap_check = 0;
	g_map_empty = -1;
	g_save = 0;
}

static int	second_arg_check(t_map *m, char **argv)
{
	char	*c;

	c = ft_strnstr(argv[2], "--save", 6);
	if (c == 0 || ft_strlen(argv[2]) != 6)
	{
		write(1, "second argument is no \"--save\" Error\n", 37);
		close(m->fd);
		return (0);
	}
	m->bitmap_check = 1;
	return (1);
}

static int	cubfile_open(t_map *m, int argc, char **argv)
{
	char	*c;

	c = ft_strnstr(argv[1], ".cub", ft_strlen(argv[1]));
	if (c == 0 || ft_strlen(c) != 4)
	{
		write(1, "first argument is no .cub file Error\n", 37);
		return (0);
	}
	if ((m->fd = open(argv[1], O_RDONLY)) < 0)
	{
		perror(argv[1]);
		write(1, "Error\n", 6);
		return (0);
	}
	if (argc == 3)
	{
		if (second_arg_check(m, argv) == 0)
			return (0);
	}
	return (1);
}

static int	map_parsing(t_map *m)
{
	if (gnl_info(m, 0) == 0 || gnl_map_info(m, 0) == 0)
	{
		close(m->fd);
		return (0);
	}
	if (close(m->fd) < 0)
	{
		write(1, "close Error\n", 12);
		return (0);
	}
	if (fill_map_array(m) == 0 || wall_check(m) == 0)
		return (0);
	get_sprite_count(m);
	return (1);
}

int			main(int argc, char **argv)
{
	t_map	m;

	map_init(&m);
	m.bonus_on = 0;
	if (argc == 2 || argc == 3)
	{
		if (cubfile_open(&m, argc, argv) == 0)
			return (0);
		if (map_parsing(&m) == 0)
		{
			if (g_save != 0)
				free(g_save);
			g_save = 0;
			return (0);
		}
		if (g_save != 0)
			free(g_save);
		g_save = 0;
		cub_play(&m);
	}
	else
		write(1, "argument is no < program [.cub] ([--save]) > Error\n", 51);
	return (0);
}
