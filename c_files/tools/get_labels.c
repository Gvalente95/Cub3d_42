/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_labels2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 11:36:20 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/10/16 11:36:29 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

const char* get_weapon_name(t_md* md, t_weapon_types type) {
	return (md->txd.weapons_names[type]);
}
const char* get_item_name(t_md* md, t_pckp_types type) {
	return (md->txd.item_names[type]);
}
const char	*get_mob_name(t_md *md, t_mob_types type) {
	return (md->txd.mob_names[type]);
}
const char* get_ent_name(t_md* md, t_ent_type type) {
	return (md->txd.ents_types_names[type]);
}