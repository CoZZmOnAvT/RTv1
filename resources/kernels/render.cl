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

#include "render.h.cl"

/*---------------------------------UTILITIES---------------------------------*/

inline float3	reflect_ray(float3 R, float3 N)
{
	return (2.0F * N * dot(N, R) - R);
}

t_uint		avg_color(t_uint arr[], t_uint num)
{
	int		it = -1;
	t_uint	r = 0;
	t_uint	g = 0;
	t_uint	b = 0;

	while(++it < num)
	{
		r += arr[it] >> 16 & 0xFF;
		g += arr[it] >> 8 & 0xFF;
		b += arr[it] & 0xFF;
	}
	r /= num;
	g /= num;
	b /= num;
	return (r * 0x10000 + g * 0x100 + b);
}

float3		canvas_to_viewport(float x, float y, t_viewport vwp, t_uint w_width, t_uint w_height)
{
	return ((float3){x * vwp.w / w_width, y * vwp.h / w_height, vwp.dist});
}

float3		rotate_point(float3 rot, float3 D)
{
	float3	sin_c;
	float3	cos_c;
	float3	RX;
	float3	RY;
	float3	RZ;

	sin_c.x = sin(rot.x * M_PI / 180.0);
	cos_c.x = cos(rot.x * M_PI / 180.0);
	sin_c.y = sin(rot.y * M_PI / 180.0);
	cos_c.y = cos(rot.y * M_PI / 180.0);
	sin_c.z = sin(rot.z * M_PI / 180.0);
	cos_c.z = cos(rot.z * M_PI / 180.0);

	RX.x = D.x;
	RX.y = D.y * cos_c.x + D.z * sin_c.x;
	RX.z = D.z * cos_c.x - D.y * sin_c.x;

	RY.x = RX.x * cos_c.y - RX.z * sin_c.y;
	RY.y = RX.y;
	RY.z = RX.z * cos_c.y + RX.x * sin_c.y;

	RZ.x = RY.x * cos_c.z + RY.y * sin_c.z;
	RZ.y = RY.y * cos_c.z - RY.x * sin_c.z;
	RZ.z = RY.z;
	return (RZ);
}

float3			calc_normal(float3 P, t_obj obj)
{
	float3		proj;
	float3		OP = {obj.pos.x, obj.pos.y, obj.pos.z};
	float3		OD = {obj.dir.x, obj.dir.y, obj.dir.z};
	float3		N;
	float3		T = {0.0F, 1.0F, 0.0F};

	if (obj.type == PLANE)
		return (OD);
	N = P - OP;
	if (obj.type == CYLINDER || obj.type == CONE)
	{
		N = P - OP;
		proj = T * dot(N, T);
		N = (N - proj) / fast_length(N);
	}
	N = N / fast_length(N);
	return (N);
}

float3			sum_colors(float3 a, float3 b)
{
	float3 res;

	res = a + b;
	res.x > 255 ? res.x = 255 : 0;
	res.y > 255 ? res.y = 255 : 0;
	res.z > 255 ? res.z = 255 : 0;
	return (res);
}

/*-------------------------------INTERSECTIONS-------------------------------*/

float2	intersect_ray_plane(float3 O, float3 D, t_obj obj)
{
	float3	OP = {obj.pos.x, obj.pos.y, obj.pos.z};
	float3	OD = {obj.dir.x, obj.dir.y, obj.dir.z};
	float3	OC;
	float2	T;
	float	k[2];

	OC = O - OP;
	k[0] = dot(D, OD);
	k[1] = dot(OC, OD);
	if (k[0])
	{
		T.x = -k[1] / k[0];
		T.y = INFINITY;
		return (T);
	}
	return ((float2){INFINITY, INFINITY});
}

float2			intersect_ray_cylinder(float3 O, float3 D, t_obj obj)
{
	double	descr;
	double	k1;
	double	k2;
	double	k3;
	float3	D_Va;
	float3	OC_Va;
	float3	OC;
	float3	C = {obj.pos.x, obj.pos.y, obj.pos.z};
	float3	CT = {obj.dir.x, obj.dir.y, obj.dir.z};
	float3	Va = (CT - C) / fast_length(CT - C);

	OC = O - C;
	D_Va = D - dot(D, Va) * Va;
	OC_Va = OC - dot(OC, Va) * Va;
	k1 = dot(D_Va, D_Va);
	k2 = 2.0F * dot(D_Va, OC_Va);
	k3 = dot(OC_Va, OC_Va) - obj.rad * obj.rad;

	descr = k2 * k2 - 4.0F * k1 * k3;
	if (descr < 0)
		return ((float2){INFINITY, INFINITY});
	return ((float2){
		(-k2 + sqrt(descr)) / (2.0F * k1),
		(-k2 - sqrt(descr)) / (2.0F * k1)});
}

float2			intersect_ray_sphere(float3 O, float3 D, t_obj obj)
{
	double	descr;
	double	k1;
	double	k2;
	double	k3;
	float3	OC;
	float3	C = (float3){obj.pos.x, obj.pos.y, obj.pos.z};

	OC = O - C;

	k1 = dot(D, D);
	k2 = 2.0F * dot(OC, D);
	k3 = dot(OC, OC) - obj.rad * obj.rad;

	descr = k2 * k2 - 4.0F * k1 * k3;
	if (descr < 0)
		return ((float2){INFINITY, INFINITY});
	return ((float2){
		(-k2 + sqrt(descr)) / (2.0F * k1),
		(-k2 - sqrt(descr)) / (2.0F * k1)});
}

