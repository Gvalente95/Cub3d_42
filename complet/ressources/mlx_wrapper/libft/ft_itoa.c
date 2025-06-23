/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:16:44 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/17 02:45:21 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

static int	ft_get_converted_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static char	*ft_handle_min(int n)
{
	int		len;
	char	*str;

	if (n == INT_MIN)
		return (ft_strdup("-2147483648"));
	len = ft_get_converted_len(n);
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	str[0] = '-';
	n = -n;
	str[len] = '\0';
	while (n)
	{
		str[--len] = (n % 10) + '0';
		n /= 10;
	}
	return (str);
}

char	*ft_itoa(int n)
{
	char	*res;
	int		len;

	if (n == 0)
		return (ft_strdup("0"));
	if (n < 0)
		return (ft_handle_min(n));
	len = ft_get_converted_len(n);
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	res[len] = '\0';
	while (n)
	{
		res[--len] = (n % 10) + '0';
		n /= 10;
	}
	return (res);
}
