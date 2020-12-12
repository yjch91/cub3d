/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_bitmap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 06:26:06 by jayun             #+#    #+#             */
/*   Updated: 2020/12/12 07:13:48 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	bitmap_setting(t_info *info)
{
	info->bit.ftype = 0x4d42;
	info->bit.fsize = 54 + 4 * info->winsize_w * info->winsize_h;
	info->bit.freserved1 = 0;
	info->bit.freserved2 = 0;
	info->bit.foffbits = 54;
	info->bit.size = 40;
	info->bit.bit_width = info->winsize_w;
	info->bit.bit_height = -info->winsize_h;
	info->bit.planes = 1;
	info->bit.bitcount = 32;
	info->bit.compression = 0;
	info->bit.sizeimage = 4 * info->winsize_w * info->winsize_h;
	info->bit.x_pelspermeter = info->winsize_w;
	info->bit.y_pelspermeter = info->winsize_h;
	info->bit.colorused = 0xFFFFFF;
	info->bit.colorimportant = 0;
}

void		save_bitmap(t_info *info)
{
	int	fd;

	bitmap_setting(info);
	fd = open("cub3D.bmp", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	write(fd, &info->bit, 54);
	write(fd, info->img.data, info->bit.sizeimage);
	close(fd);
	all_free(info, &info->m);
	exit(0);
}
