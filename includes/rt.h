/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/19 19:34:58 by aplat             #+#    #+#             */
/*   Updated: 2020/05/19 20:40:02 by aplat            ###   ########lyon.fr   */
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
#else
# include "../external_libs/minilibx_macos/mlx.h"
# include "../external_libs/sdl/SDL.h"
# include "../frameworks/SDL2.framework/Headers/SDL.h"
# include "../frameworks/SDL2_image.framework/Headers/SDL_image.h"
#endif

# include <math.h>
# include <pthread.h>
# include <fcntl.h>
# include <float.h>
# include <time.h>
# include <stdio.h>

//# define WIN_WIDTH 600
//# define WIN_HEIGHT	600
# define Q_VERY_LOW		1 // subsampling
# define Q_LOW				2 // subsampling when move (1/4) then no aa
# define Q_MED    		4 // no aa
# define Q_HIGH 			8 // no aa when move then aa x4

# define QUALITY			Q_LOW
# define TRANSP_F     0 // transp (color.val[3]) *= TRANSP_F
# define WATER_ON     1
# define DFLT_POWER 100
# define NB_THREADS		1
# define MIN_AA 		0.5
# define NO_AA      1.
# define MAX_AA 		2.
# define MAX_VIEW 1000000

/* TMP MACRO  */

//# define INDIRECT_GI			0
//# define CAUSTIC_GI				0
//# define GLOBAL_ILLUMINATION	0
//# define GL_RADIUS				0.2
//# define NB_PHOTON				100000
# define NN_CAUSTIC_PHOTON_MAX  20
# define NN_INDIRECT_PHOTON_MAX	20
# define SPEC_PROB				0.35
# define DIFF_PROB				0.65
# define NB_INDIRECT_PHOTON		100000
# define NB_CAUSTIC_PHOTON		100000
# define MAX_CAUSTIC_RADIUS		0.3
# define MAX_INDIRECT_RADIUS	0.5
# define PHOTON_DEPTH			10

/*ALTERABLE MACRO	*/

# define RAY_DEPTH			6
# define PERLIN_TRANSP_ADD	1

# define FBM_LACUNARITY		2.
# define FBM_GAIN			0.5
# define FBM_ITER			2

# define FBM_AMPLITUDE		0.5
# define MOTION_STEP		0.02
# define MOTION_FVEL		8
# define MOTION_SPP			32

/* CST MACROS */
# define _M_PI_180 0.01745329251
# define _SQRT_2 1.41421356237

# define CEL_BOUND_1	0.05
# define CEL_BOUND_2	0.15
# define CEL_BOUND_3	0.3
# define CEL_BOUND_4	0.60
# define CEL_BOUND_6	0.90

# define CEL_BOUNDARY	0.04
# define ROUGHCAST_LIMIT	12
# define BIAS				0.01

/* HOOKS MACRO */
# define A_HOOK			0b1
# define D_HOOK			0b10
# define W_HOOK			0b100
# define S_HOOK			0b1000
# define F_HOOK			0b10000
# define G_HOOK			0b100000

# define ARR_LEFT_HOOK	0b1000000
# define ARR_RIGHT_HOOK 0b10000000
# define ARR_DOWN_HOOK	0b100000000
# define ARR_UP_HOOK	0b1000000000

# define SPACE_HOOK		0b10000000000
# define SHIFT_HOOK		0b100000000000

/* Conf Mess */

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

/* Error Mess */
# define ERRORMEM "internal error: too big allocation (try to allocate %zu bytes)\n"
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

/* Infos Mess */
# define ESC "ESC          ==> Quit Program"
# define WS "W / S        ==> Forward / Backward"
# define AD "A / D        ==> Left / Right"
# define UPDOWN "UP / DOWN    ==> Rotate Verticaly"
# define LEFTRIGHT "LEFT / RIGHT ==> Rotate Horizontaly"
# define WASD "W, A, S, D   ==> Translate Object"
# define LEFTCLICK "LEFT CLICK   ==> Unselect Object"

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
}		t_obj_type;

typedef	enum {
		MAT_DIFFUSE,
		MAT_NEGATIVE
}		t_mat_type;

typedef	enum {
		LIGHT_POINT,
		LIGHT_AMBIENT,
		LIGHT_DIRECTIONAL
}		t_light_type;

typedef	enum {
		TEXT_PERLIN,
		TEXT_MARBLE,
		TEXT_WOOD,
		TEXT_FBM,
		TEXT_UNI,
		TEXT_GRID,
		TEXT_IMAGE
}		t_text_type;

