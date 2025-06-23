/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map_data.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 09:55:04 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/06/11 19:42:58 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static char	*extract_line(const char *str)
{
	int		i;
	int		j;
	char	*line;

	i = 0;
	if (!str || !str[0])
		return (NULL);
	while (str[i] && str[i] != '\n')
		i++;
	line = malloc(i + 1);
	if (!line)
		return (NULL);
	j = -1;
	while (++j < i && str[j] != '\n')
		line[j] = str[j];
	line[i] = '\0';
	return (line);
}

static void	add_texture_img(t_md *md, char *line, t_wrd_dir dir, int flip_x)
{
	int				fd;
	t_texture_data	*txd;

	txd = &md->txd;
	while (*line == ' ')
		(*line)++;
	if (ft_strlen(line) <= 4)
		free_and_quit(md, "wrong value for txtr", line);
	fd = open(line + 3, O_RDONLY);
	if (fd == -1)
		free_and_quit(md, "file at path not found for txtr", line + 3);
	close(fd);
	while (*line == ' ')
		line++;
	txd->wall_img[dir] = \
		init_abs_img(md, _v2(txd->e_scales[nt_wall]), line + 3);
	if (flip_x)
		flipx_image_data(txd->wall_img[dir]);
	fill_transparency(txd->wall_img[dir], _BLACK);
	if (md->prm.debug_mode)
		printf("%s texture[%d] correctly set\n", line + 3, dir);
}

static int	parse_file_line(char *line, t_md *md)
{
	while (*line == ' ')
		line++;
	if (!*line)
		return (1);
	if (!ft_strncmp(line, "!", 2))
		md->map.accept_unvalid = 1;
	else if (!ft_strncmp(line, "NO ", 3))
		add_texture_img(md, line, NORTH, 0);
	else if (!ft_strncmp(line, "SO ", 3))
		add_texture_img(md, line, SOUTH, 1);
	else if (!ft_strncmp(line, "EA ", 3))
		add_texture_img(md, line, EAST, 0);
	else if (!ft_strncmp(line, "WE ", 3))
		add_texture_img(md, line, WEST, 1);
	else if (line[0] == 'C')
		md->hud.ceiling_color = str_to_color(line + 2);
	else if (line[0] == 'F')
		md->hud.floor_color = str_to_color(line + 2);
	else if (line[0] == 'B')
		md->hud.fog_color = str_to_color(line + 2);
	else if (contains_valid_character(line, md->txd.ents_tp_map[0]))
		return (0);
	return (1);
}

static char	*parse_file_data(t_md *md)
{
	char	*file_content;
	char	*line;

	file_content = md->map.buffer;
	line = extract_line(file_content);
	while (line)
	{
		if (!parse_file_line(line, md))
			break ;
		file_content = ft_strchr(file_content, '\n');
		if (file_content)
			file_content++;
		else
			break ;
		setstr(&line, extract_line(file_content));
	}
	print_color(md->hud.ceiling_color, "Sky color");
	print_color(md->hud.floor_color, "Floor color");
	return (safe_free(line), ft_strdup(file_content));
}

void	init_map_data(t_md *md)
{
	int		i;
	t_image	*img;

	md->txd.wall_img = md_malloc(md, sizeof(t_image *) * 5);
	md->txd.wall_img2d = md_malloc(md, sizeof(t_image *) * 5);
	i = -1;
	while (++i < 5)
	{
		md->txd.wall_img[i] = NULL;
		md->txd.wall_img2d[i] = NULL;
	}
	setstr(&md->map.buffer, parse_file_data(md));
	if (!md->map.buffer)
		free_and_quit(md, "no map found", NULL);
	if (trim_excess_newlines(&md->map.buffer, ft_strlen(md->map.buffer)))
		printf("Excess characters found\n");
	i = -1;
	while (++i < 4)
	{
		img = md->txd.wall_img[i];
		if (!img)
			free_and_quit(md, "Wall texture's path missing", NULL);
		md->txd.wall_img2d[i] = copy_image(md, img, _v2(md->txd.size_2d), -1);
	}
}
