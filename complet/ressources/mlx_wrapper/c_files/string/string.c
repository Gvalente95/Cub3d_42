/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:13:43 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/29 12:39:35 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

char	*md_strjoin(t_md *d, char const *s1, char const *s2)
{
	int		i;
	int		u;
	char	*joined;

	if (!s1)
		return (NULL);
	if (!s2)
		return (md_strdup(d, s1));
	joined = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!joined)
		free_and_quit(d, "alloc failed in strjoin", s1);
	i = -1;
	while (s1[++i])
		joined[i] = s1[i];
	u = -1;
	while (s2[++u])
		joined[i + u] = s2[u];
	joined[i + u] = '\0';
	return (joined);
}

char	*md_strdup(t_md *d, const char	*s1)
{
	char	*dst;
	int		i;
	int		len;

	len = ft_strlen(s1);
	dst = (char *)malloc(len + 1);
	if (!dst)
	{
		printf("%s > ", s1);
		free_and_quit(d, "alloc failed in strdup", s1);
	}
	i = 0;
	while (i < len)
	{
		dst[i] = s1[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

void	*md_malloc(t_md *d, ssize_t size)
{
	void	*content;

	content = malloc(size);
	if (!content)
		free_and_quit(d, "malloc failed", NULL);
	return (content);
}

void	*md_realloc(t_md *d, void *ptr, size_t new_size)
{
	void	*new_ptr;

	if (!ptr)
		return (md_malloc(d, new_size));
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = md_malloc(d, new_size);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, new_size);
	free(ptr);
	return (new_ptr);
}

char	*md_strndup(t_md *d, const char	*s1, ssize_t n)
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
		free_and_quit(d, "strndup alloc fail", s1);
	i = 0;
	while (i < n)
	{
		dst[i] = s1[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
