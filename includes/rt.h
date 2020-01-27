/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   rt.h                                             .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aplat <aplat@student.le-101.fr>            +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/16 01:10:39 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/27 20:57:42 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# include "../libft/libft.h"
# include "../external_libs/minilibx_macos/mlx.h"
# include "../external_libs/sdl/SDL.h"
# include "../frameworks/SDL2_image.framework/Headers/SDL_image.h"
# include <math.h>
# include <pthread.h>
# include <fcntl.h>
# include <float.h>
# include <time.h>
# define WIN_WIDTH	600
# define WIN_HEIGHT	600
# define NB_THREADS	8
# define MAX_ANTI_AL	4
# define MAX_ANTI_AL2	16

# define MAX_VIEW		500

/* TMP MACRO  */
# define SCALE_X	0.1
# define SCALE_Y	0.1

# define OFFSET_X	0.4
# define OFFSET_Y	0

# define CEL_SHADING	0
# define ANTI_AL		0

/* CST MACROS */
# define _M_PI_180	0.01745329251
# define _SQRT_2	1.41421356237

# define CEL_BOUND_1	0.05
# define CEL_BOUND_2	0.15
# define CEL_BOUND_3	0.3
# define CEL_BOUND_4	0.60
# define CEL_BOUND_6	0.90

# define CEL_BOUNDARY	0.04
/* HOOKS MACRO */
# define A_KEY	1
# define D_KEY	(1 << 1)
# define W_KEY	(1 << 2)
# define S_KEY	(1 << 3)
# define F_KEY	(1 << 4)
# define G_KEY	(1 << 5)

# define ARR_LEFT_KEY	(1 << 6)
# define ARR_RIGHT_KEY	(1 << 7)
# define ARR_DOWN_KEY	(1 << 8)
# define ARR_UP_KEY		(1 << 9)

# define SPACE_KEY	(1 << 10)
# define SHIFT_KEY	(1 << 11)

typedef	enum	{OBJ_SPHERE, OBJ_PLANE, OBJ_CONE, OBJ_CYLINDER} e_obj_type;
typedef	enum	{LIGHT_POINT, LIGHT_AMBIENT, LIGHT_DIRECTIONAL} e_light_type;
typedef	enum	{TEXT_UNI, TEXT_GRID, TEXT_PERLIN, TEXT_MARBLE, TEXT_WOOD, TEXT_IMAGE} e_text_type;

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

typedef struct	s_plane
{
	t_3vecf		origin;
	t_3vecf		normal;
}				t_plane;

typedef struct	s_cone
{
	t_3vecf		center;
	t_3vecf		tip;
	double		radius;
	//t_3vecf	origin;
	//t_3vecf	normal;
}				t_cone;

typedef struct	s_cylinder
{
	t_3vecf		center;
	t_3vecf		tip;
	double		radius;
}				t_cylinder;

typedef struct	s_text_img
{
	int				width;
	int				height;
	unsigned int	*pixels;
}				t_text_img;

typedef struct	s_text_proc
{
//	t_3vecf		color_1;
//	t_3vecf		color_2;
//	t_3vecf		color_3;

	t_3vecf		color[3];
	t_3vecf		transp;
}				t_text_proc;

typedef struct	s_text
{
	e_text_type	text_type;
	t_2vecf		scale;
	t_2vecf		offset;
	void		*text_param;
}				t_text;

typedef struct	s_obj
{
	e_obj_type	obj_type;
	void		*obj_param;
	int			(*ray_intersect)(t_3vecf, t_3vecf, struct s_obj *, double *, double, double);
	t_3vecf		(*get_normal_inter)(t_3vecf, struct s_obj *);
	t_3vecf		(*get_text_color)(t_3vecf, t_3vecf, struct s_obj *);
	t_2vecf		(*get_text_coordinate)(t_3vecf, t_3vecf, struct s_obj *);
	t_text		text;
	double		reflection;
	double		refraction; // water = 1.3 diamond = 1.8 ... always > 1 => < 1 will be considered as non refractive
	double		transmitance;
	double		transparence;
	double		shininess;
//	int			color;
	struct s_obj	*next;
}				t_obj;

typedef struct	s_light
{
	e_light_type	light_type;
	t_3vecf		color;
	t_3vecf		param;
	struct s_light	*next;
}				t_light;

