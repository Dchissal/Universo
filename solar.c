/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solar.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchissal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:52:00 by dchissal          #+#    #+#             */
/*   Updated: 2024/10/01 12:52:01 by dchissal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define WIDTH 1080
#define HEIGHT 920

typedef struct s_point
{
	int	x;
	int	y;
	int	z;
}	t_point;

typedef struct s_planet
{
	double	speed;     
	double	distance;  
	double	angle;      
	int		color;         
	double	size;
	double	z;
	double	semi_eixo_y;
}	t_planet;

typedef struct s_solar_system
{
	void	*mlx_ptr;
	void	*win_ptr;
	void	*img_ptr;
	char	*img_data;
	int     	bits_per_pixel;
	int     	size_line;
	int		endian;
	t_planet	sun;
	t_planet	planets[8];
}	t_solar_system;

void	draw_planet(t_solar_system *system, t_planet planet, double camera_distance);
void	update_planets(t_solar_system *system);
void	draw_sun(t_solar_system *system);

t_point project_to_2d(t_point point, double distance_from_camera)
{
	t_point	projected;
	double	factor = distance_from_camera / (distance_from_camera - point.z);

	projected.x = point.x * factor + WIDTH / 2;
	projected.y = point.y * factor + HEIGHT / 2;

	return (projected);
}

/*void	draw_pixel(t_solar_system *system, int x, int y, int color)
{
	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
	{
		int pixel_pos = (x * system->bpp / 8) + (y * system->size_line);
		system->img_data[pixel_pos] = color;
		system->img_data[pixel_pos + 1] = color >> 8;
		system->img_data[pixel_pos + 2] = color >> 16;
	}
}*/

void	draw_pixel(t_solar_system *system, int x, int y, int color)
{
	char	*pixel;
	int		index;

	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
	{
		index = (y * system->size_line) + (x * (system->bits_per_pixel / 8));
		pixel = system->img_data + index;
		*(unsigned int *)pixel = color;
	}
}

void	draw_sun(t_solar_system *system)
{
	int	radius = 30;

	for (int x = -radius; x <= radius; x++)
	{
		for (int y = -radius; y <= radius; y++)
		{
		    if (x * x + y * y <= radius * radius)
				draw_pixel(system, WIDTH / 2 + x, HEIGHT / 2 + y, 0xFFFF00);
		}
	}
}

/*void	draw_planet_with_shadow(t_solar_system *system, t_planet planet, double camera_distance)
{
	t_point planet_position;
	planet_position.x = planet.distance * cos(planet.angle);
	planet_position.y = planet.distance * sin(planet.angle);
	planet_position.z = planet.z;

	t_point projected = project_to_2d(planet_position, camera_distance);
	
	int radius = planet.size;
	for (int i = -radius; i <= radius; i++)
	{
		for (int j = -radius; j <= radius; j++)
		{
			if (i * i + j * j <= radius * radius)
			{
				int pixel_x = projected.x + i;
				int pixel_y = projected.y + j;
				int color = planet.color;

				// Calcular a direção do sol
				double sun_x = WIDTH / 2;
				double sun_y = HEIGHT / 2;
				double angle_to_sun = atan2(sun_y - projected.y, sun_x - projected.x);

				// Verifica se o pixel está no lado oposto ao sol
				double pixel_angle = atan2(j, i);
				if (fabs(pixel_angle - angle_to_sun) > M_PI / 2) // Se o pixel estiver no lado oposto
				{
					// Escurecer a cor
					color = (color & 0xFF0000) >> 1; // Diminuir a intensidade do vermelho
					color |= (color & 0x00FF00) >> 1; // Diminuir a intensidade do verde
					color |= (color & 0x0000FF) >> 1; // Diminuir a intensidade do azul
				}
				draw_pixel(system, pixel_x, pixel_y, color);
			}
		}
	}
}*/


void	draw_planet(t_solar_system *system, t_planet planet, double camera_distance)
{
	t_point planet_position;
	planet_position.x = planet.distance * cos(planet.angle);
	planet_position.y = planet.distance * sin(planet.angle);
	planet_position.z = planet.z;

	t_point projected = project_to_2d(planet_position, camera_distance);
	
	int	radius = planet.size;
	for (int i = -radius; i <= radius; i++)
	{
		for (int j = -radius; j <= radius; j++)
		{
		    if (i * i + j * j <= radius * radius)
				draw_pixel(system, projected.x + i, projected.y + j, planet.color);
		}
	}
}