typedef enum {
		BUMP_PERLIN,
		BUMP_MARBLE,
		BUMP_WOOD,
		BUMP_FBM,
		BUMP_WATER
}		t_bump_type;

typedef enum {
		CUT_STATIC,
		CUT_REAL,
		CUT_SPHERE,
		CUT_TEXTURE,
		CUT_UV
}		t_cut_type;

typedef struct	s_mlx
{
	void		*mlx_ptr;
	void		*win_ptr;
	void		*img_ptr;
	int			*img_str;
	int			bpp;
	int			s_l;
	int			endian;
}				t_mlx;

typedef	struct	s_pixel
{
	int			i;
	int			j;
}				t_pixel;

typedef struct	s_44matf
{
	double		val[4][4];
}				t_44matf;

typedef struct	s_33matf
{
	double		val[3][3];
}				t_33matf;

typedef struct	s_4vecf
{
	double		val[4];
}				t_4vecf;

typedef struct	s_3vecf
{
	double		val[3];
}				t_3vecf;

typedef struct	s_2vecf
{
	double		val[2];
}				t_2vecf;

typedef struct	s_sphere
{
	t_3vecf		origin;
	double		radius;
}				t_sphere;

typedef struct	s_ellipsoid
{
	double		x_fact;
	double		y_fact;
	double		z_fact;
	t_3vecf		origin;
}				t_ellipsoid;

typedef struct	s_hyperboloid
{
	double		x_fact;
	double		y_fact;
	double		z_fact;
	int			surface;
	t_3vecf		origin;
}				t_hyperboloid;

typedef struct	s_horse_saddle
{
	double		x_fact;
	double		y_fact;
	t_3vecf		origin;
}				t_horse_saddle;

typedef struct	s_monkey_saddle
{
	t_3vecf		origin;
}				t_monkey_saddle;

typedef struct	s_cyclide
{
	t_3vecf		origin;
	double		param;
}				t_cyclide;

typedef struct	s_fermat
{
	t_3vecf		origin;
}				t_fermat;

typedef struct	s_moebius
{
	t_3vecf		origin;
	double		radius;
	double		half_width;
}				t_moebius;

typedef struct	s_plane
{
	t_3vecf		origin;
	t_3vecf		normal;
	t_3vecf		x2d_axis;
}				t_plane;

typedef struct	s_cube
{
	t_2vecf		x_range;
	t_2vecf		y_range;
	t_2vecf		z_range;
}				t_cube;

typedef struct	s_triangle
{
	t_3vecf		a;
	t_3vecf		b;
	t_3vecf		c;
	t_3vecf		origin;
}				t_triangle;

typedef struct	s_rect
{
	t_3vecf		x_axis;
	t_3vecf		y_axis;
	t_3vecf		z_axis;
	t_3vecf		origin;
}				t_rect;

typedef struct	s_cone
{
	t_3vecf		center;
	t_3vecf		tip;
	double		radius;
}				t_cone;

typedef struct	s_cylinder
{
	t_3vecf		center;
	t_3vecf		tip;
	double		radius;
}				t_cylinder;

typedef struct	s_cut_uv
{
	t_2vecf		u_range;
	t_2vecf		v_range;
}				t_cut_uv;

typedef struct	s_cut_classic
{
	t_3vecf		origin;
	t_3vecf		normal;
}				t_cut_classic;

typedef	struct	s_cut
{
	t_cut_type	cut_type;
	void		*cut_param;
	void		(*move)(struct s_cut *, t_3vecf, double);
	void		(*rotate)(struct s_cut *, t_3vecf, t_33matf *);
	struct s_cut	*next;
}				t_cut;

typedef struct	s_text_img
{
	int				width;
	int				height;
	unsigned int	*pixels;
}				t_text_img;

typedef struct	s_text_proc
{
	t_4vecf		color[3];
}				t_text_proc;

typedef struct	s_text
{
	t_text_type	text_type;
	t_2vecf		scale;
	t_2vecf		offset;
	t_bump_type	bump_type;
	double		bump_fact;
	void		*text_param;
}				t_text;

typedef	struct	s_motion
{
	t_3vecf		dir;
	double		speed_fact;
	int			spf;
	struct s_motion	*next;
}				t_motion;

typedef struct	s_data		t_data;
typedef struct	s_composed	t_composed;

typedef struct	s_leq
{
	t_3vecf		orig;
	t_3vecf		dir;
}				t_leq;

typedef struct	s_dist
{
		double	*dist;
		double	min_dist;
		double	max_dist;
}				t_dist;

