/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/15 13:43:02 by pgritsen          #+#    #+#             */
/*   Updated: 2018/03/02 16:43:58 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "rtv1.h"

t_obj	objs[] = {
	{0xFF0000, 0, {1.4, 0, .86},  {0, 0, 0}, 1, 8, 0.1},
	{0x00FF00, 0, {-.5, 0, .86},  {0, 0, 0}, 1, 8, 0.1},
	{0xFFFF00, 0, {.5, 0, -.86},  {0, 0, 0}, 1, 8, 0.1},
	{0x00FFFF, 0, {.5, 1.6, .3},  {0, 0, 0}, 1, 8, 0.1},
	{0x00FF00, 1, {-2.5, -1, 2.86}, {0, 8, 0}, 1, 8, 0.1},
	{0xFFFF00, 1, {2.5, -1, -2.86}, {0, 8, 0}, 1, 8, 0.1},
	{0x00FFFF, 1, {3.5, -1, 3.3}, {0, 8, 0}, 1, 8, 0.1},
	{0xC0C0C0, 3, {0, -1.4, 0}, {0, 1, 0}, 0, 2, 0},
	{0xC0C0C0, 3, {0, 8, 0}, {0, -1, 0}, 0, 2, 0},
	{0xC0C0C0, 3, {9, 0, 0}, {-1, 0, 0}, 0, 2, 0},
	{0xC0C0C0, 3, {-9, 0, 0}, {1, 0, 0}, 0, 2, 0},
	{0xC0C0C0, 3, {0, 0, 9}, {0, 0, -1}, 0, 2, 0},
	{0xC0C0C0, 3, {0, 0, -9}, {0, 0, 1}, 0, 2, 0},
	{.type = -1}
};

t_light	light[] = {
	{.type = 0, .intens = 0.1},
	{.type = 1, .intens = 0.9, .pos = {5, 6, 5}},
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
