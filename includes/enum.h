/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enum.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/23 21:40:41 by aplat             #+#    #+#             */
/*   Updated: 2020/05/23 21:40:43 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUM_H
# define ENUM_H

/*
**  enums
*/

typedef	enum {
	OBJ_SPHERE,
	OBJ_PLANE,
	OBJ_CONE,
	OBJ_CYLINDER,
	OBJ_TRIANGLE,
	OBJ_ELLIPSOID,
	OBJ_HYPERBOLOID,
	OBJ_HORSE_SADDLE,
	OBJ_MONKEY_SADDLE,
	OBJ_CYCLIDE,
	OBJ_FERMAT,
	OBJ_MOEBIUS,
	OBJ_SKYBOX
}	t_obj_type;

typedef	enum {
	MAT_DIFFUSE,
	MAT_NEGATIVE
}	t_mat_type;

typedef	enum {
	LIGHT_POINT,
	LIGHT_AMBIENT,
	LIGHT_DIRECTIONAL
}	t_light_type;

typedef	enum {
	TEXT_PERLIN,
	TEXT_MARBLE,
	TEXT_WOOD,
	TEXT_FBM,
	TEXT_UNI,
	TEXT_GRID,
	TEXT_IMAGE
}	t_text_type;

typedef enum {
	BUMP_PERLIN,
	BUMP_MARBLE,
	BUMP_WOOD,
	BUMP_FBM,
	BUMP_WATER
}	t_bump_type;

typedef enum {
	CUT_STATIC,
	CUT_REAL,
	CUT_SPHERE,
	CUT_TEXTURE,
	CUT_UV
}	t_cut_type;

#endif
