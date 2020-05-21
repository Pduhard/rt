/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/19 19:34:58 by aplat             #+#    #+#             */
/*   Updated: 2020/05/20 08:54:08 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define  RT_H

/*
**    Alterable macro
*/

# define NB_THREADS		8

/*
**    NB_THREADS determine number of threads
*/

# define QUALITY			Q_LOW

/*
**    QUALITY definition:
**
**    Q_VERY_LOW  subsampling
**    Q_LOW       subsampling when move then full sampling
**    Q_MED       full sampling
**    Q_HIGH      full sampling when move then full super sampling
*/

# define RAY_DEPTH			6

/*
**    RAY_DEPTH is the maximum number of recursively cast rays
*/

# define BIAS				0.0001

/*
**    BIAS is one way to avoid self ray intersection
*/

# define MAX_VIEW 1000000

/*
**    MAX_VIEW is the maximum distance between the camera and scene elements
*/

# define TRANSP_F     0

/*
**    TRANSP_F is multiplied by generated object's
**    transparency to determine new object transparency.
*/

# define WATER_ON     1

/*
**    WATER_ON determine whether or not scene is recalculate at each frame.
**    (principaly used for moving water texture)
*/


/*
**    Alterable macro for global illumination
*/

# define DFLT_POWER   100

/*
**    DFLT_POWER is used to determine light
**    intensity. (For global illumination)
*/


# define NB_INDIRECT_PHOTON		  100000
# define NB_CAUSTIC_PHOTON		  100000


/*
**    NB_INDIRECT_PHOTON and NB_CAUSTIC_PHOTON
**    determine how many photon will be stored.
**    Respectively for indirect and caustic photon map.
*/

# define NN_CAUSTIC_PHOTON_MAX  20
# define NN_INDIRECT_PHOTON_MAX 20

/*
**    NN_CAUSTIC_PHOTON_MAX and NN_INDIRECT_PHOTON_MAX
**    determine how many photon will be factor in
**    radiance estimation computation.
*/

# define PHOTON_DEPTH           10

/*
**    PHOTON_DEPTH is the maximum number of recursively cast photon rays
*/

# define PERLIN_TRANSP_ADD	1

/*
**    PERLIN_TRANSP_ADD modify a parlin textured object's transparency
*/

# define FBM_LACUNARITY		2.
# define FBM_GAIN			    0.5
# define FBM_ITER         2
# define FBM_AMPLITUDE    0.5

/*
**    FBM_LACUNARITY, FBM_GAIN, FBM_ITER and FBM_AMPLITUDE
**    are used to compute fractal brownian motion
**    factor
*/

# define ROUGHCAST_LIMIT	12

/*
**    ROUGHCAST_LIMIT determine the number of iteration for marble texture
*/

# define MOTION_STEP    0.02

/*
**    MOTION_STEP determine speed of motion blur effect
*/

# define MOTION_FVEL		8

/*
**   MOTION_FVEL determine the velocity factor
**   of latest sample of motion blue effect
*/

# define MOTION_SPP			32

/*
**    MOTION_SPP determine the maximum amount of sample for motion blur effect
**
*/

# include "../libft/libft.h"
# include "./key_linux.h"
# if defined(__linux)
#  include "../external_libs/minilibx/mlx.h"
#  include "/usr/include/SDL2/SDL.h"
#  include "/usr/include/SDL2/SDL_image.h"
# else
#  include "../external_libs/minilibx_macos/mlx.h"
#  include "../external_libs/sdl/SDL.h"
#  include "../frameworks/SDL2.framework/Headers/SDL.h"
#  include "../frameworks/SDL2_image.framework/Headers/SDL_image.h"
# endif

# include <math.h>
# include <pthread.h>
# include <fcntl.h>
# include <float.h>
# include <time.h>
# include <stdio.h>
# include "rt_env.h"
# include "./norme.h"

#endif
