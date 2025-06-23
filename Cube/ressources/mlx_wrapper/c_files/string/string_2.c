/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:10:03 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/29 12:39:35 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

void	setstr(char **str, char *new)
{
	safe_free((void *)*str);
	*str = new;
}

int	only_contains(char *str, char *to_contain)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (!char_in_str(str[i], to_contain))
			return (0);
	}
	return (1);
}

char	*ft_megajoin(const char *a, const char *b, const char *c, const char *d)
{
	char	*ab;
	char	*abc;
	char	*abcd;

	if (!a || !b)
		return (NULL);
	ab = ft_strjoin(a, b);
	if (!ab)
		return (NULL);
	if (!c)
		return (ab);
	abc = ft_strjoin(ab, c);
	free(ab);
	if (!abc)
		return (NULL);
	if (!d)
		return (abc);
	abcd = ft_strjoin(abc, d);
	free(abc);
	if (!abcd)
		return (NULL);
	return (abcd);
}

int	get_char_index(const char *str, char to_check)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] == to_check)
			return (i);
	return (-1);
}

char	*ft_strndup(const char	*s1, ssize_t n)
{
	char		*dst;
	ssize_t		i;
	ssize_t		len;

	if (!s1 || n < 0)
		return (NULL);
	len = ft_strlen(s1);
	if (n > len)
		n = len;
	dst = (char *)malloc(n + 1);
	if (!dst)
		return (printf("alloc for %s in ft_strdnup\n", s1), NULL);
	i = 0;
	while (i < n)
	{
		dst[i] = s1[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
