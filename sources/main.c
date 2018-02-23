/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/15 13:43:02 by pgritsen          #+#    #+#             */
/*   Updated: 2018/02/23 23:52:33 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "rtv1.h"

t_obj	objs[] = {
	{0xFF0000, {0, -1, 3}, 1.0},
	{0x0000FF, {2, 0, 4}, 1.0},
	{0x00FF00, {-2, 0, 4}, 1.0},
	{0xFFFF00, {0, -5001, 0}, 5000},
	{0, {0, 0, 0}, 0}
};

t_light	light[] = {
	{.type = 0, .intens = 0.2},
	{.type = 1, .intens = 0.6, .pos = {2, 1, 0}},
	{.type = 2, .intens = 0.2, .dir = {1, 4, 4}},
	{.type = -1}
};

int		main(void)
{
	t_env			env;

	sgl_init();
	init_env(&env);
	cl_init(&env.cl, CL_DEVICE_TYPE_GPU);
	cl_parse_kernel(&env.cl, &env.cam->kl, KERNEL_FOLDER"render.cl", "render_scene");
	cl_reinit_mem(&env.cl, &env.cam->kl, env.win->w * env.win->h * 4);
	env.objs = clCreateBuffer(env.cl.context, CL_MEM_USE_HOST_PTR, sizeof(*objs), objs, 0);
	env.light = clCreateBuffer(env.cl.context, CL_MEM_USE_HOST_PTR, sizeof(*light), light, 0);
	while (1)
	{
		SDL_SetRenderDrawColor(env.win->rend, 0, 0, 0, 0);
		SDL_RenderClear(env.win->rend);
		render_scene(&env);
		if (poll_events(&env) == 0)
			return (sgl_quit());
		SDL_UpdateTexture(env.win->tex, NULL, env.win->surf->pixels, 1);
		SDL_RenderCopy(env.win->rend, env.win->tex, NULL, NULL);
		display_fps(env.win->rend);
		SDL_RenderPresent(env.win->rend);
		sgl_vsync();
	}
	return (sgl_quit());
}