typedef struct	s_obj
{
	t_obj_type		obj_type;
	t_mat_type		material_type;
	void			*obj_param;
	t_cut			*cuts;
	t_motion		*motions;
	int				(*ray_intersect)(t_leq, struct s_obj *, t_dist, int);
	int				(*check_inside)(t_3vecf, struct s_obj *);
	t_3vecf			(*get_origin)(struct s_obj *);
	t_3vecf			(*get_normal_inter)(t_3vecf, struct s_obj *, int);
	t_4vecf			(*get_text_color)(t_3vecf, t_3vecf, struct s_obj *);
	t_2vecf			(*get_text_coordinate)(t_3vecf, t_3vecf, struct s_obj *);
	t_3vecf			(*get_bump_mapping)(t_3vecf, t_3vecf, struct s_obj *);
	void			(*move)(struct s_obj *, t_3vecf, double);
	void			(*rotate)(struct s_obj *, t_3vecf, t_33matf *);
	t_text			text;
	double			reflection;
	double			refraction;
	double			shininess;
	struct s_obj	*next;
	t_data			*data;
	t_3vecf			composed_origin;
	struct s_obj	**composed_w;
}				t_obj;

typedef struct  s_comp_param
{
	t_3vecf 	origin;
	t_2vecf 	rotation;
}				t_comp_param;

typedef struct	s_composed
{
	char		*name;
	t_obj		**components;
	struct s_composed	*next;
}				t_composed;

typedef struct	s_light
{
	t_light_type	light_type;
	t_3vecf			color;
	t_3vecf			param;
	struct s_light	*next;
}				t_light;

typedef struct	s_cam
{
	t_3vecf		origin;
	t_2vecf		rotation;
}				t_cam;

typedef	struct	s_photon
{
	t_3vecf		position;
	t_3vecf		direction;
	t_3vecf		color;
}				t_photon;

typedef	struct	s_kd_tree
{
	struct s_kd_tree	*left;
	struct s_kd_tree	*right;
	t_photon			*photon;
}				t_kd_tree;

typedef struct	s_data
{
	t_mlx		*mlx;
	t_mlx		*info;
	t_cam		*camera;
	t_obj		*objs;
	t_composed	*composed_objs;
	t_obj		*negative_objs;
	t_light		*lights;
	char		*scene_name;
	t_2vecf		size;
	int			hooks;
	t_33matf	rot_mat[3];
	int			anti_al;
	double		aa_adapt;
	int			cel_shading;
	int			motion_blur;
	int			stereoscopy;
	int			indirect_gi;
	int			caustics_gi;
	t_2vecf		fog;
	t_3vecf		(*apply_color_filter)(t_3vecf);
	t_kd_tree	*indirect_map;
	t_kd_tree	*caustic_map;
	t_cube		bbox_photon;
	t_obj		*selected_obj;
	char		*skybox_name;
	int			to_next;
	int			new_obj;
	int     	first_loop;
	unsigned char     permutation[512];
	t_3vecf 	gradient[16];
	double  	water_f;
	int     	water;
	struct s_data	*next;
}				t_data;

typedef struct	s_thread
{
	t_data		*data;
	int			start;
	int			end;
}				t_thread;

typedef struct	s_cut_fparam
{
	t_obj		*closest_obj;
	t_obj		*objs;
	int			sp_id;
	int			negative;
}				t_cut_fparam;

typedef struct	s_add_pht_p
{
	double		dist;
	int			nn_photon;
}				t_add_pht_p;

typedef struct	s_nn_param
{
	t_3vecf 	inter_point;
	t_3vecf		normal_inter;
	t_photon	**tab;
	double		*closest;
	double		*farest;
	int			nn_photon;
	int			*actual_nn;
}				t_nn_param;

typedef struct  s_phtn_cast
{
	t_data			*data;
	int				*ind_i;
	int				*caus_i;
	t_photon		**photon_tab;
	int				depth;
	unsigned int	rand_iter;
	t_3vecf			pwr;
	int				photon_type;
}				t_phtn_cast;

typedef struct  s_phtn_prob
{
	double		absorb_prob;
	double		refract_prob;
	double		reflect_prob_spe;
	double		reflect_prob_dif;
}				t_phtn_prob;

typedef struct  s_compute_light_param
{
	t_3vecf 	inter_point;
	t_3vecf 	normal_inter;
	t_3vecf 	dir;
	t_light 	*lights;
	t_obj		*objs;
	int			sp_id;
	t_data		*data;
	double 		shininess;
}				t_compute_light_param;

typedef struct	s_inter
{
	t_3vecf 	inter_point;
	t_3vecf 	normal_inter;
}				t_inter;

typedef struct	s_rt_param
{
		double	min_dist;
		double	max_dist;
		int		depth;
		int		sp_id;
}				t_rt_param;

