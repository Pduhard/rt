/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_env.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/20 07:43:03 by aplat             #+#    #+#             */
/*   Updated: 2020/05/20 09:01:34 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_ENV_H
# define RT_ENV_H

/*
**    Anti Aliasing relative macro
*/

# define Q_VERY_LOW	1
# define Q_LOW		2
# define Q_MED    	4
# define Q_HIGH 	8

# define MIN_AA 	0.5
# define NO_AA      1.
# define MAX_AA 	2.

/*
**    Global Illumination relative macro
*/

# define SPEC_PROB	0.35
# define DIFF_PROB	0.65

/*
**    Math relative macro
*/

# define _M_PI_180  0.01745329251
# define _SQRT_2    1.41421356237

/*
**    Cel shading relative macro
*/

# define CEL_BOUND_1	0.05
# define CEL_BOUND_2	0.15
# define CEL_BOUND_3	0.3
# define CEL_BOUND_4	0.60
# define CEL_BOUND_6	0.90

# define CEL_BOUNDARY	0.04

/*
**    Key Hooks relative macro
*/

# define A_HOOK			1
# define D_HOOK			2
# define W_HOOK			4
# define S_HOOK			8
# define F_HOOK			16
# define G_HOOK			32

# define ARR_LEFT_HOOK	64
# define ARR_RIGHT_HOOK 128
# define ARR_DOWN_HOOK	256
# define ARR_UP_HOOK	512

# define SPACE_HOOK		1024
# define SHIFT_HOOK		2048

/*
**    Conf Mess
*/

# define CAM "<camera\n"
# define CYLINDER "<cylinder\n\t<origin (x, y, z)>\n"
# define SPHERE "<sphere\n\t<origin (x, y, z)>\n"
# define PLANE "<plane\n\t<origin (x, y, z)>\n"
# define CONE "<cone\n\t<origin (x, y, z)>\n"
# define TRIANGLE "<triangle\n\t<a (x, y, z)>\n"
# define TRIBC "\t<b (x, y, z)>\n\t<c (x, y, z)>\n"
# define MOEBIUS "<moebius\n\t<origin (x, y, z)>\n"
# define HORSE "<horse_saddle\n\t<origin (x, y, z)>\n"
# define HORSEF "\t<x_fact (X)>\n\t<y_fact (X)>\n"
# define MONKEY "<monkey_saddle\n"
# define ELLIPSE "<ellipsoid\n\t<origin (x, y, z)>\n"
# define HYPERBOL "<hyperboloid\n\t<origin (x, y, z)>\n"
# define SURFACE "\t<surface (Value)> (optional)\n"
# define CYCLIDE "<cyclide\n"
# define FERMAT "<fermat\n"
# define XYFACT "\t<x_fact (Value)>\n\t<y_fact (Value)>\n"
# define COMPOSED "<objects\n\t<Name_object\n\t\t<origin (x, y, z)>\n\t"
# define PARAM "\t<param (Value)>\n"
# define ORIGIN "\t<origin (x, y, z)>\n"
# define CENTER "\t<center (x, y, z)>\n"
# define ROTATION "\t<rotation (x, y)>\n"
# define TIP "\t<tip (x, y)>\n"
# define RADIUS "\t<radius (X)>\n"
# define NAME "<name(scene_name)>\n"
# define SIZE "<size (WD,HH)>\n"
# define NORMAL "\t<normal (x, y ,z)>\n"
# define XAXIS "\t<xaxis (x, y, z)>\n"
# define AXIS "\t<x_axis (x,y,z)>\n\t<y_axis (x,y,z)>\n\t<z_axis (x,y,z)>\n"
# define HALFWIDTH "\t<half_width (X)>\n"
# define LSTCOLOR "SEPIA\n"
# define TEXT "<texture\n"
# define IMG "\t<IMAGE (path)>\n"
# define OFFSCALE "\t<offset (x, y)>\n\t<scale (x, y)>\n"
# define TEXTPROC "\t<name_text\n\t\t<color(r, g, b, a)> (*1-3)\n\t>\n"
# define CUTTEXTURE "Cutting texture don't need parameter\n"
# define SYNCUT "<cutting\n"
# define SPHERECUT "\t<sphere\n\t\t...\n\t>\t\n"
# define CUBECUT "\t<cube\n\t\t<x_range (a, b)>\n\t\t<y_range (a, b)>\n"
# define ZRANGE "\t\t<z_range (a,b)>\n\t>\n"
# define STATICCUT "\t<static\n\t\t<origin(x,y,z)>\n\t\t<normal(x,y,z)>\n\t>\n"
# define LIGHT "<lights\n"
# define AMBIENT "\t<ambient (r, g, b)>\n"
# define DIRECTIONAL "\t<directional (x, y, z)>\n\t<color (r, g, b)>\n"
# define POINT "\t<point (x, y, z)>\n\t<color (r, g, b)>\n"

