/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/02 20:30:44 by pgritsen          #+#    #+#             */
/*   Updated: 2018/03/16 18:11:45 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void			read_object(t_env *env, char *data, int index)
{
	char	**arr;

	!data ? ft_err_handler("Object broken!", 0, 0, 1) : 0;
	if (count_splited((arr = ft_strsplit(data, ','))) != 7)
		ft_err_handler("Object broken!", 0, 0, 1);
	ft_memdel((void **)&data);
	env->scene.objs_h[index].type = ABS(ft_atoi(arr[0]));
	env->scene.objs_h[index].color = ft_atol_base(arr[1], 16);
	env->scene.objs_h[index].pos = sgl_atop(arr[2]);
	env->scene.objs_h[index].dir = sgl_atop(arr[3]);
	if (!sgl_check_point(env->scene.objs_h[index].pos)
		|| !sgl_check_point(env->scene.objs_h[index].dir))
		ft_err_handler("Object broken!", 0, 0, 1);
	env->scene.objs_h[index].rad = ABS(ft_atof(arr[4]));
	env->scene.objs_h[index].spec = ABS(ft_atoi(arr[5]));
	env->scene.objs_h[index].refl = ABS(ft_atof(arr[6]));
	free_splited(arr);
}

static void			read_light(t_env *env, char *data, int index)
{
	char	**arr;

	!data ? ft_err_handler("Light broken!", 0, 0, 1) : 0;
	if (count_splited((arr = ft_strsplit(data, ','))) != 3)
		ft_err_handler("Light broken!", 0, 0, 1);
	ft_memdel((void **)&data);
	env->scene.light_h[index].type = ABS(ft_atoi(arr[0]));
	env->scene.light_h[index].intens = ABS(ft_atof(arr[1]));
	env->scene.light_h[index].pos = sgl_atop(arr[2]);
	if (!sgl_check_point(env->scene.light_h[index].pos))
		ft_err_handler("Light broken!", 0, 0, 1);
	free_splited(arr);
}

static void			procced_data(t_env *env, int fd)
{
	char	*str;
	t_uint	l_c;
	t_uint	o_c;

	l_c = 0;
	o_c = 0;
	while (ft_get_next_line(fd, &str) > 0)
	{
		if (!ft_strncmp(str, "object", 6) && ++o_c <= env->scene.objs_c)
			read_object(env, ft_get_content(str, '[', ']'), o_c - 1);
		else if (!ft_strncmp(str, "light", 5) && ++l_c <= env->scene.light_c)
			read_light(env, ft_get_content(str, '[', ']'), l_c - 1);
		if (o_c > env->scene.objs_c || l_c > env->scene.light_c)
			ft_err_handler("Scene broken!", 0, 0, 1);
		ft_memdel((void **)&str);
	}
	ft_memdel((void **)&str);
	if (o_c != env->scene.objs_c || l_c != env->scene.light_c)
		ft_err_handler("Scene broken!", 0, 0, 1);
	cl_reinit_mem(&env->cl, &env->scene.objs,
		sizeof(t_obj) * (env->scene.objs_c + 1), env->scene.objs_h);
	cl_reinit_mem(&env->cl, &env->scene.light,
		sizeof(t_light) * (env->scene.light_c + 1), env->scene.light_h);
}

static void			scene_configuration(t_env *env, int fd)
{
	char	*str;
	char	*tmp;
	char	**arr;
	t_point	p_tmp;

	if (ft_get_next_line(fd, &str) <= 0)
		ft_err_handler("Scene broken!", 0, 0, 1);
	if (!(tmp = ft_get_content(str, '[', ']')))
		ft_err_handler("Scene broken!", 0, 0, 1);
	ft_memdel((void **)&str);
	if (count_splited((arr = ft_strsplit(tmp, ','))) != 3)
		ft_err_handler("Scene broken!", 0, 0, 1);
	ft_memdel((void **)&tmp);
	env->cam->rot_os = ABS(ft_atoi(arr[2]));
	env->cam->pos = sgl_atop(arr[0]);
	p_tmp = sgl_atop(arr[1]);
	if (!sgl_check_point(env->cam->pos) || !sgl_check_point(p_tmp))
		ft_err_handler("Scene broken!", 0, 0, 1);
	env->cam->rot = *(t_rotate *)&p_tmp;
	free_splited(arr);
}

void				read_scene(t_env *env, int ac, char **av)
{
	int		fd;
	char	*str;
	char	**tmp;

	ac != 2 ? display_usage(0) : 0;
	!ft_strcmp("-help", av[1]) ? display_usage(1) : 0;
	(fd = open(av[1], O_RDONLY)) == -1 ? ft_err_handler(av[1], 0, errno, 1) : 0;
	if (ft_get_next_line(fd, &str) <= 0)
		ft_err_handler("Scene broken!", 0, 0, 1);
	if (count_splited((tmp = ft_strsplit(str, ' '))) != 2)
		ft_err_handler("Scene broken!", 0, 0, 1);
	env->scene.objs_c = ABS(ft_atoi(tmp[0]));
	env->scene.light_c = ABS(ft_atoi(tmp[1]));
	!(env->scene.objs_h = malloc(sizeof(t_obj) * (env->scene.objs_c + 1)))
		? ft_err_handler("malloc", "can't allocate region!", 0, 1) : 0;
	!(env->scene.light_h = malloc(sizeof(t_light) * (env->scene.light_c + 1)))
		? ft_err_handler("malloc", "can't allocate region!", 0, 1) : 0;
	env->scene.objs_h[env->scene.objs_c].type = -1;
	env->scene.light_h[env->scene.light_c].type = -1;
	free_splited(tmp);
	ft_memdel((void **)&str);
	scene_configuration(env, fd);
	procced_data(env, fd);
}