typedef struct	s_ilc_p
{
		t_3vecf *lighted_color;
		t_4vecf	obj_color;
		t_data	*data;
		int		sp_id;
}				t_ilc_p;

typedef struct  s_rayt_param
{
		t_3vecf *lighted_color;
		t_3vecf inv_dir;
		t_4vecf	obj_color;
		t_data	*data;
		int		depth;
		int		sp_id;
}				t_rayt_param;

typedef struct  s_clre_param
{
		double	closest_dist;
		t_leq	l;
		int     depth;
}				t_clre_param;

typedef struct  s_anti_al
{
		int     anti_al_iter;
		int     offset;
		int     aa;
}				t_anti_al;

t_obj	*copy_object(t_obj *src);
void	get_uv_axis(t_3vecf axis[3], t_3vecf first_axis);
void	clamp_and_set_dflt(t_obj *obj);
int		error_parse_object(t_obj *obj);


int		is_closest_intersect(t_dist dist, double root);

void	generate_new_sphere(t_data *data);
void	generate_new_plane(t_data *data);
void	generate_new_cone(t_data *data);
void	generate_new_cylinder(t_data *data);
void	generate_new_triangle(t_data *data);
void	generate_new_moebius(t_data *data);
void	generate_new_ellipsoid(t_data *data);
void	generate_new_hyperboloid(t_data *data);
void	generate_new_horse_saddle(t_data *data);
void	generate_new_monkey_saddle(t_data *data);
void	generate_new_cyclide(t_data *data);
void	generate_new_fermat(t_data *data);
t_text	generate_random_texture(t_obj *obj);
t_4vecf	generate_random_color(unsigned int x, double transp_f);
int		generate_random_enum(int e_max);
void	delete_object(t_data *data, t_obj *obj);
void  	free_all(t_data *data);
int		close_cross(t_data *data);
t_data	*init_data(char *file_name, t_mlx *mlx);
void    init_perlin(t_data *data);
t_33matf	init_rotation_matrix_x(double theta);
t_33matf	init_rotation_matrix_y(double theta);
t_33matf	init_rotation_matrix_z(double theta);
t_33matf	init_rotation_matrix_vec(t_3vecf a, double b);

t_3vecf	window_to_view(double x, double y, double win_w, double win_h);

void	render(t_data *data);

void	init_threads(t_thread threads[NB_THREADS], t_data *data);
void  init_frames_rot_mat(t_data *data);
t_anti_al	init_anti_al(int aa, int offset, int anti_al_iter);
t_3vecf init_ray_dir(int i, int j, t_anti_al a, t_data *data);
void  compute_stereoscopy(t_data *data, t_leq l, int i, int j);
void  compute_classic(t_data *data, t_leq l, int i, int j);

void  	check_subsampling(t_data *data, int i, int j, t_3vecf color);
void	ray_put_pixel(t_pixel cmp, int *img, t_3vecf color, t_data *data);
int		clip_color(double color);

t_3vecf get_refl_color(t_rayt_param p, t_inter i);
t_3vecf get_refr_color(t_rayt_param p, t_inter i, t_leq l, t_obj *obj);
t_3vecf	compute_glare(t_leq l, t_light *lights, t_3vecf *inter_point);
t_3vecf add_color_effect(t_data *data, t_clre_param p,
	t_3vecf lighted_color, t_inter i);
t_inter init_inter(t_leq l, double closest_dist, t_obj *closest_obj, int sp_id);
int	init_lighted_color(t_obj *closest_obj, t_inter i,
	t_leq l, t_ilc_p p);
t_3vecf	compute_lights(t_compute_light_param p);
void	cel_shade(double *val);
void    clamp_transparency(t_3vecf *transp_fact);
t_obj *check_for_shadow(t_3vecf *transp_fact, t_3vecf light_dir,
	double light_len, t_compute_light_param *p);
t_dist	new_tdist(double *cdist);




int		parse_rt_conf(char *file_name, t_data *data);
int		parse_3vecf(char *line, int i, t_3vecf *vec);
int		parse_4vecf(char *line, int i, t_4vecf *vec);
int		parse_double(char **line, int i, double *val);
int		parse_2vecf(char *line, int i, t_2vecf *vec);
int		parse_int(char **line, int i, int *val);

