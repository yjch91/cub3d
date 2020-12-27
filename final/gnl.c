/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 03:23:59 by jayun             #+#    #+#             */
/*   Updated: 2020/12/27 18:59:23 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <string.h>

static	int	find_save(char **line)
{
	int		i;

	i = 0;
	if ((g_save) != 0)
	{
		while ((g_save)[i] != '\0' && (g_save)[i] != '\n')
			i++;
		if ((g_save)[i] != '\n')
			return (0);
		g_save[i] = '\0';
		(*line) = ft_strdup(g_save);
		free(g_save);
		g_save = 0;
		return (1);
	}
	return (0);
}

static int	save_plus_buf(char *buf)
{
	char	*temp;

	if (g_save != 0)
	{
		temp = ft_strjoin(g_save, buf);
		if (temp == 0)
			return (-1);
		free(g_save);
		g_save = temp;
	}
	else
	{
		if ((g_save = ft_strdup(buf)) == 0)
			return (-1);
	}
	return (0);
}

static int	read_line(int fd, char **line)
{
	int		n;
	int		m;
	char	buf[2];

	while ((n = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[n] = '\0';
		if ((m = save_plus_buf(buf)) == -1)
			return (-1);
		if ((m = find_save(&(*line))) != 0)
			return (m);
	}
	return (n);
}

static int	rest_clear(char **line)
{
	if (g_save != 0)
	{
		if ((*line = ft_strdup(g_save)) == 0)
		{
			free(g_save);
			g_save = NULL;
			return (-1);
		}
	}
	else if (g_save == 0)
		(*line) = 0;
	return (0);
}

int			get_next_line(int fd, char **line)
{
	int			n;

	if (fd < 0 || BUFFER_SIZE <= 0 || line == 0)
		return (-1);
	if (g_save != 0)
	{
		if ((n = find_save(&(*line))) != 0)
			return (n);
	}
	n = read_line(fd, &(*line));
	if (n == 1)
		return (1);
	if (n == 0)
		n = rest_clear(&(*line));
	if (g_save != 0)
		free(g_save);
	g_save = NULL;
	return (n);
}
