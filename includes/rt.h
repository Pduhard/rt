#ifndef RT_H
# define RT_H


# include "../libft/libft.h"
# include "../external_libs/minilibx_macos/mlx.h"
# include "../external_libs/sdl/SDL.h"
# include "../frameworks/SDL2.framework/Headers/SDL.h"
# include "../frameworks/SDL2_image.framework/Headers/SDL_image.h"
# include <math.h>
# include <pthread.h>
# include <fcntl.h>
# include <float.h>
# include <time.h>
# define WIN_WIDTH 600
# define WIN_HEIGHT	600
# define NB_THREADS	8
# define MAX_ANTI_AL 4
# define MAX_ANTI_AL2 16

# define MAX_VIEW 500

/* TMP MACRO  */

# define GLOBAL_ILLUMINATION	0
# define GL_RADIUS				0.1
# define NB_PHOTON				10000
# define NN_PHOTON_MAX			20
//# define NB_GLOBAL_PHOTON		10000
//# define NB_CAUSTIC_PHOTON		100000
# define PHOTON_DEPTH			10
# define CEL_SHADING	0
# define ANTI_AL		0

/*ALTERABLE MACRO	*/

# define PERLIN_TRANSP_ADD 1
# define MOTION_STEP 0.02
# define MOTION_FVEL 8
# define MOTION_SPP 32

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
# define A_KEY	0b1
# define D_KEY	0b10
# define W_KEY	0b100
# define S_KEY	0b1000
# define F_KEY	0b10000
# define G_KEY	0b100000

# define ARR_LEFT_KEY	0b1000000
# define ARR_RIGHT_KEY	0b10000000
# define ARR_DOWN_KEY	0b100000000
# define ARR_UP_KEY		0b1000000000

# define SPACE_KEY	0b10000000000
# define SHIFT_KEY	0b100000000000

# define ESC_KEY 0x0035

/* Conf Mess */

# define CAM "<camera\n"
# define CYLINDER "<cylinder\n"
# define SPHERE "<sphere\n"
# define PLANE "<plane\n"
# define CONE "<cone\n"
# define MOEBIUS "<moebius\n"
# define ORIGIN "\t<origin (x, y, z)>\n"
# define CENTER "\t<center (x, y, z)>\n"
# define ROTATION "\t<rotation (x, y)\n>"
# define TIP "\t<tip (x, y)>\n"
# define RADIUS "\t<radius (X)>\n"
# define NAME "<name(scene_name)>\n"
# define SIZE "<size (WD,HH)>\n"
# define NORMAL "\t<normal (x, y ,z)>\n"
# define XAXIS "\t<xaxis (x, y, z)>\n"
# define HALFWIDTH "\t<half_width (X)>\n"

/* Error Mess */
# define ERRORSIZE "WIN_Size : Min 400/400, Max 2560/1420\n"
# define ERRORARG  "Usage: ./rtv1 NameFile.rt_conf\n"
# define ERRORTHREAD "Number Thread : Min 1, Max 16\n"
# define ERRORFILE "Invalid File\n"
# define ERROREMPTY "File error : empty\n"
# define ERRORSTRIPE "File error : stripe\n"
# define ERRORSCENE "File error : rt_conf start by <scene...\n"
# define UNKNOWSCENE "Unrecognized Scene Element\n"
# define UNKNOWOBJECT "Unrecognized Object Element\n"
# define ALREADYCAM "File error : Camera already exist\n"
# define ALREADYOBJ "Object already declared\n"
# define SERROR "Syntax or Values error :\n"
# define ERRORCAM "No camera in file .rt_conf\n"
# define SCAM "<camera\n\t<origin (x,y,z)>\n\t<rotation (x,y)>\n>\n"
# define SERRORLIGHT "Syntax error: light(type)(origin)(intensity)\n"
# define SERRORCYL "<cylinder\n\t<origin (x,y,z)>\n\t(tip)(radius)(color)\n"
# define SERRORSPHERE "Syntax error: sphere(origin)(radius)(color)\n"
# define SERRORPLANE "Syntax error: plane(origin)(normal)(color)\n"
# define SERRORCONE "Syntax error: cone(center)(tip)(radius)(color)\n"

