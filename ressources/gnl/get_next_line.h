/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:14:16 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/12 13:21:18 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include "stdlib.h"
# include "unistd.h"
# include "stdio.h"

# define MAXSIZE 99999
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 30
# endif

typedef struct s_t_data
{
	char	buffer[MAXSIZE];
	char	leftover[MAXSIZE];
}	t_gnl_data;

char	*get_next_line(int fd);
int		contains(const char *s, char c);
char	*ft_strdup(const char *s);
int		update_fd(t_gnl_data *d, int fd, int index, ssize_t b_read);
char	*handle_leftover(t_gnl_data *d);

#endif