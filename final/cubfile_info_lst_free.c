/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cubfile_info_lst_free.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 10:09:19 by jayun             #+#    #+#             */
/*   Updated: 2020/12/12 10:10:25 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	cubfile_info_free(t_map *m, int n)
{
	if (m->north != 0)
		free(m->north);
	if (m->south != 0)
		free(m->south);
	if (m->west != 0)
		free(m->west);
	if (m->east != 0)
		free(m->east);
	if (m->item != 0)
		free(m->item);
	if (m->floor != 0)
		free(m->floor);
	if (m->ceil != 0)
		free(m->ceil);
	if (m->size != 0)
		free(m->size);
	if (n == -1)
		write(1, "error : get_next_line return value -1\n", 38);
}

void	cubfile_info_lst_free(t_map *m, int n)
{
	cubfile_info_free(m, n);
	ft_lstclear(&m->lst);
}
