/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:10:05 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/11 19:27:32 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static char	*get_map(char *file_name)
{
	int		fd;
	char	*line;
	char	*content;
	char	*buffer;

	fd = open(file_name, O_RDONLY, 0644);
	if (fd == -1)
		return (NULL);
	content = ft_strdup("");
	if (!content)
		return (close(fd), NULL);
	line = get_next_line(fd);
	while (line)
	{
		trim_excess_spaces(&line);
		buffer = ft_strjoin(content, line);
		free(content);
		free(line);
		if (!buffer)
			return (close(fd), NULL);
		content = buffer;
		line = get_next_line(fd);
	}
	close(fd);
	return (content);
}

static char	*get_resized_line(char *buffer, int width, char replace_end_with)
{
	char	*resized_line;
	int		i;

	resized_line = malloc(width + 1);
	if (!resized_line)
		return (printf("alloc in resized line failed"), NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n' && i < width - 1)
	{
		resized_line[i] = buffer[i];
		i++;
	}
	while (i < width - 1)
		resized_line[i++] = replace_end_with;
	resized_line[i++] = '\n';
	resized_line[i] = '\0';
	return (resized_line);
}

static char	*redimension_map(char *map_buffer, t_vec2 size)
{
	char	*tmp;
	char	*new_map;
	char	*new_line;
	char	*map_index;

	map_index = map_buffer;
	new_map = ft_strdup("");
	new_line = get_resized_line(map_buffer, size.x, ' ');
	while (contains(map_index, '\n'))
	{
		tmp = ft_strjoin(new_map, new_line);
		free(new_line);
		free(new_map);
		new_map = tmp;
		map_index = ft_strchr(map_index, '\n');
		if (map_index)
			map_index++;
		new_line = get_resized_line(map_index, size.x, ' ');
	}
	tmp = ft_strjoin(new_map, new_line);
	free(new_line);
	free(new_map);
	free(map_buffer);
	return (tmp);
}

static t_vec2	get_map_size(char *map)
{
	int		cur_width;
	t_vec2	size;
	int		i;

	cur_width = 0;
	size = v2(1, 0);
	i = -1;
	while (map[++i])
	{
		if (map[i] == '\n')
		{
			size.y++;
			if (cur_width > size.x)
				size.x = cur_width;
			cur_width = 1;
		}
		else
			cur_width++;
	}
	if (cur_width > size.x)
		size.x = cur_width;
	size.y++;
	return (size);
}

int	init_map(t_md *md, char *file_name)
{
	char	*extension_name;
	int		plr_index;

	extension_name = ft_strchr(file_name, '.');
	if (!extension_name || ft_strncmp(extension_name, ".cub", 4))
		free_and_quit(md, "Wrong file extension", extension_name);
	md->map.name = file_name;
	md->map.buffer = get_map(file_name);
	if (!md->map.buffer)
		free_and_quit(md, "map file not found", file_name);
	md->map.len = ft_strlen(md->map.buffer);
	init_map_data(md);
	md->map.size = get_map_size(md->map.buffer);
	md->map.buffer = redimension_map(md->map.buffer, md->map.size);
	md->map.len = ft_strlen(md->map.buffer);
	plr_index = get_to_find_index(md->map.buffer, "NSEW");
	if (plr_index < 0)
		free_and_quit(md, "No player", NULL);
	if (!md->map.accept_unvalid && find_breach(md, &md->map, plr_index))
		free_and_quit(md, "Unclosed map\n", NULL);
	if (!md->map.accept_unvalid && !validate_map(md, md->map.buffer))
		free_and_quit(md, NULL, NULL);
	printf("%s\n", md->map.buffer);
	md->map.size.x--;
	return (1);
}
