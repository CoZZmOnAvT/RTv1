/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 14:01:53 by pgritsen          #+#    #+#             */
/*   Updated: 2018/02/27 21:42:49 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		poll_events(t_env *env)
{
	SDL_Event	e;

	while (SDL_PollEvent(&e))
		if (e.window.event == SDL_WINDOWEVENT_CLOSE
			|| (e.type == SDL_KEYDOWN && e.key.keysym.sym == EXIT_KEY))
			return (0);
		else if (e.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			sgl_win_resize(e.window.windowID, e.window.data1, e.window.data2);
			cl_reinit_mem(&env->cl, &env->cam->kl.mem,
							e.window.data1 * e.window.data2 * 4, 0);
		}
		else if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_P)
			env->flags ^= 0b1;
	return (1);
}