typedef	enum {
	OBJ_SPHERE,
	OBJ_PLANE,
	OBJ_CONE,
	OBJ_CYLINDER,
	OBJ_MOEBIUS,
	OBJ_CUT_TEXTURE,
	OBJ_CUBE
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
	TEXT_UNI,
	TEXT_GRID,
	TEXT_PERLIN,
	TEXT_MARBLE,
	TEXT_WOOD,
	TEXT_IMAGE
}	t_text_type;

typedef enum {
	BUMP_UNI,
	BUMP_GRID,
	BUMP_PERLIN,
	BUMP_MARBLE,
	BUMP_WOOD,
	BUMP_IMAGE,
	BUMP_SINUS
}	t_bump_type;

typedef enum {
	CUT_STATIC,
	CUT_REAL,
	CUT_PLAN
}	t_cut_type;

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

typedef struct	s_cut_classic
{
	t_3vecf		origin;
	t_3vecf		normal;
}				t_cut_classic;

typedef	struct	s_cut
{
	t_cut_type	cut_type;
	void		*cut_param;
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

typedef struct	s_data	t_data;

typedef struct	s_obj
{
	t_obj_type		obj_type;
	t_mat_type		material_type;
	void			*obj_param;
	struct s_obj	*cuts;
	t_motion		*motions;
	int				(*ray_intersect)(t_3vecf, t_3vecf, struct s_obj *, double *, double, double, int);
	int				(*check_inside)(t_3vecf, struct s_obj *);
	t_3vecf			(*get_origin)(struct s_obj *);
	t_3vecf			(*get_normal_inter)(t_3vecf, struct s_obj *i, int);
	t_4vecf			(*get_text_color)(t_3vecf, t_3vecf, struct s_obj *);
	t_2vecf			(*get_text_coordinate)(t_3vecf, t_3vecf, struct s_obj *);
	t_3vecf			(*get_bump_mapping)(t_3vecf, t_3vecf, struct s_obj *);
	void			(*move)(struct s_obj *, t_3vecf, double);
	t_text			text;
	double			reflection;
	double			refraction;
	double			shininess;
	struct s_obj	*next;
	t_data			*data;
}				t_obj;

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
	t_photon		*photon;
}				t_kd_tree;

typedef struct	s_data
{
	double			f;
	t_mlx		*mlx;
	t_cam		*camera;
	t_obj		*objs;
	t_obj		*negative_objs;
	t_light		*lights;
	char		*scene_name;
	t_2vecf		size;
	int			hooks;
	t_33matf	rot_mat[3];
	Uint32		fps;
	Uint32		delta_time;
	int			anti_al;
	int			motion_blur;
	int			stereoscopy;
	t_3vecf		(*apply_color_filter)(t_3vecf);
	t_kd_tree	*photon_map;
	t_cube		bbox_photon;
}				t_data;

typedef struct	s_thread
{
	t_data		*data;
	int			start;
	int			end;
}				t_thread;

t_data	*init_data(char *file_name);
void	init_camera_to_world_matrix(double mat[4][4]);
void	init_light_to_world_matrix(double mat[4][4]);
t_33matf	init_rotation_matrix_x(double theta);
t_33matf	init_rotation_matrix_y(double theta);
t_33matf	init_rotation_matrix_z(double theta);
t_33matf	init_rotation_matrix_vec(t_3vecf, double);

void	render(t_data *data);

int		parse_rt_conf(char *file_name, t_data *data);
int		parse_3vecf(char *line, int i, t_3vecf *vec);
int		parse_4vecf(char *line, int i, t_4vecf *vec);
int		parse_double(char *line, int i, double *val);
int		parse_2vecf(char *line, int i, t_2vecf *vec);
int		parse_double2(char **line, int i, double *val);
int		parse_int(char **line, int i, int *val);

//int		parse_sphere(char *line, t_data *data);
//int		parse_plane(char *line, t_data *data);
//int		parse_cone(char *line, t_data *data);
//int		parse_cylinder(char *line, t_data *data);

t_3vecf	assign_3vecf(double x, double y, double z);
t_2vecf	assign_2vecf(double x, double y);
void	normalize_3vecf(t_3vecf *vec);
double	get_length_3vecf(t_3vecf vec);
t_3vecf	sub_3vecf(t_3vecf a, t_3vecf b);
t_3vecf	product_3vecf(t_3vecf a, t_3vecf b);
double	dot_product_3vecf(t_3vecf a, t_3vecf b);
double	dot_product_2vecf(t_2vecf a, t_2vecf b);
int		is_null_3vecf(t_3vecf vec);

