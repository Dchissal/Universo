/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_tracing_teste.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchissal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 12:32:03 by dchissal          #+#    #+#             */
/*   Updated: 2024/10/02 12:32:05 by dchissal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <math.h>

#define WIDTH 1080
#define HEIGHT 720
#define FOV 110  // Campo de visão

typedef struct s_Vec3
{
	float x, y, z;
}	Vec3;

typedef struct s_Ray
{
	Vec3 origin;
	Vec3 direction;
}	Ray;

typedef struct s_Sphere
{
	Vec3 center;
	float radius;
}	Sphere;

typedef struct
{
	unsigned char r, g, b;
}	Color;

Vec3	vec_sub(Vec3 a, Vec3 b)
{
	return	(Vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec3	vec_add(Vec3 a, Vec3 b)
{
	return	(Vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

Vec3	vec_scale(Vec3 v, float t)
{
	return (Vec3){v.x * t, v.y * t, v.z * t};
}

float	vec_dot(Vec3 a, Vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float	vec_length(Vec3 v)
{
	return sqrtf(vec_dot(v, v));
}

Vec3	vec_normalize(Vec3 v)
{
	float len = vec_length(v);
	return (Vec3){v.x / len, v.y / len, v.z / len};
}

int	ray_intersect_sphere(Ray ray, Sphere sphere, float *t)
{
	Vec3 L = vec_sub(sphere.center, ray.origin);
	float tca = vec_dot(L, ray.direction);

	if (tca < 0)
		return 0;  // Raio não intersecta
	float d2 = vec_dot(L, L) - tca * tca;
	if (d2 > sphere.radius * sphere.radius)
		return 0;  // Raio não intersecta
	float thc = sqrtf(sphere.radius * sphere.radius - d2);
	*t = tca - thc;  // Distância para o ponto de interseção
	return 1;
}

Color	trace_ray(Ray ray, Sphere sphere)
{
	float	t;

	if (ray_intersect_sphere(ray, sphere, &t))
	{
		// Iluminação simples (tom de cinza baseado na distância)
		return (Color){255 - (unsigned char)(t * 10), 0, 255};  // Vermelho
	}
	return (Color){0, 0, 0};  // Fundo preto
}

void	render(Sphere sphere)
{
	FILE	*image = fopen("output.ppm", "w");
	fprintf(image, "P3\n%d %d\n255\n", WIDTH, HEIGHT);

	Vec3 camera_origin = {0, 0, 0};  // Câmera na origem

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
		// Coordenadas normalizadas da tela [-1, 1]
		float px = (2 * (x + 0.5) / (float)WIDTH - 1) * tanf(FOV / 2 * M_PI / 180) * WIDTH / (float)HEIGHT;
		float py = (1 - 2 * (y + 0.5) / (float)HEIGHT) * tanf(FOV / 2 * M_PI / 180);

		Vec3 direction = vec_normalize((Vec3){px, py, -1});  // Raio em direção à tela
		Ray ray = {camera_origin, direction};
		// Cor do pixel baseado no raio
		Color color = trace_ray(ray, sphere);
		fprintf(image, "%d %d %d ", color.r, color.g, color.b);
		}
		fprintf(image, "\n");
	}

	fclose(image);
}

int main() {
    // Definir uma esfera no espaço
    Sphere sphere = {{0, 0, -5}, 1};

    // Renderizar a cena
    render(sphere);

    printf("Renderização concluída! Verifique o arquivo output.ppm.\n");
    return 0;
}
