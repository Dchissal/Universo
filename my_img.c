/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_img.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchissal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:12:37 by dchissal          #+#    #+#             */
/*   Updated: 2024/10/02 11:12:38 by dchissal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define WIDTH 1080
#define HEIGHT 720

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}		t_data;

void	my_mlx_pixel_put(t_data *system, int x, int y, int color)
{
	char	*pixel;
	int		index;

	//if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
	//{
		index = (y * system->line_length) + (x * (system->bits_per_pixel / 8));
		pixel = system->addr + index;
		*(unsigned int *)pixel = color;
	//}
}

void	fill_background(t_data *img, int color)
{
	int x;
	int y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			my_mlx_pixel_put(img, x, y, color);
			x++;
		}
		y++;
	}
}

void	dda(int x1, int x2, int y1, int y2, t_data *img)
{
	int	step;
	float	x, y, xinc, yinc;
	
	step = fabs(x2 - x1);
	
	/*if (fabs(y2 - y1) > step)
		step = fabs(y2 - y1);
	xinc = (x2 - x1) / step;
	yinc = (y2 - y1) / step;*/
	step = fabs(x2 - x1);
	if (fabs(y2 - y1) > step)
		step = fabs(y2 - y1);
		
	xinc = (float)(x2 - x1) / step;
	yinc = (float)(y2 - y1) / step;
	x = x1;
	y = y1;
	int	i = 0;
	
	while (i <= step)
	{
		my_mlx_pixel_put(img, 1000 / 2 + x, 720 / 2 + y, 0xFFFFFF);
		x = x + xinc;
		y = y + yinc;
		i++;
		//printf("chegouuuuu!\n");
	}
}

int	main(void)
{
	void	*mlx;
	void	*mlx_win;
	int	background_color;
	int	y;
	int	x;
	t_data	img;
	
	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 1080, 720, "Pixel na janela");
	img.img = mlx_new_image(mlx, 1000, 720);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
								&img.endian);
	background_color = 0x003366;
	fill_background(&img, background_color);
	my_mlx_pixel_put(&img, 15, 15, 0xFFFFFF);
	y = 0;
	while (y < 300)
	{
		x = 0;
		while (x < 300)
		{
			dda(x, x + 1, y, y + 1, &img);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(mlx, mlx_win, img.img, 180, 0);
	mlx_loop(mlx);
}