t_3vecf	assign_3vecf(double x, double y, double z);
t_2vecf	assign_2vecf(double x, double y);
void	normalize_3vecf(t_3vecf *vec);
double	get_length_3vecf(t_3vecf vec);
t_3vecf	sub_3vecf(t_3vecf a, t_3vecf b);
t_3vecf	add_3vecf(t_3vecf a, t_3vecf b);
t_3vecf	add_c3vecf(t_3vecf a, double c);
t_3vecf	product_3vecf(t_3vecf a, t_3vecf b);
t_3vecf	product_c3vecf(t_3vecf a, double c);
double	dot_product_3vecf(t_3vecf a, t_3vecf b);
double	dot_product_2vecf(t_2vecf a, t_2vecf b);
int		is_null_3vecf(t_3vecf vec);
t_3vecf neg_3vecf(t_3vecf vec);

t_3vecf	mult_3vecf_33matf(t_3vecf vect, t_33matf mat);
void	mult_vec_matrix(t_3vecf a, t_44matf mat, t_3vecf *dst);
void	mult_dir_matrix(t_3vecf a, t_44matf mat, t_3vecf *dst);
t_44matf	build_translation_matrix(t_3vecf a, t_3vecf b, t_3vecf c, t_3vecf d);

t_3vecf	move_3vecf(t_3vecf a, t_motion *b, int c);

void    handle_perlin_inter_point(t_3vecf *int_part,
  t_3vecf *floating_part, t_3vecf inter_point, double scale);
void set_quintic_factors(double q[3], t_3vecf f);
double	compute_3dperlin_factor(t_3vecf inter_point, double scale, const unsigned char permutation[512], const t_3vecf gradient[16]);
double	compute_3dfbm_factor(t_3vecf inter_point, double scale);
double	compute_wood_factor(t_3vecf inter_point, double scale, const unsigned char permutation[512], const t_3vecf gradient[16]);
double	compute_marble_factor(t_3vecf inter_point, double scale, const unsigned char permutation[512], const t_3vecf gradient[16]);

t_3vecf	compute_global_illumination(t_3vecf inter_point, t_3vecf normal_inter, t_kd_tree *photon_map, int nn_photon);
t_3vecf		compute_radiance_estimation(t_photon **nearest_n, t_3vecf inter_point,
	double farest, int nn_photon);

t_3vecf	refract_ray(t_3vecf dir, t_3vecf normal_inter, double refraction_index);
t_3vecf	reflect_ray(t_3vecf dir, t_3vecf normal_inter);
double	compute_fresnel_ratio(t_3vecf dir, t_3vecf normal_inter, double refraction_index);

double	linear_interpolate(double a, double b, double val);

int		is_null(double value);
int		is_null_3vecf(t_3vecf t);
double	degree_to_radian(double degree);

t_3vecf	solve_cubic(double a, double b, double c, double d);
t_2vecf	solve_quadratic(double a, double b, double c);


int		key_press(int keycode, void *param);
int		key_release(int keycode, void *param);
int		check_pscreen_key_press(int keycode, t_data *data);
int		check_gen_key_press(int keycode, t_data *data);
int		check_delete_key_press(int keycode, t_data *data);
int		check_switch_key_press(int keycode, t_data *data);
int		check_esc_key_press(int keycode, t_data *data);
int		check_mov_key_press(int keycode, t_data *data);
int		check_rot_key_press(int keycode, t_data *data);

int		mouse_hook(int button, int x, int y, void *param);
int		print_loop_image(void *param);
void	manage_obj_move(t_data *data, int *ret);
void	manage_obj_rotation(t_data *data, int *ret);
void	manage_cam_rotation(t_data *data, int *ret);
void	manage_cam_move(t_data *data, int *ret);

int		parse_scene(char **line, t_data *data);
int		parse_composed_model(char **line, t_data *data);
int		is_composed_object(char **line, t_data *data, int *ret);
char	goto_next_element(char **line);
int		parse_name(char **line, char **name, int i);
int		parse_objects(char **line, t_data *data, t_composed *from);
int		parse_lights(char **line, t_data *data);
int		parse_camera(char **line, t_data *data);

void	*parse_proc(char **line);
void	*parse_img(char *name);
void	*parse_texture_img(char **line);
int		parse_texture2(char **line, t_obj *obj);
int		parse_bump_mapping(char **line, t_obj *obj);
void	set_bump_own(t_obj *obj);
int		parse_bump_inde(char **line, t_obj *obj, int index);
int		parse_motion(char **line, t_obj *obj);

int		parse_rotation(char **line, t_2vecf *t, int i);
int		parse_origin(char **line, t_3vecf *t, int i);

