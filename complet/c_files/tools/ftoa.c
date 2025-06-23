/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftoa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 00:10:48 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/15 02:40:09 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

char	*frac_ftoa(char *int_part_str, char *result, int dec_len, int precision)
{
	int	i;
	int	j;
	int	had_dec;

	i = 0;
	j = 0;
	had_dec = dec_len > 0;
	while (dec_len-- > 0)
		result[i++] = int_part_str[j++];
	if (!int_part_str[i] || precision == 0)
		return (result[i] = '\0', result);
	if (had_dec)
		result[i++] = '.';
	while (int_part_str[j] && precision)
		result[i++] = int_part_str[j++];
	result[i] = '\0';
	return (result);
}

char	*get_int_part_str(int *dec_len, float num)
{
	int		scaled_num;

	scaled_num = num;
	*dec_len = 0;
	while (scaled_num != (int)scaled_num)
	{
		(*dec_len)++;
		scaled_num *= 10;
	}
	return (ft_itoa(scaled_num));
}

char	*ftoa(float num, int precision)
{
	int		is_negative;
	char	*int_part_str;
	int		len;
	char	*result;
	int		dec_len;

	is_negative = 0;
	if (num < 0)
	{
		is_negative = 1;
		num = -num;
	}
	int_part_str = get_int_part_str(&dec_len, num);
	len = 0;
	while (int_part_str[len])
		len++;
	len += is_negative + precision + 1;
	result = malloc(len + 1);
	return (frac_ftoa(int_part_str, result, dec_len, precision));
}
