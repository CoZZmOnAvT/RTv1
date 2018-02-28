/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/15 13:43:02 by pgritsen          #+#    #+#             */
/*   Updated: 2018/02/28 16:53:57 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "rtv1.h"

t_obj	objs[] = {
	// {0x8C0000, 1, {0, 0, 0}, {0, 1, 0},  1, 50, 0.4},
	{0x00008C, 0, {3, 0, 0},  {0, 0, 0}, 1, 20, 0.01},
	{0x00008C, 0, {-3, 0, 0},  {0, 0, 0}, 1, 20, 0.01},
	{0x00008C, 0, {0, 0, 3},  {0, 0, 0}, 1, 20, 0.01},
	{0x00008C, 0, {0, 0, -3},  {0, 0, 0}, 1, 20, 0.01},
	// {0x0000FF, 0, {2, 0, 0},  {0, 0, 0}, 0, 200, 0.01},
	// {0x00FF00, 0, {0, 1.73, 0}, {0, 0, 0}, 0, 1000, 0.01},
	// {0xC0C0C0, 3, {0, -1, 0}, {0, 1, 0}, 0, 10, 0.3},
	// {0xC0C0C0, 3, {0, 8, 0}, {0, -1, 0}, 0, 100, 0.3},
	// {0xFFFFFF, 3, {-8, 0, 0}, {-1, 0, 0}, 0, 10, 0.3},
	// {0xFFFFFF, 3, {8, 0, 0}, {1, 0, 0}, 0, 10, 0.3},
	// {0xFFFFFF, 3, {0, 0, 8}, {0, 0, 1}, 0, 10, 0.3},
	// {0xFFFFFF, 3, {0, 0, -8}, {0, 0, -1}, 0, 10, 0.3},
	{.type = -1}
};

t_light	light[] = {
	// {.type = 0, .intens = 1},
	{.type = 1, .intens = 1, .pos = {1, 6, 0}},
	// {.type = 2, .intens = 0.1, .dir = {4, 2, 0}},
	{.type = -1}
};

inline static void	camera_rotate(t_env *env)
{
	env->cam->rot.ry = (int)(env->cam->rot.ry + 1) % 360;
	env->cam->pos.x = 8 * sin(ft_degtorad(env->cam->rot.ry));
	env->cam->pos.z = -8 * cos(ft_degtorad(env->cam->rot.ry));
}

int					main(void)
{
	t_env			env;

	sgl_init();
	init_env(&env);
	cl_init(&env.cl, CL_DEVICE_TYPE_GPU);
	cl_parse_kernel(&env.cl, &env.cam->kl, KERNEL_FOLDER"render.cl", "render_scene");
	cl_reinit_mem(&env.cl, &env.cam->kl.mem, env.win->w * env.win->h * 4, 0);
	cl_reinit_mem(&env.cl, &env.objs, sizeof(objs), objs);
	cl_reinit_mem(&env.cl, &env.light, sizeof(light), light);
	while (1)
	{
		clEnqueueWriteBuffer(env.cl.queue, env.objs, CL_TRUE, 0, sizeof(objs), objs, 0, NULL, NULL);
		clEnqueueWriteBuffer(env.cl.queue, env.light, CL_TRUE, 0, sizeof(light), light, 0, NULL, NULL);
		if (poll_events(&env) == 0)
			return (sgl_quit());
		render_scene(&env);
		SDL_UpdateTexture(env.win->tex, NULL, env.win->surf->pixels, 1);
		SDL_RenderCopy(env.win->rend, env.win->tex, NULL, NULL);
		display_fps(env.win->rend);
		SDL_RenderPresent(env.win->rend);
		CAMERA_M(env.flags) ? camera_rotate(&env) : 0;
		sgl_vsync();
	}
	return (sgl_quit());
}