int		parse_cone(char **line, t_obj *cone);
int		parse_cylinder(char **line, t_obj *cylinder);
int		parse_plane(char **line, t_obj *plane);
int		parse_triangle(char **line, t_obj *triangle);
int		parse_sphere(char **line, t_obj *sphere);
int		parse_ellipsoid(char **line, t_obj *ellipsoid);
int		parse_hyperboloid(char **line, t_obj *hyperboloid);
int		parse_horse_saddle(char **line, t_obj *horse_saddle);
int		parse_monkey_saddle(char **line, t_obj *monkey_saddle);
int		parse_cyclide(char **line, t_obj *cyclide);
int		parse_fermat(char **line, t_obj *fermat);
int		parse_moebius(char **line, t_obj *moebius);

void  	assign_cone_function(t_obj *cone);
void  	assign_cylinder_function(t_obj *cylinder);
void  	assign_plane_function(t_obj *plane);
void  	assign_triangle_function(t_obj *triangle);
void  	assign_sphere_function(t_obj *sphere);
void  	assign_ellipsoid_function(t_obj *ellipsoid);
void  	assign_hyperboloid_function(t_obj *hyperboloid);
void  	assign_horse_saddle_function(t_obj *horse_saddle);
void  	assign_monkey_saddle_function(t_obj *monkey_saddle);
void  	assign_cyclide_function(t_obj *cyclide);
void  	assign_fermat_function(t_obj *fermat);
void  	assign_moebius_function(t_obj *moebius);

int		parse_ambient(char **line, t_light *light, t_data *data);
t_3vecf	ray_trace(t_leq l, t_data *data, int depth, int sp_id);
t_3vecf	motion_trace(t_3vecf orig, t_3vecf dir, t_data *data);

t_3vecf	get_cone_origin(t_obj *cone, t_cone *cone_param, int sp_id);
t_3vecf	get_cone_tip(t_obj *cone, t_cone *cone_param, int sp_id);
int		ray_intersect_cone(t_leq l, t_obj *cone, t_dist dist, int sp_id);
int		check_inside_cone(t_3vecf point, t_obj *cone);
t_3vecf	get_normal_intersect_cone(t_3vecf inter_point, t_obj *cone, int sp_id);
t_3vecf	get_origin_cone(t_obj *cone);
void	move_cone(t_obj *cone, t_3vecf a, double b);
void	rotate_cone(t_obj *cone, t_3vecf b, t_33matf *c);
t_2vecf	get_text_coordinate_cone(t_3vecf inter_point, t_3vecf normal_inter, t_obj *cone);

t_3vecf	get_cylinder_origin(t_obj *cylinder, t_cylinder *cylinder_param,
	int sp_id);
	t_3vecf	get_cylinder_tip(t_obj *cylinder, t_cylinder *cylinder_param, int sp_id);
int 	ray_intersect_cylinder(t_leq l, t_obj *cylinder, t_dist dist, int sp_id);
int		check_inside_cylinder(t_3vecf point, t_obj *cylinder);
t_3vecf	get_normal_intersect_cylinder(t_3vecf inter_point, t_obj *cylinderi, int sp_id);
t_3vecf	get_origin_cylinder(t_obj *a);
void	move_cylinder(t_obj *a, t_3vecf b, double c);
void	rotate_cylinder(t_obj *cone, t_3vecf b, t_33matf *c);
t_2vecf	get_text_coordinate_cylinder(t_3vecf inter_point, t_3vecf normal_inter, t_obj *cylinder);

t_3vecf	get_sphere_origin(t_obj *sphere, t_sphere *sphere_param, int sp_id);
int		ray_intersect_sphere(t_leq l, t_obj *sphere, t_dist dist, int sp_id);
int		check_inside_sphere(t_3vecf point, t_obj *sphere);
t_3vecf	get_normal_intersect_sphere(t_3vecf inter_point, t_obj *sphere, int a);
t_3vecf	get_origin_sphere(t_obj *a);
void	move_sphere(t_obj *a, t_3vecf b, double c);
void	rotate_sphere(t_obj *cone, t_3vecf b, t_33matf *c);
t_2vecf	get_text_coordinate_sphere(t_3vecf inter_point, t_3vecf normal_inter, t_obj *sphere);

t_3vecf	get_fermat_origin(t_obj *fermat, t_fermat *param, int sp_id);
int		ray_intersect_fermat(t_leq l, t_obj *fermat, t_dist dist, int sp_id);
int		check_inside_fermat(t_3vecf point, t_obj *fermat);
t_3vecf	get_normal_intersect_fermat(t_3vecf inter_point, t_obj *fermat, int a);
t_3vecf	get_origin_fermat(t_obj *a);
void	move_fermat(t_obj *a, t_3vecf b, double c);
t_2vecf	get_text_coordinate_fermat(t_3vecf inter_point, t_3vecf normal_inter, t_obj *fermat);