t_obj_data		closest_intersection(float3 O, float3 D, float min, float max,
								__constant t_obj *objs)
{
	t_obj_data	obj_data;
	float2		T;
	int			it;

	obj_data.closest_t = INFINITY;
	obj_data.obj.type = -1;
	while (objs[++it].type != -1)
	{
		if (objs[it].type == SPHERE)
			T = intersect_ray_sphere(O, D, objs[it]);
		else if (objs[it].type == CYLINDER)
			T = intersect_ray_cylinder(O, D, objs[it]);
		else if (objs[it].type == PLANE)
			T = intersect_ray_plane(O, D, objs[it]);
		if (T.x >= min && T.x <= max && T.x < obj_data.closest_t)
		{
			obj_data.closest_t = T.x;
			obj_data.obj = objs[it];
		}
		if (T.y >= min && T.y <= max && T.y < obj_data.closest_t)
		{
			obj_data.closest_t = T.y;
			obj_data.obj = objs[it];
		}
	}
	return (obj_data);
}

/*-----------------------------------LIGHT-----------------------------------*/

float		compute_lighting(float3 P, float3 N, float3 V, int s,
						__constant t_light *light, __constant t_obj *objs)
{
	t_obj_data	shadow_obj;
	float3		L;
	float3		R;
	float		coef;
	float		ln;
	int			it;

	coef = 0;
	it = -1;
	while (light[++it].type != -1)
		if (light[it].type == 0)
			coef += light[it].intens;
		else
		{
			L = (float3){light[it].pos.x - P.x, light[it].pos.y - P.y, light[it].pos.z - P.z};
			L /= fast_length(L);

			shadow_obj = closest_intersection(P, L, 0.001, 2.0F, objs);
			if (shadow_obj.obj.type != -1)
				continue ;

			ln = dot(L, N);
			coef += light[it].intens * fmax(0.0F, ln);

			if (s > 0)
			{
				R = 2.0F * N * dot(N, L) - L;
				coef += light[it].intens * pown(fmax(0.0F, dot(R, V)), s);
			}
		}
	return (coef);
}

/*---------------------------------RAYTRACING---------------------------------*/

t_uint			trace_ray(float3 O, float3 D, float min, float max,
						__constant t_obj *objs, __constant t_light *light)
{
	t_obj_data	obj_data;

	float3		P;
	float3		R;
	float3		N;

	float		light_coef;
	float3		color = 0;
	float3		tmp;
	float		r = 0;

	int			it = -1;

	while(++it < REFLECT_DEPTH)
	{
		obj_data = closest_intersection(O, D, min, max, objs);
		if (obj_data.obj.type == -1)
			break ;
		P = O + obj_data.closest_t * D;
		N = calc_normal(P, obj_data.obj);

		light_coef = compute_lighting(P, N, -D, obj_data.obj.spec, light, objs);
		light_coef > 1 ? light_coef = 1 : 0;

		tmp = (float3){obj_data.obj.color >> 16 & 0xFF,
								obj_data.obj.color >> 8 & 0xFF,
								obj_data.obj.color & 0xFF} * light_coef;
		tmp *= (it == 0 ? (1.0F - r) : r);
		color = sum_colors(color, tmp);
		r = obj_data.obj.refl;
		if (r <= 0)
			break ;
		R = reflect_ray(-D, N);
		O = P;
		D = R;
		min = 0.001;
	}
	return ((uchar)color.x * 0x10000 + (uchar)color.y * 0x100 + (uchar)color.z);
}

__kernel void
render_scene(__global t_uint *pixels, t_point cam_pos, t_rotate cam_rot,
				t_uint w_width, t_uint w_height, t_viewport vwp,
				__constant t_obj *objs, __constant t_light *light)
{
	int			screen_x = get_global_id(0);
	int			screen_y = get_global_id(1);
	int			x = screen_x - w_width / 2;
	int			y = w_height / 2 - screen_y;
	int			itx = -1;
	int			ity = -1;
	t_uint		color[SMOOTH_LEVEL * SMOOTH_LEVEL];
	float3		O;
	float3		D;
	float3		CR = (float3){cam_rot.rx, cam_rot.ry, cam_rot.rz};

	O = (float3){cam_pos.x, cam_pos.y, cam_pos.z};
	while (++itx < SMOOTH_LEVEL)
	{
		ity = -1;
		while (++ity < SMOOTH_LEVEL)
		{
			D = rotate_point(CR, canvas_to_viewport(x + (itx + 0.5) / SMOOTH_LEVEL,
								y + (ity + 0.5) / SMOOTH_LEVEL, vwp, w_width, w_height));
			color[ity * SMOOTH_LEVEL + itx] = trace_ray(O, D, 1, INFINITY, objs, light);
		}
	}

	pixels[screen_y * w_width + screen_x] = avg_color(color, SMOOTH_LEVEL * SMOOTH_LEVEL);
}
