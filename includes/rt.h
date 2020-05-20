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

# define RT_H

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

# define QUALITY			Q_LOW
# define TRANSP_F     0
# define WATER_ON     1
# define DFLT_POWER 100
# define NB_THREADS		8
# define MAX_VIEW 1000000

# define NN_CAUSTIC_PHOTON_MAX  20
# define NN_INDIRECT_PHOTON_MAX	20
# define NB_INDIRECT_PHOTON		100000
# define NB_CAUSTIC_PHOTON		100000
# define MAX_CAUSTIC_RADIUS		0.3
# define MAX_INDIRECT_RADIUS	0.5
# define PHOTON_DEPTH			10

/*
**    ALTERABLE MACRO
*/

# define RAY_DEPTH			6
# define PERLIN_TRANSP_ADD	1

# define FBM_LACUNARITY		2.
# define FBM_GAIN			0.5
# define FBM_ITER			2

# define FBM_AMPLITUDE		0.5
# define MOTION_STEP		0.02
# define MOTION_FVEL		8
# define MOTION_SPP			32

# define ROUGHCAST_LIMIT	12
# define BIAS				0.01

# include "./norme.h"

#endif
