/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sgl_plot.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/08 18:37:34 by pgritsen          #+#    #+#             */
/*   Updated: 2018/02/22 15:00:07 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sgl_private.h"

inline void		sgl_plot(t_point p, t_uint color, SDL_Texture *tex)
{
	int			dx;
	int			dy;
	t_uint		*pixels;
	SDL_Rect	size;

	SDL_QueryTexture(tex, NULL, NULL, &size.w, &size.h);
	SDL_LockTexture(tex, NULL, (void **)&pixels, &size.x);
	dx = ROUND(p.x);
	dy = ROUND(p.y);
	if (dx >= 0 && dx < size.w && dy >= 0 && dy < size.h)
		pixels[dy * (size.x / sizeof(t_uint)) + dx] = color;
	SDL_UnlockTexture(tex);
}
