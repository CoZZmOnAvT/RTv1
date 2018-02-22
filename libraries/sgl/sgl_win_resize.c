/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sgl_win_resize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/22 15:11:53 by pgritsen          #+#    #+#             */
/*   Updated: 2018/02/22 15:56:16 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sgl_private.h"

void	sgl_win_resize(t_uint win_id, t_uint new_width, t_uint new_height)
{
	t_sgl_window	*win;

	win = sgl_get_window_by_id(win_id);
	win->w = new_width;
	win->h = new_height;
}
