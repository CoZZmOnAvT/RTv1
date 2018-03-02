/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/02 20:30:44 by pgritsen          #+#    #+#             */
/*   Updated: 2018/03/02 21:00:08 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	read_scene(t_env *env, int ac, char **av)
{
	int		fd;

	(void)env;
	ac != 2 ? display_usage(0) : 0;
	!ft_strcmp("-help", av[1]) ? display_usage(1) : 0;
	(fd = open(av[1], O_RDONLY)) == -1 ? ft_err_handler(av[1], 0, errno, 1) : 0;
}
