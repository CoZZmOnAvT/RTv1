/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/15 13:43:42 by pgritsen          #+#    #+#             */
/*   Updated: 2018/02/26 20:31:32 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include <stdio.h>
# include <time.h>
# include <errno.h>
# include <OpenCl/OpenCl.h>
# include "sgl.h"
# include "structures.h"

# define PROGRAM_NAME "RTv1"

# define KERNEL_FOLDER "./resources/kernels/"

# define W_WIDTH 1920
# define W_HEIGHT 1080

# define EXIT_KEY SDLK_ESCAPE

# define CAMERA_M(n)(n & 0b1)

typedef struct	s_cl_core
{
	cl_device_id		device;
	cl_context			context;
	cl_command_queue	queue;
}				t_cl_core;

typedef struct	s_cl_kl
{
	cl_program			program;
	cl_kernel			kernel;
	cl_mem				mem;
}				t_cl_kl;

typedef struct	s_cam
{
	t_point		pos;
	t_rotate	rot;
	t_cl_kl		kl;
	t_viewport	*vwp;
}				t_cam;

typedef struct	s_env
{
	t_cl_core		cl;
	t_sgl_window	*win;
	t_cam			*cam;
	cl_mem			objs;
	cl_mem			light;
	int				smooth;
	int				rf_depth;
	t_ulong			flags;
}				t_env;

extern t_obj	objs[];
extern t_light	light[];

/*
**				Draw.c
**				↓↓↓↓↓↓
*/

void			render_scene(t_env *env);

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

/*
**				Utils.c
**				↓↓↓↓↓↓↓
*/

void			init_env(t_env *env);

/*
**				OpenCl.c
**				↓↓↓↓↓↓↓↓
*/

void			cl_reinit_mem(t_cl_core *cl, cl_mem *mem, size_t size, void *ptr);

void			cl_init(t_cl_core *cl, cl_device_type dev_type);

void			cl_parse_kernel(t_cl_core *cl, t_cl_kl *kl,
						const char *kernel_name, const char *func_name);

/*
**				Sys_handler.c
**				↓↓↓↓↓↓↓↓↓↓↓↓↓
*/

void			ft_err_handler(const char *msg, const char *add,
								int err, t_uchar stop);

#endif
