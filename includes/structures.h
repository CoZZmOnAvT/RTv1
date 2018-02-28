/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/24 13:21:23 by pgritsen          #+#    #+#             */
/*   Updated: 2018/02/27 15:39:43 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

typedef struct	s_light
{
	char	type;
	double	intens;
	t_point	pos;
	t_point	dir;
}				t_light;

typedef struct	s_obj
{
	t_uint		color;
	short int	type;
	t_point		pos;
	t_point		dir;
	double		rad;
	int			spec;
	double		refl;
}				t_obj;

typedef struct	s_viewport
{
	double	w;
	double	h;
	double	dist;
}				t_viewport;

#endif