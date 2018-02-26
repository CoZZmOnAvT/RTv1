/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sgl_rotate_point.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/26 19:38:42 by pgritsen          #+#    #+#             */
/*   Updated: 2018/02/26 19:59:27 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sgl_private.h"

t_point		sgl_rotate_point(t_point p, t_rotate angle)
{
	t_point	sin_c;
	t_point	cos_c;
	t_point	RX;
	t_point	RY;
	t_point	RZ;

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
