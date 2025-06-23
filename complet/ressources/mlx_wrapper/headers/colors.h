/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 05:11:14 by gvalente          #+#    #+#             */
/*   Updated: 2025/04/04 12:04:48 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
# define COLORS_H

//COLORS
unsigned char	get_t(int trgb);
unsigned char	get_r(int trgb);
unsigned char	get_g(int trgb);
unsigned char	get_b(int trgb);

# define RGB_RED	0
# define RGB_GREEN	1
# define RGB_BLUE	2
# define RGB_CYAN	3
# define RGB_MAGENT	4
# define RGB_GRAY	5
# define RGB_BROWN	6
# define RGB_WHITE	7
# define RGB_TEAL	8
# define RGB_LIME	9
# define RGB_GOLD	10
# define RGB_SILVER	11
# define RGB_TURQ	12
# define RGB_INDIGO	13
# define RGB_VIOLET	14
# define RGB_CORAL	15
# define RGB_BLACK	16
# define RGB_YELLOW	17
# define RGB_ORANGE	18
# define RGB_NULL	19

# define PORTAL_START			0xFF111111  // Fully opaque black
# define PORTAL_ENT				0xFF222222  // Fully opaque black

# define _RED					0x00FF0000
# define _GREEN					0x0000FF00
# define _BLUE					0x000000FF
# define _CYAN					0x0000FFFF
# define _MAGENT				0x00FF00FF
# define _GRAY					0x00A9A9A9
# define _BROWN					0x008B4513
# define _TEAL					0x00008080
# define _LIME					0x0000FF00
# define _GOLD					0x00FFD700
# define _SILVER				0x00C0C0C0
# define _TURQ					0x0040E0D0
# define _INDIGO				0x004B0082
# define _VIOLET				0x00EE82EE
# define _CORAL					0x00FF8050
# define _WHITE					0x00FFFFFF
# define _BLACK					0x00000000
# define _YELLOW				0x00FFFF00
# define _ORANGE				0x00FFA500
# define _NULL					0x00000000

# define COLOR_TXT_RED				"\033[31m"
# define COLOR_TXT_RESET			"\033[0m"
# define COLOR_TXT_GREEN			"\033[32m"

//		var/colors.c
int				get_trgb(unsigned char t, unsigned char r, \
	unsigned char g, unsigned char b);
unsigned char	get_t(int trgb);
unsigned char	get_r(int trgb);
unsigned char	get_g(int trgb);
unsigned char	get_b(int trgb);

#endif
