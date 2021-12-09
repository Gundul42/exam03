/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_paint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: graja <graja@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 11:40:00 by graja             #+#    #+#             */
/*   Updated: 2021/12/08 13:03:19 by graja            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef	struct	s_cvs
{
	int	width;
	int	height;
	char	back;
}		t_cvs;

typedef	struct	s_circle
{
	char	code;
	float	x;
	float	y;
	float	rad;
	char	colr;
}		t_circle;

static
int	print_err(FILE *file)
{
	write(1, "Error: Operation file corrupted\n", 32);
	if (file)
		fclose(file);
	return (1);
}

static
int	check_circle(float x, float y, t_circle ccl, int flag)
{
	float	newr;

	newr = powf((powf(x - ccl.x, 2.0) + powf(y - ccl.y, 2)), 0.5);
	if (flag && newr <= ccl.rad)
		return (1);
	if (!flag && (newr - ccl.rad) < 1.0 && (newr - ccl.rad) >= 0.0)
		return (1);
	return (0);
}

static
void	draw_circle(char **matrix, t_cvs cvs, t_circle ccl)
{
	int	fflag;
	int	x;
	int	y;

	if (ccl.code == 'C')
		fflag = 1;
	else
		fflag = 0;
	y = 0;
	while (y < cvs.height)
	{
		x = 0;
		while (x < cvs.width)
		{
			if (check_circle(x, y, ccl, fflag))
				matrix[y][x] = ccl.colr;
			x++;
		}
		y++;
	}
}

static
void	print_cvs(char **matrix, t_cvs cvs)
{
	int	y;

	y = 0;
	while (y < cvs.height)
	{
		write(1, matrix[y], cvs.width);
		write(1, "\n", 1);
		free(matrix[y]);
		y++;
	}
	free(matrix);
}

int	main(int argc, char **argv)
{
	t_cvs		cvs;
	t_circle	ccl;
	int		i;
	int		err;
	FILE		*file;
	char		**matrix;

	if (argc != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	file = fopen(argv[1], "r");
	if (!file)
		return(print_err(file));
	err = fscanf(file, "%d %d %c\n", &(cvs.width), &(cvs.height), &(cvs.back));
	if (err != 3 || cvs.width < 1 || cvs.width > 300 || cvs.height < 1 || cvs.height > 300)
		return (print_err(file));
	i = 0;
	matrix = malloc(cvs.height * sizeof(char *));
	if (!matrix)
		return (1);
	while (i < cvs.height)
	{
		matrix[i] = calloc(cvs.width, sizeof(char));
		if (!matrix[i])
			return (1);
		memset(matrix[i], cvs.back, cvs.width);
		i++;
	}
	err = 5;
	while (err == 5)
	{
		err = fscanf(file, "%c %f %f %f %c\n", &(ccl.code), &(ccl.x), &(ccl.y),
				&(ccl.rad), &(ccl.colr));
		if (err == EOF)
			break ;
		if (err != 5 || (ccl.code != 'c' && ccl.code != 'C') || ccl.rad <= 0)
			return (print_err(file));
		draw_circle(matrix, cvs, ccl);
	}
	print_cvs(matrix, cvs);
	fclose(file);
	return (0);
}