void	update_planets(t_solar_system *system)
{
	for (int i = 0; i < 8; i++)
	{
		system->planets[i].angle += system->planets[i].speed;
		if (system->planets[i].angle > 2 * M_PI)
			system->planets[i].angle -= 2 * M_PI;  // Loop da órbita
	}
}

void	initialize_planets(t_solar_system *system)
{
	system->sun.angle = 0;
	system->sun.distance = 0;
	system->sun.size = 30;
	system->sun.color = 0xFFFF00;
	system->sun.z = 0; 

	system->planets[0] = (t_planet){0.01, 100, 0, 0xFF0000, 5, 0, 90};  // Mercúrio
	system->planets[1] = (t_planet){0.008, 150, 0, 0xFFA500, 7, 10, 70}; // Vênus
	system->planets[2] = (t_planet){0.006, 200, 0, 0x0000FF, 8, 20, 80}; // Terra
	system->planets[3] = (t_planet){0.004, 250, 0, 0xFF69B4, 6, 30, 60}; // Marte
	system->planets[4] = (t_planet){0.002, 300, 0, 0xFFFF00, 12, -20, 120}; // Júpiter
	system->planets[5] = (t_planet){0.0015, 350, 0, 0x87CEEB, 10, -30, 100}; // Saturno
	system->planets[6] = (t_planet){0.001, 400, 0, 0xADD8E6, 9, -10, 90}; // Urano
	system->planets[7 ] = (t_planet){0.0005, 450, 0, 0x00008B, 8, -5, 75}; // Netuno
}

void	draw_universe(t_solar_system *system)
{
	for (int i = 0; i < 100; i++)
	{
		int x = rand() % WIDTH;
		int y = rand() % HEIGHT;
		draw_pixel(system, x, y, 0xFFFFFF);
		//usleep(1000);
	}
}

int	render_frame(t_solar_system *system)
{
	mlx_clear_window(system->mlx_ptr, system->win_ptr);
	
	memset(system->img_data, 0, WIDTH * HEIGHT * (system->bits_per_pixel / 8));
	
	draw_sun(system);
	draw_universe(system);
	update_planets(system);
	double camera_distance = 450000.9;
	for (int i = 0; i < 8; i++)
	{
		draw_planet(system, system->planets[i], camera_distance);
		//draw_planet_with_shadow(system, system->planets[i], camera_distance);
	}
	//mlx_clear_window(system->mlx_ptr, system->win_ptr);
	mlx_put_image_to_window(system->mlx_ptr, system->win_ptr, system->img_ptr, 0, 0);
	usleep(1000);
	return (0);
}

int	close_window(t_solar_system *system)
{
	mlx_destroy_window(system->mlx_ptr, system->win_ptr);
	exit(0);
	return (0);
}

int	main(void)
{
	t_solar_system system;

	system.mlx_ptr = mlx_init();
	system.win_ptr = mlx_new_window(system.mlx_ptr, WIDTH, HEIGHT, "Solar System Simulation");
	system.img_ptr = mlx_new_image(system.mlx_ptr, WIDTH, HEIGHT);
	system.img_data = mlx_get_data_addr(system.img_ptr, &system.bits_per_pixel, &system.size_line, &system.endian);

	initialize_planets(&system);
	draw_universe(&system);
	/*
	mlx_clear_window(system.mlx_ptr, system.win_ptr);
	
	memset(system.img_data, 0, WIDTH * HEIGHT * (system.bits_per_pixel / 8));
	
	draw_sun(&system);
	draw_universe(&system);
	update_planets(&system);
	double camera_distance = 450000.0;
	for (int i = 0; i < 8; i++)
	{
		draw_planet(&system, system.planets[i], camera_distance);
		//draw_planet_with_shadow(system, system->planets[i], camera_distance);
	}
	//mlx_clear_window(system->mlx_ptr, system->win_ptr);
	mlx_put_image_to_window(system.mlx_ptr, system.win_ptr, system.img_ptr, 0, 0);
	usleep(1000);
	//return (0);
	//draw_universe(&system);*/
	mlx_hook(system.win_ptr, 17, 0, close_window, &system);
	mlx_loop_hook(system.mlx_ptr, render_frame, &system);
	mlx_loop(system.mlx_ptr);

	return (0);
}
