/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.cl                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/23 14:32:50 by pgritsen          #+#    #+#             */
/*   Updated: 2018/02/23 20:34:48 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma OPENCL EXTENSION cl_khr_fp64 : enable

typedef unsigned int	t_uint;
typedef unsigned char	t_uchar;

typedef struct			s_point
{
	double	x;
	double	y;
	double	z;
}						t_point;

typedef struct			s_viewport
{
	double	w;
	double	h;
	double	dist;
}						t_viewport;

typedef struct	s_light
{
	char	type;
	double	intens;
	t_point	pos;
	t_point	dir;
}				t_light;

typedef struct	s_obj
{
	t_uint	color;
	t_point	pos;
	double	rad;
}				t_obj;

double2	intersect_ray_sphere(double3 O, double3 D, t_obj obj)
{
	double	descr;
	double	k1;
	double	k2;
	double	k3;
	double3	OC;
	double3 C = (double3){obj.pos.x, obj.pos.y, obj.pos.z};

	OC = O - C;

	k1 = dot(D, D);
	k2 = 2.0 * dot(OC, D);
	k3 = dot(OC, OC) - obj.rad * obj.rad;

	descr = k2 * k2 - 4.0 * k1 * k3;
	if (descr < 0)
		return ((double2){INFINITY, INFINITY});
	return ((double2){
		(-k2 + sqrt(descr)) / (2.0 * k1),
		(-k2 - sqrt(descr)) / (2.0 * k1)});
}

double	compute_lighting(double3 P, double3 N, __constant t_light *light)
{
	double3	L;
	double	coef;
	double	n_dot_l;
	int		it;

	coef = 0.0;
	it = -1;
	while (light[++it].type != -1)
		if (light[it].type == 0)
			coef += light[it].intens;
		else
		{
			if (light[it].type == 1)
				L = (double3){light[it].pos.x - P.x, light[it].pos.y - P.y, light[it].pos.z - P.z};
			else
				L = (double3){light[it].dir.x, light[it].dir.y, light[it].dir.z};
			n_dot_l = dot(N, L);
			if (n_dot_l > 0)
				coef += light[it].intens * n_dot_l / (length(N) * length(L));
		}
	return (coef);
}

t_uint	trace_ray(double3 O, double3 D, double min, double max,
					__constant t_obj *objs, __constant t_light *light)
{
	double2		T;
	double		closest_t;
	t_obj		closest_obj;
	int			it;

	double3		P;
	double3		N;
	double3		CO_C;

	double		light_coef;

	closest_t = INFINITY;
	closest_obj.color = 0x000000;
	it = -1;
	while (objs[++it].color)
	{
		T = intersect_ray_sphere(O, D, objs[it]);
		if (T.x >= min && T.x <= max && T.x < closest_t)
		{
			closest_t = T.x;
			closest_obj = objs[it];
		}
		if (T.y >= min && T.y <= max && T.y < closest_t)
		{
			closest_t = T.y;
			closest_obj = objs[it];
		}
	}
	if (closest_obj.color == 0x000000)
		return (0x000000);

	CO_C = (double3){closest_obj.pos.x, closest_obj.pos.y, closest_obj.pos.z};
	P = O + closest_t * D;
	N = P - CO_C;
	N = N / length(N);

	light_coef = compute_lighting(P, N, light);

	int	r = (closest_obj.color >> 16 & 0xFF) * light_coef;
	int	g = (closest_obj.color >> 8 & 0xFF) * light_coef;
	int	b = (closest_obj.color & 0xFF) * light_coef;

	return (r * 0x10000 + g * 0x100 + b);
}

__kernel void
render_scene(__global t_uint *pixels, t_point cam_pos, t_uint w_width, t_uint w_height,
				t_viewport vwp, __constant t_obj *objs, __constant t_light *light)
{
	int			screen_x = get_global_id(0);
	int			screen_y = get_global_id(1);
	int			x = screen_x - w_width / 2;
	int			y = screen_y - w_height / 2;
	t_uint		color;
	double3		O;
	double3		D = (double3){	x * vwp.w / w_width,
								y * vwp.h / w_height,
								vwp.dist};

	O = (double3){cam_pos.x, cam_pos.y,	cam_pos.z};
	color = trace_ray(O, D, 1, INFINITY, objs, light);

	screen_y = w_height - screen_y - 1;
	pixels[screen_y * w_width + screen_x] = color;
}
