/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_map_array.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 09:17:26 by jayun             #+#    #+#             */
/*   Updated: 2020/12/26 17:49:05 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	get_map_size(t_map *m)
{
	t_list	*temp;

	temp = m->lst;
	m->map_x = ft_lstsize(m->lst);
	m->map_y = 0;
	while (temp)
	{
		if (ft_strlen(temp->content) > m->map_y)
			m->map_y = ft_strlen(temp->content);
		temp = temp->next;
	}
}

static int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

void		map_array_free(t_map *m, int n, int type)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free(m->map[i]);
		i++;
	}
	free(m->map);
	cubfile_info_lst_free(m, 0);
	if (type == 0)
		write(1, "malloc allocation fail Error\n", 29);
}

static int	fill_map_array2(t_map *m)
{
	t_list	*temp;
	char	*s;
	int		i;
	int		j;

	temp = m->lst;
	i = -1;
	while (++i < m->map_x)
	{
		if ((m->map[i] = (int *)malloc(sizeof(int) * m->map_y)) == 0)
			map_array_free(m, i, 0);
		if (m->map[i] == 0)
			return (0);
		j = -1;
		s = temp->content;
		while (++j < m->map_y)
		{
			if (j < ft_strlen(s) && ft_isdigit(s[j]) == 1)
				m->map[i][j] = s[j] - '0';
			else
				m->map[i][j] = 32;
		}
		temp = temp->next;
	}
	return (1);
}

int			fill_map_array(t_map *m)
{
	get_map_size(m);
	if ((m->map = (int **)malloc(sizeof(int *) * m->map_x)) == 0)
	{
		cubfile_info_lst_free(m, 0);
		write(1, "malloc allocation fail Error\n", 29);
		return (0);
	}
	if (fill_map_array2(m) == 0)
		return (0);
	return (1);
}
