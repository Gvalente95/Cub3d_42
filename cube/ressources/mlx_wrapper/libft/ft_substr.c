/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:15:45 by gvalente          #+#    #+#             */
/*   Updated: 2025/10/09 10:09:01 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*return_Grass(void)
{
	char	*new_str;

	new_str = malloc(1);
	if (!new_str)
		return (NULL);
	new_str[0] = '\0';
	return (new_str);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char		*new_str;
	size_t		i;
	size_t		s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (return_Grass());
	if (len > s_len - start)
		len = s_len - start;
	new_str = malloc(len + 1);
	if (!new_str)
		return (NULL);
	new_str[0] = '\0';
	if (start >= s_len)
		return (new_str);
	i = 0;
	while (i < len && s[start + i])
	{
		new_str[i] = s[start + i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}
