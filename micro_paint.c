/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: graja <graja@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 13:38:27 by graja             #+#    #+#             */
/*   Updated: 2021/12/08 16:21:41 by graja            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef	struct	s_cvs
{
	int	x;
	int	y;
	char	bck;
}		t_cvs;

typedef struct	s_rec
{
	char	code;
	float	x;
	float	y;
	float	w;
	float	h;
	char	colr;
}		t_rec;

static
int	print_error(FILE *file)
{
	write(1, "Error: Operation file corrupted\n", 32);
	if (file)
		fclose(file);
	return (1);
}

static
void	print_cvs(char **matrix, t_cvs cvs)
{
	int	y;

	y = 0;
	while (y < cvs.y)
	{
		write(1, matrix[y], cvs.x);
		write(1, "\n", 1);
		free(matrix[y]);
		y++;
	}
	free(matrix);
}
static
int	dist(float x1, float y1, float x2, float y2)
{
	float	dist;

	dist = powf((powf(x1 -x2, 2) + powf(y1 - y2, 2)), 0.5);
	if (dist < 1.0 && dist >= 0.0)
		return (1);
	return (0);
}

static
int	check_recn(float x, float y, t_rec rec, int flag)
{
	float	rx;
	float	ry;

	if (x < rec.x || x > rec.x + rec.w || y < rec.y || y > rec.y + rec.h)
		return (0);
	rx = rec.x;
	while (rx < rec.x + rec.w)
	{
		ry = rec.y;
		while (ry < rec.y + rec.h)
		{
			if (flag && dist(x, y, rx, ry))
				return (1);
			else if (!flag && dist(x, y, rx, rec.y))
				return (1);
			else if (!flag && dist(x, y, rx, rec.y + rec.h))
				return (1);
			else if (!flag && dist(x, y, rec.x, ry))
				return (1);
			else if (!flag && dist(x, y, rec.x + rec.w, ry))
				return (1);
			ry += 1.0;
		}
		rx += 1.0;
	}
	return (0);
}

static
void	print_rec(char **matrix, t_cvs cvs, t_rec rec)
{
	int	flag;
	int	x;
	int	y;

	if (rec.code == 'R')
		flag = 1;
	else
		flag = 0;
	x = 0;
	while (x < cvs.x)
	{
		y = 0;
		while (y < cvs.y)
		{
			if (check_recn(x, y, rec, flag))
				matrix[y][x] = rec.colr;
			y++;
		}
		x++;
	}
}

int	main(int argc, char **argv)
{
	t_cvs	cvs;
	t_rec	rec;
	FILE	*file;
	char	**matrix;
	int	i;
	int	err;

	if (argc != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	file = fopen(argv[1], "r");
	if (!file)
		return (print_error(file));
	err = fscanf(file, "%d %d %c\n", &(cvs.x), &(cvs.y), &(cvs.bck));
	if (err != 3)
		return (print_error(file));
	i = 0;
	matrix = malloc(cvs.y * sizeof(char *));
	if (!matrix)
		return (1);
	while (i < cvs.y)
	{
		matrix[i] = calloc(cvs.x, sizeof(char));
		if (!matrix[i])
			return (1);
		memset(matrix[i], cvs.bck, cvs.x);
		i++;
	}
	err = 6;
	while (err == 6)
	{
		err = fscanf(file, "%c %f %f %f %f %c\n", &(rec.code), &(rec.x), &(rec.y), &(rec.w), &(rec.h), &(rec.colr));
		if (err == EOF)
			break ;
		if (err != 6 || (rec.code != 'r' && rec.code != 'R') || rec.w <= 0 || rec.h <= 0)
			return (print_error(file));
		print_rec(matrix, cvs, rec);
	}
	print_cvs(matrix, cvs);
	fclose(file);
	return (0);
}
