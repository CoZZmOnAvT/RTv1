/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/02 20:30:44 by pgritsen          #+#    #+#             */
/*   Updated: 2018/03/05 18:21:32 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static inline int	count_splited(char **arr)
{
	int	count;

	count = 0;
	while (arr[count])
		count++;
	return (count);
}

static void			scene_configuration(t_env *env, int fd)
{

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
	free_splited(tmp);
	ft_memdel((void **)&str);
	scene_configuration(env, fd);
}
