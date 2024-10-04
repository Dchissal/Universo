/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   universo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchissal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:39:53 by dchissal          #+#    #+#             */
/*   Updated: 2024/10/01 12:39:54 by dchissal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdlib.h>
#include <math.h>

typedef struct s_point
{
	int	x;
	int	y;
	int	z;
}	t_point;

typedef struct s_universe
{
	void	*mlx_ptr;
	void	*win_ptr;
	int	width;
	int	height;
} t_universe;

void	draw_star(t_universe *universe, int x, int y);
void	draw_planet(t_universe *universe, int x, int y);
void	draw_universe(t_universe *universe);

void	draw_star(t_universe *universe, int x, int y)
{
	mlx_pixel_put(universe->mlx_ptr, universe->win_ptr, x, y, 0xFFFFFF);
}

void	draw_planet(t_universe *universe, int x, int y)
{
	int	radius = 5;
	for (int i = -radius; i <= radius; i++)
	{
		for (int j = -radius; j <= radius; j++)
		{
		    if (i * i + j * j <= radius * radius)
			mlx_pixel_put(universe->mlx_ptr, universe->win_ptr, x + i, y + j, 0xFF0000);
		}
	}
}

void	draw_universe(t_universe *universe)
{
	for (int i = 0; i < 100; i++)
	{
		int x = rand() % universe->width;
		int y = rand() % universe->height;
		draw_star(universe, x, y);
	}
	draw_planet(universe, universe->width / 2, universe->height / 2);
}

int	main(void)
{
	t_universe universe;

	universe.width = 800;
	universe.height = 600;
	universe.mlx_ptr = mlx_init();
	universe.win_ptr = mlx_new_window(universe.mlx_ptr, universe.width, universe.height, "Universe");

	draw_universe(&universe);

	mlx_loop(universe.mlx_ptr);
	return (0);
}
