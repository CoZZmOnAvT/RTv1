/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/23 14:32:50 by pgritsen          #+#    #+#             */
/*   Updated: 2018/02/23 23:06:54 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

// double	vec_length(t_point vec)
// {
// 	return (sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));
// }

// t_point	intersect_ray_sphere(t_point cam_p, t_point c_p, t_obj *obj)
// {
// 	double	d;
// 	double	k1;
// 	double	k2;
// 	double	k3;
// 	t_point	oc;

// 	oc = (t_point){cam_p.x - obj->pos.x, cam_p.y - obj->pos.y, cam_p.z - obj->pos.z};

// 	k1 = c_p.x * c_p.x + c_p.y * c_p.y + c_p.z * c_p.z;
// 	k2 = 2.0 * (oc.x * c_p.x + oc.y * c_p.y + oc.z * c_p.z);
// 	k3 = oc.x * oc.x + oc.y * oc.y + oc.z * oc.z - obj->rad * obj->rad;

// 	d = k2 * k2 - 4.0 * k1 * k3;
// 	if (d < 0)
// 		return ((t_point){INFINITY, INFINITY, 0});

// 	return ((t_point){(-k2 + sqrt(d)) / (2.0 * k1), (-k2 - sqrt(d)) / (2.0 * k1), 0});
// }

// double	compute_lighting(t_point p, t_point n)
// {
// 	t_point	l;
// 	double	coef;
// 	double	n_dot_l;
// 	int		it;

// 	coef = 0.0;
// 	it = -1;
// 	while (light[++it])
// 		if (light[it]->type == 0)
// 			coef += light[it]->intens;
// 		else
// 		{
// 			if (light[it]->type == 1)
// 				l = (t_point){light[it]->pos.x - p.x, light[it]->pos.y - p.y, light[it]->pos.z - p.z};
// 			else
// 				l = light[it]->dir;
// 			n_dot_l = n.x * l.x + n.y * l.y + n.z * l.z;
// 			if (n_dot_l > 0)
// 				coef += light[it]->intens * n_dot_l / (vec_length(n) * vec_length(l));
// 		}
// 	return (coef);
// }

// t_uint	trace_ray(t_point cam_p, t_point c_p, double min, double max)
// {
// 	t_point	t;
// 	double	closest_t;
// 	t_obj	*closest_obj;
// 	int		it;

// 	closest_t = INFINITY;
// 	closest_obj = NULL;
// 	it = -1;
// 	while (spheres[++it])
// 	{
// 		t = intersect_ray_sphere(cam_p, c_p, spheres[it]);
// 		if (t.x >= min && t.x <= max && t.x < closest_t)
// 		{
// 			closest_t = t.x;
// 			closest_obj = spheres[it];
// 		}
// 		if (t.y >= min && t.y <= max && t.y < closest_t)
// 		{
// 			closest_t = t.y;
// 			closest_obj = spheres[it];
// 		}
// 	}
// 	if (closest_obj == NULL)
// 		return (0x000000);

// 	t_point	p;
// 	t_point	n;

// 	p = (t_point){cam_p.x + c_p.x * closest_t, cam_p.y + c_p.y * closest_t, cam_p.z + c_p.z * closest_t};

// 	n = (t_point){p.x - closest_obj->pos.x, p.y - closest_obj->pos.y, p.z - closest_obj->pos.z};

// 	n = (t_point){n.x / vec_length(n), n.y / vec_length(n), n.z / vec_length(n)};

// 	double	r = closest_obj->color >> 16 & 0xFF;
// 	double	g = closest_obj->color >> 8 & 0xFF;
// 	double	b = closest_obj->color & 0xFF;

// 	double	light_coef = compute_lighting(p, n);

// 	return ((ROUND(r * light_coef) * 0x10000) + (ROUND(g * light_coef) * 0x100) + ROUND(b * light_coef));
// }

void	render_scene(t_env *env)
{
	cl_int	err;

	(err = clSetKernelArg(env->cam->kl.kernel, 0, sizeof(cl_mem),
		&env->cam->kl.mem)) ? ft_err_handler("OpenCL", "Fail!", 0, 1) : 0;
	(err = clSetKernelArg(env->cam->kl.kernel, 1, sizeof(t_point),
		&env->cam->pos)) ? ft_err_handler("OpenCL", "Fail!", 0, 1) : 0;
	(err = clSetKernelArg(env->cam->kl.kernel, 2, sizeof(t_uint),
		&env->win->w)) ? ft_err_handler("OpenCL", "Fail!", 0, 1) : 0;
	(err = clSetKernelArg(env->cam->kl.kernel, 3, sizeof(t_uint),
		&env->win->h)) ? ft_err_handler("OpenCL", "Fail!", 0, 1) : 0;
	(err = clSetKernelArg(env->cam->kl.kernel, 4, sizeof(t_viewport),
		env->cam->vwp)) ? ft_err_handler("OpenCL", "Fail!", 0, 1) : 0;
	(err = clSetKernelArg(env->cam->kl.kernel, 5, sizeof(cl_mem),
		&env->objs)) ? ft_err_handler("OpenCL", "Fail!", 0, 1) : 0;
	(err = clSetKernelArg(env->cam->kl.kernel, 6, sizeof(cl_mem),
		&env->light)) ? ft_err_handler("OpenCL", "Fail!", 0, 1) : 0;
	err = clEnqueueNDRangeKernel(env->cl.queue, env->cam->kl.kernel, 2, NULL,
		(size_t[3]){env->win->w, env->win->h, 0}, NULL, 0, NULL, NULL);
	err ? ft_err_handler("OpenCL", "Fail!", 0, 1) : 0;
	err = clEnqueueReadBuffer(env->cl.queue, env->cam->kl.mem, CL_TRUE, 0,
		env->win->w * env->win->h * 4, env->win->surf->pixels, 0, NULL, NULL);
}
