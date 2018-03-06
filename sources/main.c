/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/15 13:43:02 by pgritsen          #+#    #+#             */
/*   Updated: 2018/03/06 15:36:12 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "rtv1.h"

t_obj	objs[] = {
	// {0xFF0000, 0, {1.4, 0, .86},  {0, 0, 0}, 1, 8, 0.1},
	// {0x00FF00, 0, {-.5, 0, .86},  {0, 0, 0}, 1, 8, 0.1},
	// {0xFFFF00, 0, {.5, 0, -.86},  {0, 0, 0}, 1, 8, 0.1},
	{0x00FFFF, 0, {0, 1.6, 0},  {0, 0, 0}, 1, 8, 0.1},
	// {0x00FF00, 1, {-2.5, -1, 2.86}, {0, 8, 0}, 1, 8, 0.1},
	// {0xFFFF00, 1, {2.5, -1, -2.86}, {0, 8, 0}, 1, 8, 0.1},
	// {0x00FFFF, 1, {3.5, -1, 3.3}, {0, 8, 0}, 1, 8, 0.1},
	{0x00FFFF, 1, {2, 0, 2}, {2, 4, 2}, 1, 8, 0.1},
	{0x8C0000, 2, {4, 0, 0}, {4, 4, 0}, 25, 20, 0.3},
	{0xC0C0C0, 3, {0, -1.4, 0}, {0, 1, 0}, 20, 2, 0.5},
	{0xC0C0C0, 3, {0, 8, 0}, {0, -1, 0}, 0, 2, 0.5},
	{0xC0C0C0, 3, {9, 0, 0}, {-1, 0, 0}, 0, 2, 0},
	{0xC0C0C0, 3, {-9, 0, 0}, {1, 0, 0}, 0, 2, 0.5},
	{0xC0C0C0, 3, {0, 0, 9}, {0, 0, -1}, 0, 2, 0},
	{0xC0C0C0, 3, {0, 0, -9}, {0, 0, 1}, 0, 2, 0.5},
	{.type = -1}
};

t_light	light[] = {
	{.type = 0, .intens = 0.1},
	{.type = 1, .intens = 0.9, .pos = {-5, 6, -5}},
	{.type = -1}
};

int					main(int ac, char **av)
{
	t_env			env;

	sgl_init();
	init_env(&env);
	read_scene(&env, ac, av);
	cl_reinit_mem(&env.cl, &env.scene.objs, sizeof(objs), objs);
	cl_reinit_mem(&env.cl, &env.scene.light, sizeof(light), light);
	render_scene(&env);
	while (1)
	{
		if (poll_events(&env) == 0)
			return (sgl_quit());
		SDL_UpdateTexture(env.win->tex, NULL, env.win->surf->pixels, 1);
		SDL_RenderCopy(env.win->rend, env.win->tex, NULL, NULL);
		display_fps(env.win->rend);
		SDL_RenderPresent(env.win->rend);
		CAMERA_M(env.flags) ? render_scene(&env) : 0;
		CAMERA_M(env.flags) ? camera_rotate(&env) : 0;
		sgl_vsync();
	}
	return (sgl_quit());
}