t_3vecf	mult_3vecf_33matf(t_3vecf vect, t_33matf mat);
t_33matf	mult_33matf_33matf(t_33matf a, t_33matf b);
void	mult_vec_matrix(t_3vecf, t_44matf mat, t_3vecf *dst);
void	mult_dir_matrix(t_3vecf, t_44matf mat, t_3vecf *dst);

t_3vecf	move_3vecf(t_3vecf, t_motion *, int);

double	compute_2dperlin_factor(t_2vecf inter_point, double scale);
double	compute_3dperlin_factor(t_3vecf inter_point, double scale);
double	compute_wood_factor(t_3vecf inter_point, double scale);
double	compute_marble_factor(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj, double scale);

t_3vecf	refract_ray(t_3vecf dir, t_3vecf normal_inter, double refraction_index, int inside);
t_3vecf	reflect_ray(t_3vecf dir, t_3vecf normal_inter);
double	compute_fresnel_ratio(t_3vecf dir, t_3vecf normal_inter, double refraction_index, int inside);

double	linear_interpolate(double a, double b, double val);

int		is_null(double value);
int		is_null_3vecf(t_3vecf t);
double	degree_to_radian(double degree);

t_3vecf	solve_cubic(double a, double b, double c, double d);
int		key_press(int keycode, void *param);
int		key_release(int keycode, void *param);
int		moov_hook(int x, int y, void *param);
int		print_loop_image(void *param);

int		brackets_rt(char *line);
int		parse_scene(char **line, t_data *data);
int		parse(char **line, t_data *data);
char	goto_next_element(char **line);
int		parse_scene_name(char **line, t_data *data);
int		parse_size(char **line, t_data *data);
int		parse_camera(char **line, t_data *data);
int		parse_objects(char **line, t_data *data);
int		parse_lights(char **line, t_data *data);
int		parse_color_transp(char **line, int i, t_4vecf *t);
void	*parse_proc(char **line, t_text *text);
void	*parse_img(char **line, t_text *text);
int		parse_texture2(char **line, t_obj *obj/*, t_data *data*/);
int		parse_bump_mapping(char **line, t_obj *obj);//t_text *text);
void	set_bump_own(t_obj *obj);//t_text *text);
int		parse_bump_inde(char **line, t_obj *obj, /*t_text *text, */int	index);
void	set_bump_inde(char *s, t_obj *obj);//t_text *text);
int		parse_motion(char **line, t_obj *obj);

int		parse_rotation(char **line, t_2vecf *t, int i);
int		parse_origin(char **line, t_3vecf *t, int i);

int		parse_cone(char **line, t_obj *cone, t_data *data);
int		parse_cylinder(char **line, t_obj *cylinder, t_data *data);
int		parse_plane(char **line, t_obj *plane, t_data *data);
int		parse_sphere(char **line, t_obj *sphere, t_data *data);
int		parse_moebius(char **line, t_obj *moebius, t_data *data);

int		parse_ambient(char **line, t_light *light, t_data *data);
t_3vecf	ray_trace(t_3vecf orig, t_3vecf dir, double min_dist, double max_dist, t_data *data, int depth, int sp_id);
t_3vecf	motion_trace(t_3vecf orig, t_3vecf dir, t_data *data);

int		ray_intersect_cone(t_3vecf orig, t_3vecf dir, t_obj *cone, double *dist, double min_dist, double max_dist, int sp_id);
int		check_inside_cone(t_3vecf point, t_obj *cone);
t_3vecf	get_normal_intersect_cone(t_3vecf inter_point, t_obj *cone, int sp_id);
t_3vecf	get_origin_cone(t_obj *cone);
void	move_cone(t_obj *cone, t_3vecf, double);
t_2vecf	get_text_coordinate_cone(t_3vecf inter_point, t_3vecf normal_inter, t_obj *cone);

