/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 14:01:53 by pgritsen          #+#    #+#             */
/*   Updated: 2018/02/22 15:55:43 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		poll_events(t_env *env)
{
	SDL_Window	*win;
	static char		full_screen;
	SDL_Event	e;

	(void)env;
	while (SDL_PollEvent(&e))
		if (e.window.event == SDL_WINDOWEVENT_CLOSE
			|| (e.type == SDL_KEYDOWN && e.key.keysym.sym == EXIT_KEY))
			return (0);
		else if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_TAB)
		{
			win = sgl_get_window_by_id(e.window.windowID)->p;
			full_screen && !(full_screen = 0) ? SDL_SetWindowFullscreen(win, 0)
			: (full_screen = 1) &&
				SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
		}
		else if (e.window.event == SDL_WINDOWEVENT_RESIZED)
			sgl_win_resize(e.window.windowID, e.window.data1, e.window.data2);
	return (1);
}