int		ray_intersect_triangle(t_leq l, t_obj *triangle, t_dist dist, int sp_id);
t_3vecf	get_normal_intersect_triangle(t_3vecf inter_point, t_obj *triangle, int a);
t_3vecf	get_origin_triangle(t_obj *a);
void	move_triangle(t_obj *a, t_3vecf b, double c);
void	rotate_triangle(t_obj *cone, t_3vecf b, t_33matf *c);
t_2vecf	get_text_coordinate_triangle(t_3vecf inter_point, t_3vecf normal_inter, t_obj *triangle);

t_3vecf	get_ellipsoid_origin(t_obj *ellipsoid, t_ellipsoid *ellipsoid_param,
	int sp_id);
int		ray_intersect_ellipsoid(t_leq l, t_obj *ellipsoid, t_dist dist, int sp_id);
int		check_inside_ellipsoid(t_3vecf point, t_obj *ellipsoid);
t_3vecf	get_normal_intersect_ellipsoid(t_3vecf inter_point, t_obj *ellipsoid, int a);
t_3vecf	get_origin_ellipsoid(t_obj *a);
void	move_ellipsoid(t_obj *a, t_3vecf b, double c);
t_2vecf	get_text_coordinate_ellipsoid(t_3vecf inter_point, t_3vecf normal_inter, t_obj *ellipsoid);

t_3vecf	get_hyperboloid_origin(t_obj *hyperboloid,
	t_hyperboloid *hyperboloid_param, int sp_id);
int		ray_intersect_hyperboloid(t_leq l, t_obj *hyperboloid, t_dist dist, int sp_id);
int		check_inside_hyperboloid(t_3vecf point, t_obj *hyperboloid);
t_3vecf	get_normal_intersect_hyperboloid(t_3vecf inter_point, t_obj *hyperboloid, int a);
t_3vecf	get_origin_hyperboloid(t_obj *a);
void	move_hyperboloid(t_obj *a, t_3vecf b, double c);
t_2vecf	get_text_coordinate_hyperboloid(t_3vecf inter_point, t_3vecf normal_inter, t_obj *hyperboloid);

t_3vecf	get_horse_saddle_origin(t_obj *horse_saddle, t_horse_saddle *param,
	int sp_id);
int		ray_intersect_horse_saddle(t_leq l, t_obj *horse_saddle, t_dist dist, int sp_id);
int		check_inside_horse_saddle(t_3vecf point, t_obj *horse_saddle);
t_3vecf	get_normal_intersect_horse_saddle(t_3vecf inter_point, t_obj *horse_saddle, int a);
t_3vecf	get_origin_horse_saddle(t_obj *a);
void	move_horse_saddle(t_obj *a, t_3vecf b, double c);
t_2vecf	get_text_coordinate_horse_saddle(t_3vecf inter_point, t_3vecf normal_inter, t_obj *horse_saddle);

t_3vecf	get_monkey_saddle_origin(t_obj *monkey_saddle, t_monkey_saddle *param,
	int sp_id);
int		ray_intersect_monkey_saddle(t_leq l, t_obj *monkey_saddle, t_dist dist, int sp_id);
int		check_inside_monkey_saddle(t_3vecf point, t_obj *monkey_saddle);
t_3vecf	get_normal_intersect_monkey_saddle(t_3vecf inter_point, t_obj *monkey_saddle, int a);
t_3vecf	get_origin_monkey_saddle(t_obj *a);
void	move_monkey_saddle(t_obj *a, t_3vecf b, double c);
t_2vecf	get_text_coordinate_monkey_saddle(t_3vecf inter_point, t_3vecf normal_inter, t_obj *monkey_saddle);

t_3vecf	get_cyclide_origin(t_obj *cyclide, t_cyclide *param, int sp_id);
int		ray_intersect_cyclide(t_leq l, t_obj *cyclide, t_dist dist, int sp_id);
int		check_inside_cyclide(t_3vecf point, t_obj *cyclide);
t_3vecf	get_normal_intersect_cyclide(t_3vecf inter_point, t_obj *cyclide, int sp_id);
t_3vecf	get_origin_cyclide(t_obj *cyclide);
void	move_cyclide(t_obj *cyclide, t_3vecf b, double c);
t_2vecf	get_text_coordinate_cyclide(t_3vecf inter_point, t_3vecf normal_inter, t_obj *cyclide);