/*
**     Error Mess
*/

# define ERRORMEM "internal error: too big allocation (allocate %zu bytes)\n"
# define ERRORSIZE "WIN_Size : Min 400/400, Max 2560/1420\n"
# define ERRORARG  "Usage: ./rtv1 NameFile.rt_conf\n"
# define ERRORTHREAD "Number Thread : Min 1, Max 16\n"
# define ERRORFILE "Invalid File\n"
# define ERROREMPTY "File error : empty\n"
# define ERRORSTRIPE "File error : stripe\n"
# define ERRORSCENE "File error : rt_conf start by <scene...\n"
# define ERRORON "ON OFF possible value: ON/1, OFF/0\n"
# define ERRORCOLOR "Color filter possible value: "
# define ERRORMATERIAL "Unknow material type\n"
# define EVALUESCENE "Error value Scene elements\n"
# define EVALUELIGHT "Error value Light elements\n"
# define UNKNOWSCENE "Unrecognized Scene Element\n"
# define UNKNOWOBJECT "Unrecognized Object Element\n"
# define UNKNOWTEXT "Unrecognized Texture Element\n"
# define UNKNOWCUT "Unrecognized element in Cutting\n"
# define UNKNOWLIGHT "Unrecognized element in Light\n"
# define ALREADYCAM "File error : Camera already exist\n"
# define ALREADYOBJ "Object already declared\n"
# define ALREADYTEXTURE "Texture already exist for this object\n"
# define ALREADYCUT "Cut parameter already exist\n"
# define SERROR "Syntax or Values error :\n"
# define ERRORCAM "No camera in file .rt_conf\n"
# define SERRORLIGHT "Syntax error: light(type)(origin)(intensity)\n"
# define SERRORCYL "<cylinder\n\t<origin (x,y,z)>\n\t(tip)(radius)(color)\n"
# define SERRORSPHERE "Syntax error: sphere(origin)(radius)(color)\n"
# define SERRORPLANE "Syntax error: plane(origin)(normal)(color)\n"
# define SERRORCONE "Syntax error: cone(center)(tip)(radius)(color)\n"
# define BUMPINDE "<BumpMapping\n\t<independent (Type)(BumpFact)>\n"
# define BUMPOWN "<BumpMapping\n\t<own (BumpFact)>\n"
# define MOTION "<MotionBlur\n\t<dir (x, y, z)>\n\t<speed (SpeedFact)>\n\t"

/*
**     Infos Mess
*/

# define ESC "ESC          ==> Quit Program"
# define WS "W / S        ==> Forward / Backward"
# define AD "A / D        ==> Left / Right"
# define UPDOWN "UP / DOWN    ==> Rotate Verticaly"
# define LEFTRIGHT "LEFT / RIGHT ==> Rotate Horizontaly"
# define WASD "W, A, S, D   ==> Translate Object"
# define LEFTCLICK "LEFT CLICK   ==> Unselect Object"

#endif
