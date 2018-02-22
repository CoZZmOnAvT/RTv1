/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sgl_draw_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/08 18:37:28 by pgritsen          #+#    #+#             */
/*   Updated: 2018/02/22 16:02:16 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sgl_private.h"

/*
**	void	sgl_draw_line(t_point a, t_point b, t_uint color, SDL_Renderer *rend)
**	{
**		SDL_Color	tmp;
**		t_point		d;
**		t_point		s;
**		int			err[2];
**
**		d = (t_point){fabs(b.x - a.x), fabs(b.y - a.y), 0};
**		s = (t_point){a.x < b.x ? 1 : -1, a.y < b.y ? 1 : -1, 0};
**		err[0] = (d.x > d.y ? d.x : -d.y) / 2;
**		while (1)
**		{
**			sgl_plot(a, color, rend);
**			if (a.x == b.x && a.y == b.y)
**				break ;
**			err[1] = err[0];
**			err[1] > -d.x ? (err[0] -= d.y) : 0;
**			err[1] > -d.x ? (a.x += s.x) : 0;
**			err[1] < d.y ? (err[0] += d.x) : 0;
**			err[1] < d.y ? (a.y += s.y) : 0;
**		}
**	}
*/

void	sgl_draw_line(t_point a, t_point b, t_uint color, SDL_Renderer *rend)
{
	SDL_Color	tmp;

	SDL_GetRenderDrawColor(rend, &tmp.r, &tmp.g, &tmp.b, &tmp.a);
	SDL_SetRenderDrawColor(rend, color >> 16 & 0xFF, color >> 8 & 0xFF,
									color & 0xFF, color >> 24 & 0xFF);
	SDL_RenderDrawLine(rend, a.x, a.y, b.x, b.y);
	SDL_SetRenderDrawColor(rend, tmp.r, tmp.g, tmp.b, tmp.a);
}