int 	ray_intersect_cylinder(t_3vecf orig, t_3vecf dir, t_obj *cylinder, double *dist, double min_dist, double max_dist, int sp_id);
int		check_inside_cylinder(t_3vecf point, t_obj *cylinder);
t_3vecf	get_normal_intersect_cylinder(t_3vecf inter_point, t_obj *cylinderi, int sp_id);
t_3vecf	get_origin_cylinder(t_obj *);
void	move_cylinder(t_obj *, t_3vecf, double);
t_2vecf	get_text_coordinate_cylinder(t_3vecf inter_point, t_3vecf normal_inter, t_obj *cylinder);

int		ray_intersect_sphere(t_3vecf orig, t_3vecf dir, t_obj *sphere, double *dist, double min_dist, double max_dist, int sp_id);
int		check_inside_sphere(t_3vecf point, t_obj *sphere);
t_3vecf	get_normal_intersect_sphere(t_3vecf inter_point, t_obj *sphere, int);
t_3vecf	get_origin_sphere(t_obj *);
void	move_sphere(t_obj *, t_3vecf, double);
t_2vecf	get_text_coordinate_sphere(t_3vecf inter_point, t_3vecf normal_inter, t_obj *sphere);

int		ray_intersect_plane(t_3vecf orig, t_3vecf dir, t_obj *plane, double *dist, double min_dist, double max_dist, int sp_id);
int		check_inside_plane(t_3vecf point, t_obj *plane);
t_3vecf	get_normal_intersect_plane(t_3vecf inter_point, t_obj *plane, int sp_id);
t_3vecf	get_origin_plane(t_obj *);
void	move_plane(t_obj *, t_3vecf, double);
t_2vecf	get_text_coordinate_plane(t_3vecf inter_point, t_3vecf normal_inter, t_obj *plane);

int		ray_intersect_moebius(t_3vecf orig, t_3vecf dir, t_obj *moebius, double *dist, double min_dist, double max_dist, int sp_id);
int		check_inside_moebius(t_3vecf point, t_obj *moebius);
t_3vecf	get_normal_intersect_moebius(t_3vecf inter_point, t_obj *moebius, int sp_id);
t_3vecf	get_origin_moebius(t_obj *);
void	move_moebius(t_obj *, t_3vecf, double);
t_2vecf	get_text_coordinate_moebius(t_3vecf inter_point, t_3vecf normal_inter, t_obj *moebius);

t_obj	*check_cuts(t_3vecf orig, t_3vecf dir, t_obj *closest_obj, double min_dist, double max_dist, double *closest_dist, t_obj *objs, int sp_id, t_data *data);
t_obj	*ray_first_intersect(t_3vecf orig, t_3vecf dir, double min_dist, double max_dist, double *closest_dist, t_obj *objs, int sp_id, t_data *data);

void	print_conf(t_data *data);
void	print_vec2(double vec[2]);
void	print_vec(double vec[3]);
void	print_obj_param(t_obj *obj);
int		isequal_3vecf(t_3vecf *t1, t_3vecf *t2);
int		check_normal(t_3vecf *t);

t_4vecf	get_uni_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_4vecf	get_grid_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_4vecf	get_perlin_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_4vecf	get_marble_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_4vecf	get_wood_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_4vecf	get_image_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);

t_3vecf	get_bump_mapping_perlin(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_3vecf	get_bump_mapping_marble(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_3vecf	get_bump_mapping_wood(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_3vecf	get_bump_mapping_image(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_3vecf	get_bump_mapping_sinus(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);

int		parse_cutting(char **line, t_obj *obj);
int		parse_cut_static_real(char **line, t_obj *cut);

int		parse_onoff(char **line, int *onoff);
int		parse_color_filter(char **line, t_data *data);
int		parse_material(char **line, int i, t_obj *obj);

int		return_update(char *error, int ret, int skip);
int		clamp_val(double *val, double min, double max);

t_3vecf	apply_color_filter_sepia(t_3vecf color);

int		ft_strncmp_case(const char *s1, const char *s2, size_t n);
void	add_object(t_obj *obj, t_data *data);

t_kd_tree	*create_photon_map(t_data *data);
double		get_random_number(unsigned int x);
t_3vecf		compute_global_illumination(t_3vecf inter_point, t_3vecf normal_inter, t_data *data);

int     syn_error(char *s1, char *s2, char*s3, char *s4, char *s5);
int     error(char *s1);


#endif
