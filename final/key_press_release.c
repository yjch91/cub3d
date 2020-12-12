/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_press_release.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jayun <jayun@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 06:50:02 by jayun             #+#    #+#             */
/*   Updated: 2020/12/12 07:12:37 by jayun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int			key_release(int key, t_info *info)
{
	if (key == K_W)
		info->flag_w = 0;
	if (key == K_S)
		info->flag_s = 0;
	if (key == K_D)
		info->flag_d = 0;
	if (key == K_A)
		info->flag_a = 0;
	if (key == K_C && info->flag_c == 1)
	{
		info->flag_c = 0;
		info->movespeed *= 3;
	}
	if (key == K_V)
		info->flag_shot = 0;
	return (0);
}

static void	key_press_other(int key, t_info *info)
{
	if (key == K_T)
	{
		info->pitch += 400 * info->movespeed;
		if (info->pitch > 400)
			info->pitch = 400;
	}
	if (key == K_G)
	{
		info->pitch -= 400 * info->movespeed;
		if (info->pitch < -400)
			info->pitch = -400;
	}
}

static void	key_press_other2(int key, t_info *info)
{
	if (key == K_SP && info->flag_sp == 0)
	{
		info->jump = info->winsize_h / 2;
		info->flag_sp = 1;
	}
	if (key == K_C && info->flag_c == 0)
	{
		info->jump = -info->winsize_h / 3;
		info->flag_c = 1;
		info->movespeed /= 3;
	}
	if (key == K_V && info->flag_weap == 1 && info->flag_shot == 0)
	{
		info->flag_shot = 1;
		weap_shot(info);
		system("afplay ./sound/attack.wav &");
	}
}

int			key_press(int key, t_info *info)
{
	if (key == K_W)
		info->flag_w = 1;
	else if (key == K_S)
		info->flag_s = 1;
	else if (key == K_D)
		info->flag_d = 1;
	else if (key == K_A)
		info->flag_a = 1;
	else if (key == K_ESC)
	{
		all_free(info, &info->m);
		exit(0);
	}
	if (info->bonus_on == 1)
	{
		key_press_other(key, info);
		key_press_other2(key, info);
	}
	return (0);
}

int			button_redcross(t_info *info)
{
	all_free(info, &info->m);
	exit(0);
}
