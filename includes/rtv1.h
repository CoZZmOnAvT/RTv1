/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/15 13:43:42 by pgritsen          #+#    #+#             */
/*   Updated: 2018/02/22 14:05:31 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include <stdio.h>
# include <time.h>
# include "sgl.h"

# define PROGRAM_NAME "RTv1"

# define EXIT_KEY SDLK_ESCAPE

typedef struct	s_cam
{
	t_point		pos;
	t_rotate	rot;
	SDL_Surface	*surf;
}				t_cam;

typedef struct	s_env
{
	t_sgl_window	*win;
	t_cam			*cam;
}				t_env;

/*
**				Events.c
**				↓↓↓↓↓↓↓↓
*/

int				poll_events(t_env *env);

/*
**				Events.c
**				↓↓↓↓↓↓↓↓
*/

void			display_fps(SDL_Renderer *rend);

#endif
