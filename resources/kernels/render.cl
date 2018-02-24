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

# define SMOOTH_LEVEL 2

inline float3	reflect_ray(float3 R, float3 N)
{
	return (2.0F * N * dot(N, R) - R);
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
	obj_data.obj.color = 0x000000;
	while (objs[++it].color)
	{
		T = intersect_ray_sphere(O, D, objs[it]);
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

float			compute_lighting(float3 P, float3 N, float3 V, int s, float max,
						__constant t_light *light, __constant t_obj *objs)
{
	t_obj_data	shadow_obj;
	float3		L;
	float3		R;
	float		coef;
	float		n_dot_l;
	float		r_dot_v;
	int			it;

	coef = 0.0;
	it = -1;
	while (light[++it].type != -1)
		if (light[it].type == 0)
			coef += light[it].intens;
		else
		{
			if (light[it].type == 1)
				L = (float3){light[it].pos.x - P.x, light[it].pos.y - P.y, light[it].pos.z - P.z};
			else
				L = (float3){light[it].dir.x, light[it].dir.y, light[it].dir.z};

			shadow_obj = closest_intersection(P, L, 0.001, max, objs);
			if (shadow_obj.obj.color != 0x000000)
				continue ;

			n_dot_l = dot(N, L);
			if (n_dot_l > 0)
				coef += light[it].intens * n_dot_l / (fast_length(N) * fast_length(L));

			if (s > 0)
			{
				R = 2.0F * N * n_dot_l - L;
				r_dot_v = dot(R, V);
				if (r_dot_v > 0)
					coef += light[it].intens * pown(r_dot_v / (fast_length(R) * fast_length(V)), s);
			}
		}
	return (coef);
}

t_uint			trace_ray(float3 O, float3 D, float min, float max,
						__constant t_obj *objs, __constant t_light *light)
{
	t_obj_data	obj_data;

	float3		P;
	float3		R;
	float3		N;
	float3		CO_C;

	float		light_coef;

	float4		reflected_color;
	float4		local_color;
	float4		result_color;
	float		r;

	obj_data = closest_intersection(O, D, min, max, objs);
	if (obj_data.obj.color == 0x000000)
		return (0x000000);

	CO_C = (float3){obj_data.obj.pos.x, obj_data.obj.pos.y, obj_data.obj.pos.z};
	P = O + obj_data.closest_t * D;
	N = P - CO_C;
	N = N / fast_length(N);

	light_coef = compute_lighting(P, N, -D, obj_data.obj.spec, max, light, objs);
	light_coef > 1 ? light_coef = 1 : 0;

	local_color.x = obj_data.obj.color >> 24 & 0xFF;
	local_color.y = obj_data.obj.color >> 16 & 0xFF;
	local_color.z = obj_data.obj.color >> 8 & 0xFF;
	local_color.w = obj_data.obj.color & 0xFF;
	local_color *= light_coef;

	result_color = local_color;
	r = obj_data.obj.refl;
	if (r <= 0)
		return ((uchar)result_color.y * 0x10000 + (uchar)result_color.z * 0x100 + (uchar)result_color.w);

	R = reflect_ray(-D, N);

	obj_data = closest_intersection(P, R, 0.001, max, objs);
	if (obj_data.obj.color == 0x000000)
		return ((uchar)local_color.y * 0x10000 + (uchar)local_color.z * 0x100 + (uchar)local_color.w);

	CO_C = (float3){obj_data.obj.pos.x, obj_data.obj.pos.y, obj_data.obj.pos.z};
	P = P + obj_data.closest_t * R;
	N = P - CO_C;
	N = N / fast_length(N);

	light_coef = compute_lighting(P, N, -R, obj_data.obj.spec, max, light, objs);
	light_coef > 1 ? light_coef = 1 : 0;

	reflected_color.x = obj_data.obj.color >> 24 & 0xFF;
	reflected_color.y = obj_data.obj.color >> 16 & 0xFF;
	reflected_color.z = obj_data.obj.color >> 8 & 0xFF;
	reflected_color.w = obj_data.obj.color & 0xFF;
	reflected_color *= light_coef;
	result_color = local_color * (1.0F - r) + reflected_color * r;

	return ((uchar)result_color.y * 0x10000 + (uchar)result_color.z * 0x100 + (uchar)result_color.w);
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

__kernel void
render_scene(__global t_uint *pixels, t_point cam_pos, t_uint w_width, t_uint w_height,
				t_viewport vwp, __constant t_obj *objs, __constant t_light *light)
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

	O = (float3){cam_pos.x, cam_pos.y, cam_pos.z};
	while (++itx < SMOOTH_LEVEL)
	{
		ity = -1;
		while (++ity < SMOOTH_LEVEL)
		{
			D = canvas_to_viewport(x + (itx + 0.5) / SMOOTH_LEVEL, y + (ity + 0.5) / SMOOTH_LEVEL, vwp, w_width, w_height);
			color[ity * SMOOTH_LEVEL + itx] = trace_ray(O, D, 1, INFINITY, objs, light);
		}
	}

	pixels[screen_y * w_width + screen_x] = avg_color(color, SMOOTH_LEVEL * SMOOTH_LEVEL);
}
