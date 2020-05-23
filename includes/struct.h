/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/23 21:43:07 by aplat             #+#    #+#             */
/*   Updated: 2020/05/23 21:43:17 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

/*
**  Vector and matrix structure
*/

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

/*
**  Shape specific structure
*/

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

typedef struct	s_triangle
{
	t_3vecf		a;
	t_3vecf		b;
	t_3vecf		c;
	t_3vecf		origin;
}				t_triangle;

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

/*
**  Cut structure
*/

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
	t_cut_type		cut_type;
	void			*cut_param;
	void			(*move)(struct s_cut *, t_3vecf, double);
	void			(*rotate)(struct s_cut *, t_3vecf, t_33matf *);
	struct s_cut	*next;
}				t_cut;

/*
**  Texture structure
*/

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

/*
**  Mlx structure
*/

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

/*
**  Motion structure
*/

typedef	struct	s_motion
{
	t_3vecf			dir;
	double			speed_fact;
	int				spf;
	struct s_motion	*next;
}				t_motion;

/*
**  Other
*/

typedef	struct	s_pixel
{
	int			i;
	int			j;
}				t_pixel;

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
	void			*data;
	t_3vecf			composed_origin;
	struct s_obj	**composed_w;
}				t_obj;

typedef struct	s_comp_param
{
	t_3vecf		origin;
	t_2vecf		rotation;
}				t_comp_param;

typedef struct	s_composed
{
	char				*name;
	t_obj				**components;
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
	t_mlx			*mlx;
	t_mlx			*info;
	t_cam			*camera;
	t_obj			*objs;
	t_composed		*composed_objs;
	t_obj			*negative_objs;
	t_light			*lights;
	char			*scene_name;
	t_2vecf			size;
	int				hooks;
	t_33matf		rot_mat[3];
	int				anti_al;
	double			aa_adapt;
	int				cel_shading;
	int				motion_blur;
	int				stereoscopy;
	int				indirect_gi;
	int				caustics_gi;
	t_2vecf			fog;
	t_3vecf			(*apply_color_filter)(t_3vecf);
	t_kd_tree		*indirect_map;
	t_kd_tree		*caustic_map;
	t_obj			*selected_obj;
	char			*skybox_name;
	int				to_next;
	int				new_obj;
	int				first_loop;
	unsigned char	permutation[512];
	t_3vecf			gradient[16];
	double			water_f;
	int				water;
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
	t_3vecf		inter_point;
	t_3vecf		normal_inter;
	t_photon	**tab;
	double		*closest;
	double		*farest;
	int			nn_photon;
	int			*actual_nn;
}				t_nn_param;

typedef struct	s_phtn_cast
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

typedef struct	s_phtn_prob
{
	double		absorb_prob;
	double		refract_prob;
	double		reflect_prob_spe;
	double		reflect_prob_dif;
}				t_phtn_prob;

typedef struct	s_compute_light_param
{
	t_3vecf		inter_point;
	t_3vecf		normal_inter;
	t_3vecf		dir;
	t_light		*lights;
	t_obj		*objs;
	int			sp_id;
	t_data		*data;
	double		shininess;
}				t_compute_light_param;

typedef struct	s_inter
{
	t_3vecf		inter_point;
	t_3vecf		normal_inter;
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

typedef struct	s_rayt_param
{
	t_3vecf *lighted_color;
	t_3vecf inv_dir;
	t_4vecf	obj_color;
	t_data	*data;
	int		depth;
	int		sp_id;
}				t_rayt_param;

typedef struct	s_clre_param
{
	double	closest_dist;
	t_leq	l;
	int		depth;
}				t_clre_param;

typedef struct	s_anti_al
{
	int	anti_al_iter;
	int	offset;
	int	aa;
}				t_anti_al;

#endif