typedef struct	s_cam
{
	t_3vecf		origin;
	t_2vecf		rotation;
}				t_cam;

typedef struct	s_data
{
//	double			f;
	t_mlx		*mlx;
	t_cam		*camera;
	t_obj		*objs;
	t_light		*lights;
//	double		fov;
//	t_44matf	camera_to_world;
	char		*scene_name;
	t_2vecf		size;
	int			hooks;
	t_33matf	rot_mat[3];
	Uint32		fps;
	Uint32		delta_time;
	int			anti_al;
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
t_33matf	init_rotation_matrix_vec(t_3vecf ,double);

void	render(t_data *data);

int		parse_rt_conf(char *file_name, t_data *data);
int		parse_3vecf(char *line, int i, t_3vecf *vec);
int		parse_double(char *line, int i, double *val);
//int		parse_texture(char *line, int i, t_obj *obj);
int		parse_2vecf(char *line, int i, t_2vecf *vec);
int		parse_double2(char **line, int i, double *val);

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

t_3vecf	mult_3vecf_33matf(t_3vecf vect, t_33matf mat);
t_33matf	mult_33matf_33matf(t_33matf a, t_33matf b);
void	mult_vec_matrix(t_3vecf, t_44matf mat, t_3vecf *dst);
void	mult_dir_matrix(t_3vecf, t_44matf mat, t_3vecf *dst);

double	compute_perlin_factor(t_3vecf inter_point);
double	compute_wood_factor(t_3vecf inter_point);
double	compute_marble_factor(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);

double	linear_interpolate(double a, double b, double val);

double	degree_to_radian(double degree);

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
int		parse_color_transp(char **line, t_3vecf *t_3vecf, int	i, double *val);
void	*parse_proc(char **line, t_text *text);
void	*parse_img(char **line, t_text *text);
int		parse_texture2(char **line, t_obj *obj/*, t_data *data*/);

int		parse_rotation(char **line, t_2vecf *t, int i);
int		parse_origin(char **line, t_3vecf *t, int i);

int		parse_cone(char **line, t_obj *cone, t_data *data);
int		parse_cylinder(char **line, t_obj *cylinder, t_data *data);
int		parse_plane(char **line, t_obj *plane, t_data *data);
int		parse_sphere(char **line, t_obj *sphere, t_data *data);

int		parse_ambient(char **line, t_light *light, t_data *data);

int		ray_intersect_cone(t_3vecf orig, t_3vecf dir, t_obj *cone, double *dist, double min_dist, double max_dist);
t_3vecf	get_normal_intersect_cone(t_3vecf inter_point, t_obj *cone);
t_2vecf	get_text_coordinate_cone(t_3vecf inter_point, t_3vecf normal_inter, t_obj *cone);

int 	ray_intersect_cylinder(t_3vecf orig, t_3vecf dir, t_obj *cylinder, double *dist, double min_dist, double max_dist);
t_3vecf	get_normal_intersect_cylinder(t_3vecf inter_point, t_obj *cylinder);
t_2vecf	get_text_coordinate_cylinder(t_3vecf inter_point, t_3vecf normal_inter, t_obj *cylinder);

int		ray_intersect_sphere(t_3vecf orig, t_3vecf dir, t_obj *sphere, double *dist, double min_dist, double max_dist);
t_3vecf	get_normal_intersect_sphere(t_3vecf inter_point, t_obj *sphere);
t_2vecf	get_text_coordinate_sphere(t_3vecf inter_point, t_3vecf normal_inter, t_obj *sphere);

int		ray_intersect_plane(t_3vecf orig, t_3vecf dir, t_obj *plane, double *dist, double min_dist, double max_dist);
t_3vecf	get_normal_intersect_plane(t_3vecf inter_point, t_obj *plane);
t_2vecf	get_text_coordinate_plane(t_3vecf inter_point, t_3vecf normal_inter, t_obj *plane);

void	print_conf(t_data *data);
void	print_vec2(double vec[2]);
void	print_vec(double vec[3]);
void	print_obj_param(t_obj *obj);

t_3vecf	get_uni_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_3vecf	get_grid_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_3vecf	get_perlin_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_3vecf	get_marble_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_3vecf	get_wood_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);
t_3vecf	get_image_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj);

#endif
