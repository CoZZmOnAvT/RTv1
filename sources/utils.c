/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/23 14:16:00 by pgritsen          #+#    #+#             */
/*   Updated: 2018/02/26 20:25:03 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	init_env(t_env *env)
{
	ft_bzero(env, sizeof(t_env));
	env->win = sgl_new_window(PROGRAM_NAME, W_WIDTH, W_HEIGHT,
								SDL_WINDOW_RESIZABLE);
	env->cam = ft_memalloc(sizeof(t_cam));
	env->cam->pos.z = 4;
	env->cam->rot.ry = 180;
	env->cam->vwp = ft_memalloc(sizeof(t_viewport));
	env->cam->vwp->w = 1.77;
	env->cam->vwp->h = 1.0;
	env->cam->vwp->dist = 1;
	env->smooth = 3;
	env->rf_depth = 4;
}
