/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:21:44 by gvalente          #+#    #+#             */
/*   Updated: 2025/04/21 17:00:27 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

int	r_range_seed(unsigned int *g_seed, int min, int max)
{
	if (min > max)
		return (-1);
	*g_seed = (*g_seed * 1103515245 + 12345) & 0x7FFFFFFF;
	return (min + (*g_seed % (max - min + 1)));
}

int	r_range(int min, int max)
{
	if (min > max)
		return (-1);
	return (min + rand() % (max - min + 1));
}

float	f_range(float min, float max)
{
	float	scale;

	if (min > max)
		return (-1.0f);
	scale = rand() / (float)RAND_MAX;
	return (min + scale * (max - min));
}

int	wrap_int(int *number, int min, int max, int increment)
{
	if (!increment)
		return (0);
	*number += increment;
	if (*number > max)
		*number = min;
	else if (*number < min)
		*number = max;
	return (1);
}

unsigned int	get_r_seed(void)
{
	int				fd;
	unsigned int	seed;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0 || read(fd, &seed, sizeof(seed)) != sizeof(seed))
		seed = (unsigned int)time(NULL);
	close(fd);
	return (seed);
}
