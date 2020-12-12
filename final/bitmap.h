/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitmap.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 10:43:09 by jayun             #+#    #+#             */
/*   Updated: 2020/12/12 10:44:37 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITMAP_H

# define BITMAP_H

# pragma pack(push, 1)

typedef struct	s_bit
{
	unsigned short	ftype;
	unsigned int	fsize;
	unsigned short	freserved1;
	unsigned short	freserved2;
	unsigned int	foffbits;
	unsigned int	size;
	int				bit_width;
	int				bit_height;
	unsigned short	planes;
	unsigned short	bitcount;
	unsigned int	compression;
	unsigned int	sizeimage;
	int				x_pelspermeter;
	int				y_pelspermeter;
	unsigned int	colorused;
	unsigned int	colorimportant;
}				t_bit;

# pragma pack(pop)

#endif
