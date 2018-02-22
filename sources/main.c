/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/15 13:43:02 by pgritsen          #+#    #+#             */
/*   Updated: 2018/02/22 15:00:44 by pgritsen         ###   ########.fr       */
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
		sgl_draw_line((t_point){1013, 1005, 0}, (t_point){38, 13, 0}, 0xFF8C, env.win->tex);
		if (poll_events(&env) == 0)
			return (sgl_quit());

		SDL_RenderCopy(env.win->rend, env.win->tex, NULL, NULL);
		display_fps(env.win->rend);
		SDL_RenderPresent(env.win->rend);
		sgl_vsync();
	}
	return (sgl_quit());
}