t_3vecf	get_plane_origin(t_obj *plane, t_plane *plane_param, int sp_id);
int		ray_intersect_plane(t_leq l, t_obj *plane, t_dist dist, int sp_id);
int		check_inside_plane(t_3vecf point, t_obj *plane);
t_3vecf	get_normal_intersect_plane(t_3vecf inter_point, t_obj *plane, int sp_id);
t_3vecf	get_origin_plane(t_obj *a);
void	move_plane(t_obj *a, t_3vecf b, double c);
void	rotate_plane(t_obj *cone, t_3vecf a, t_33matf *b);
t_2vecf	get_text_coordinate_plane(t_3vecf inter_point, t_3vecf normal_inter, t_obj *plane);

t_3vecf	get_moebius_origin(t_obj *moebius, t_moebius *param, int sp_id);
int		ray_intersect_moebius(t_leq l, t_obj *moebius, t_dist dist, int sp_id);
int		check_inside_moebius(t_3vecf point, t_obj *moebius);
t_3vecf	get_normal_intersect_moebius(t_3vecf inter_point, t_obj *moebius, int sp_id);
t_3vecf	get_origin_moebius(t_obj *a);
void	move_moebius(t_obj *a, t_3vecf b, double c);
t_2vecf	get_text_coordinate_moebius(t_3vecf inter_point, t_3vecf normal_inter, t_obj *moebius);


t_obj *check_cuts(t_leq l, t_dist dist, t_cut_fparam cp, t_data *data);
t_obj	*ray_first_intersect(t_leq l, t_dist dist, t_obj *objs, int sp_id);

void	print_conf(t_data *data);
void	print_vec2(double vec[2]);
void	print_vec(double vec[3]);
void	print_obj_param(t_obj *obj);
int		isequal_3vecf(t_3vecf *t1, t_3vecf *t2);
int		check_normal(t_3vecf *t);

t_4vecf	get_uni_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_4vecf	get_grid_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_4vecf	get_perlin_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_4vecf	get_fbm_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_4vecf	get_marble_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_4vecf	get_wood_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_4vecf	get_image_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);

t_3vecf   get_bump_mapping_water(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_3vecf	get_bump_mapping_perlin(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_3vecf	get_bump_mapping_fbm(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_3vecf	get_bump_mapping_marble(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_3vecf	get_bump_mapping_wood(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_3vecf	get_bump_mapping_image(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);

int		pick_type_cutting(char **line, t_cut *cut, int *ret);
int		parse_cutting(char **line, t_obj *obj);
void	move_cut_plane(t_cut *cut, t_3vecf dir, double fact);
void	move_cut_sphere(t_cut *cut, t_3vecf dir, double fact);
void	move_cut_cube(t_cut *cut, t_3vecf dir, double fact);

void	rotate_cut_plane(t_cut *cut, t_3vecf orig, t_33matf rot_mat[2]);

int		parse_color_filter(char **line, t_data *data);
int		parse_material(char **line, int i, t_obj *obj);

int		return_update(char *error, int ret, int skip);
int		clamp_val(double *val, double min, double max);

t_3vecf	apply_color_filter_sepia(t_3vecf color);

void	add_object(t_obj *obj, t_data *data);

void  cast_photon(t_leq l, t_phtn_cast p);;
void			check_photon_map(t_data *data);
void  cast_photon(t_leq l, t_phtn_cast p);
t_3vecf add_color_bleed(t_3vecf pwr, t_4vecf obj_color);
void  refract_photon(t_leq l, t_phtn_cast p);
void  reflect_photon_spec(t_leq l, t_phtn_cast p, int photon_type);
void  reflect_photon_diff(t_leq l, t_phtn_cast p,
	t_photon photon, t_3vecf normal_inter);
void  absorb_photon(t_leq l, t_phtn_cast p, t_photon photon);
t_phtn_prob	get_prob(double absorb_prob, double refract_prob, double reflect_prob);
void		scatter_photon(t_photon **photon_tab, t_data *data);
t_kd_tree	*build_tree(t_photon *phtn_tab, int lw, int hi, int ax);

t_3vecf get_random_dir(int rand_iter);



double		get_random_number(unsigned int x);
int     syn_error(char *s1, char *s2, char*s3, char *s4);
int     error(char *s1, char *s2);
int     ft_memalloc_error(int ret, size_t size);
void	ft_mem_error(void);

int		check_lights_cam(t_data *data);
int		check_skybox(t_data *data);
int		open_info(t_data *data);
int		init_loading_screen(t_data *data);
void	update_loading_screen_gi(int pc, t_text_img *img, t_data *data);

void	push_object(t_obj *obj, int composed, t_data *data, t_composed *from);
void  free_info(t_data *data);
void	free_data(t_data *data);
void	free_object(t_obj *obj);

int			parse_sphere_element(char **line, int *ret, t_sphere *sphere_param);
#endif
