/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:38:47 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/21 03:41:34 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "limits.h"

static int	ft_isspace(char c)
{
	if (c == ' ' || c == '\n' || c == '\t')
		return (1);
	if (c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

static int	set_variables(int *res, int *i, int *sign, const char *str)
{
	if (!str)
		return (0);
	*res = 0;
	*i = 0;
	*sign = 1;
	return (1);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	res;

	if (!set_variables(&res, &i, &sign, str))
		return (0);
	while (ft_isspace(str[i]))
		i++;
	if ((str[i] == '-' || str[i] == '+') && str[i++] == '-')
		sign = -1;
	if (!ft_isdigit(str[i]))
		return (0);
	while (ft_isdigit(str[i]))
	{
		if (res > (INT_MAX - str[i] - '0') / 10 && sign)
		{
			if (sign)
				return (INT_MAX);
			return (INT_MIN);
		}
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (res * sign);
}
