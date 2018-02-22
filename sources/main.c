/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/15 13:43:02 by pgritsen          #+#    #+#             */
/*   Updated: 2018/02/22 16:28:28 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "rtv1.h"

int		main(void)
{
	t_env			env;

	sgl_init();
	env.win = sgl_new_window(PROGRAM_NAME, 1024, 1024, SDL_WINDOW_OPENGL);
	while (1)
	{
		SDL_SetRenderDrawColor(env.win->rend, 0, 0, 0, 0);
		SDL_RenderClear(env.win->rend);

		if (poll_events(&env) == 0)
			return (sgl_quit());

		display_fps(env.win->rend);
		SDL_RenderPresent(env.win->rend);
		sgl_vsync();
	}
	return (sgl_quit());
}
