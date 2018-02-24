/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/24 13:21:23 by pgritsen          #+#    #+#             */
/*   Updated: 2018/02/24 13:36:32 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

typedef unsigned char	t_uchar;
typedef unsigned int	t_uint;
typedef unsigned long	t_ulong;
typedef unsigned short	t_ushort;

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
	int		spec;
}				t_obj;

typedef struct	s_viewport
{
	double	w;
	double	h;
	double	dist;
}				t_viewport;

#endif