/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 06:30:21 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/26 20:51:48 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

char	*get_img_path(char c)
{
	char	*path;
	char	*txt;

	c = ft_toupper(c);
	if (c == '.')
		txt = ft_strdup("dot");
	else if (c == ':')
		txt = ft_strdup("db_dot");
	else if (c == '/')
		txt = ft_strdup("bar");
	else
	{
		txt = malloc(2);
		txt[0] = c;
		txt[1] = '\0';
	}
	path = ft_megajoin("utils/font/", txt, ".xpm", NULL);
	free(txt);
	return (path);
}

int	display_letter(t_md *md, char c, t_txtd data)
{
	t_image	*l;
	char	*path;

	path = get_img_path(c);
	if (!path)
		return (printf("font not found: %c\n", c), 0);
	l = init_img(md, _v2(data.scale), path, -1);
	if (!l || !l->img)
		return (printf("Error\nImg letter not found %s", path), data.scale);
	if (data.color != -1)
		flush_img(l, data.color, get_alpha(data.color), 1);
	free(path);
	if (l->img)
	{
		draw_img(l, data.onto, v2(data.x, data.y), -1);
		free_image_data(md, l);
	}
	return (data.scale);
}

static int	display_text(t_md *md, char *text, t_txtd data)
{
	int			i;
	t_txtd		cur_pos;
	int			total_width;

	i = -1;
	cur_pos = data;
	total_width = 0;
	while (text[++i])
	{
		if (text[i] == '\n')
		{
			cur_pos.x = data.x;
			cur_pos.y += data.scale * 2 + 10;
			continue ;
		}
		if (text[i] == '	')
		{
			cur_pos.x += data.scale * 2;
			continue ;
		}
		cur_pos.x += display_letter(md, text[i], cur_pos);
		total_width += md->prm.txt_sc * 1.5;
	}
	return (total_width);
}

//	DATA = (x pos, y pos, text color, text scale) return: text width
int	render_text(t_md *md, t_txtd data, const char *format, ...)
{
	char	buff[256];
	va_list	args;
	int		txt_width;

	va_start(args, format);
	vsnprintf(buff, sizeof(buff), format, args);
	va_end(args);
	if (!data.onto && md->menu.active && md->menu.ui_overlay)
		data.onto = md->menu.ui_overlay;
	if (!data.onto)
		data.onto = md->screen;
	txt_width = display_text(md, buff, data);
	return (txt_width);
}

int	rnd_txt_simple(t_md *md, t_vec2 pos, const char *format, ...)
{
	char	buff[256];
	va_list	args;
	int		txt_width;
	t_txtd	data;

	if (!format[0])
		return (0);
	va_start(args, format);
	vsnprintf(buff, sizeof(buff), format, args);
	va_end(args);
	data.x = pos.x;
	data.y = pos.y;
	data.color = -1;
	data.onto = md->screen;
	data.scale = md->prm.txt_sc;
	txt_width = display_text(md, buff, data);
	return (txt_width);
}
