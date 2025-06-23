/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_text.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:36:29 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 09:50:52 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

char	get_input_char(t_md *md, int key)
{
	const char	lookup_chars[] = {\
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', \
		'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', \
		'q', 'r', 's', 't', 'u', 'v', 'w', 'x', \
		'y', 'z', ' ', '1', '2', '3', '4', \
		'5', '6', '7', '8', '9', '0'
	};
	int			i;

	i = -1;
	while (++i < 34)
	{
		if (md->input_to_char[i].x == key)
			return (lookup_chars[md->input_to_char[i].y]);
	}
	return ('\0');
}

static char	*get_font_path(t_md *md, const char *base_path, char c)
{
	char	*path;
	char	*letter;

	if (c == '.')
		letter = md_strdup(md, "dot");
	else if (c == ':')
		letter = md_strdup(md, "db_dot");
	else if (c == '/')
		letter = md_strdup(md, "bar");
	else
	{
		letter = md_malloc(md, 2);
		letter[0] = ft_toupper(c);
		letter[1] = '\0';
	}
	path = ft_megajoin(base_path, letter, ".xpm", NULL);
	free(letter);
	return (path);
}

static void	init_input_to_char(t_md *md)
{
	const int	inputs[] = {
		A_KEY, B_KEY, C_KEY, D_KEY, E_KEY, F_KEY, G_KEY, H_KEY,
		I_KEY, J_KEY, K_KEY, L_KEY, M_KEY, N_KEY, O_KEY, P_KEY,
		Q_KEY, R_KEY, S_KEY, T_KEY, U_KEY, V_KEY, W_KEY, X_KEY,
		Y_KEY, Z_KEY, ' ', _1_KEY, _2_KEY, _3_KEY, _4_KEY,
		_5_KEY, _6_KEY, _7_KEY, _8_KEY, _9_KEY, _0_KEY
	};
	int			i;

	i = -1;
	while (++i < (int)(sizeof(inputs) / sizeof(inputs[0])))
		md->input_to_char[i] = (t_vec2){inputs[i], i};
}

void	init_fonts(t_md *md)
{
	const char	*base_path = "utils/font/";
	char		*path;
	t_image		*img;
	int			c;

	init_input_to_char(md);
	c = 31;
	while (++c < 126)
	{
		if (char_in_str((char)c, "$&*;=\\^`{|}"))
			path = get_font_path(md, base_path, '?');
		else
			path = get_font_path(md, base_path, (char)c);
		img = init_img(md, _v2(md->prm.txt_sc), path, -1);
		clean_img(img);
		md->txd.font[(unsigned char)c] = img;
		free(path);
	}
}
