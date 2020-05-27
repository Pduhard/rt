/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proto.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/19 20:54:03 by aplat             #+#    #+#             */
/*   Updated: 2020/05/27 20:39:18 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTO_H
# define PROTO_H

/*
**	Parsing functions
*/

int				parse_rt_conf(char *file_name, t_data *data);
int				parse_3vecf(char *line, int i, t_3vecf *vec);
int				parse_4vecf(char *line, int i, t_4vecf *vec);
int				parse_double(char **line, int i, double *val);
int				parse_2vecf(char *line, int i, t_2vecf *vec);
int				parse_int(char **line, int i, int *val);
void			clamp_and_set_dflt(t_obj *obj);
t_obj			*copy_object(t_obj *src);
int				parse_scene(char **line, t_data *data);
int				parse_composed_model(char **line, t_data *data);
int				is_composed_object(char **line, t_data *data, int *ret);
char			goto_next_element(char **line);
int				parse_name(char **line, char **name, int i);
int				parse_objects(char **line, t_data *data, t_composed *from);
int				parse_lights(char **line, t_data *data);
int				parse_camera(char **line, t_data *data);
void			*parse_proc(char **line);
void			*parse_img(char *name);
void			*parse_texture_img(char **line);
int				parse_texture2(char **line, t_obj *obj);
int				parse_bump_mapping(char **line, t_obj *obj);
void			set_bump_own(t_obj *obj);
int				parse_bump_inde(char **line, t_obj *obj, int index);
int				parse_motion(char **line, t_obj *obj);
int				parse_rotation(char **line, t_2vecf *t, int i);
int				parse_origin(char **line, t_3vecf *t, int i);
int				parse_cone(char **line, t_obj *cone);
int				parse_cylinder(char **line, t_obj *cylinder);
int				parse_plane(char **line, t_obj *plane);
int				parse_triangle(char **line, t_obj *triangle);
int				parse_sphere(char **line, t_obj *sphere);
int				parse_ellipsoid(char **line, t_obj *ellipsoid);
int				parse_hyperboloid(char **line, t_obj *hyperboloid);
int				parse_horse_saddle(char **line, t_obj *horse_saddle);
int				parse_monkey_saddle(char **line, t_obj *monkey_saddle);
int				parse_cyclide(char **line, t_obj *cyclide);
int				parse_fermat(char **line, t_obj *fermat);
int				parse_moebius(char **line, t_obj *moebius);
int				parse_ambient(char **line, t_light *light, t_data *data);
int				pick_type_cutting(char **line, t_cut *cut, int *ret);
int				parse_cutting(char **line, t_obj *obj);
void			push_object(t_obj *obj, int composed, t_data *data,
				t_composed *from);

int				parse_sphere_element(char **line, int *ret,
				t_sphere *sphere_param);
int				check_sphere_param(t_sphere *sphere_param, int ret);
int				parse_cut_uv(char **line, t_cut *cut);
int				parse_cut_static_real(char **line, t_cut *cut,
				t_cut_type cut_type);
void			add_object(t_obj *obj, t_data *data);
int				parse_color_filter(char **line, t_data *data);
int				parse_material(char **line, int i, t_obj *obj);

int				return_update(char *error, int ret, int skip);
int				clamp_val(double *val, double min, double max);
int				check_lights_cam(t_data *data);
int				check_skybox(t_data *data);
int				check_macro(void);
int				check_quality(void);

/*
**	Vector functions
*/

t_3vecf			assign_3vecf(double x, double y, double z);
t_2vecf			assign_2vecf(double x, double y);
void			normalize_3vecf(t_3vecf *vec);
double			get_length_3vecf(t_3vecf vec);
t_3vecf			sub_3vecf(t_3vecf a, t_3vecf b);
t_3vecf			add_3vecf(t_3vecf a, t_3vecf b);
t_3vecf			add_c3vecf(t_3vecf a, double c);
t_3vecf			product_3vecf(t_3vecf a, t_3vecf b);
t_3vecf			product_c3vecf(t_3vecf a, double c);
double			dot_product_3vecf(t_3vecf a, t_3vecf b);
double			dot_product_2vecf(t_2vecf a, t_2vecf b);
int				is_null_3vecf(t_3vecf vec);
t_3vecf			neg_3vecf(t_3vecf vec);
t_3vecf			mult_3vecf_33matf(t_3vecf vect, t_33matf mat);
void			mult_vec_matrix(t_3vecf a, t_44matf mat, t_3vecf *dst);
void			mult_dir_matrix(t_3vecf a, t_44matf mat, t_3vecf *dst);
t_3vecf			move_3vecf(t_3vecf a, t_motion *b, int c);
t_obj			*ray_first_intersect(t_leq l, t_dist dist, t_obj *objs,
				int sp_id);
t_3vecf			apply_color_filter_sepia(t_3vecf color);

/*
**	Error functions
*/

int				error_parse_object(t_obj *obj);
int				syn_error(char *s1, char *s2, char*s3, char *s4);
int				error(char *s1, char *s2);
int				ft_memalloc_error(int ret, size_t size);
void			ft_mem_error(void);

/*
**	Ray trace functions
*/

t_3vecf			window_to_view(double x, double y, double win_w, double win_h);
void			render(t_data *data);
void			compute_stereoscopy(t_data *data, t_leq l, int i, int j);
void			compute_classic(t_data *data, t_leq l, int i, int j);
void			check_subsampling(t_data *data, int i, int j, t_3vecf color);
void			ray_put_pixel(t_pixel cmp, int *img, t_3vecf color,
				t_data *data);
int				clip_color(double color);
t_3vecf			get_refl_color(t_rayt_param p, t_inter i);
t_3vecf			get_refr_color(t_rayt_param p, t_inter i, t_leq l, t_obj *obj);
t_3vecf			compute_glare(t_leq l, t_light *lights, t_3vecf *inter_point);
t_3vecf			add_color_effect(t_data *data, t_clre_param p,
				t_3vecf lighted_color, t_inter i);
t_3vecf			compute_lights(t_compute_light_param p);
void			cel_shade(double *val);
void			clamp_transparency(t_3vecf *transp_fact);
t_obj			*check_for_shadow(t_3vecf *transp_fact, t_3vecf light_dir,
				double light_len, t_compute_light_param *p);
t_dist			new_tdist(double *cdist);
t_3vecf			refract_ray(t_3vecf dir, t_3vecf normal_inter,
				double refraction_index);
t_3vecf			reflect_ray(t_3vecf dir, t_3vecf normal_inter);
double			compute_fresnel_ratio(t_3vecf dir, t_3vecf normal_inter,
				double refraction_index);
t_3vecf			ray_trace(t_leq l, t_data *data, int depth, int sp_id);
t_3vecf			motion_trace(t_3vecf orig, t_3vecf dir, t_data *data);
void			compute_fog(t_data *data, double closest_dist,
				t_3vecf *lighted_color);
t_4vecf			get_obj_color(t_inter *i, t_obj *closest_obj, t_leq l);

/*
**	Shape relative functions
*/

int				is_closest_intersect(t_dist dist, double root);
void			get_uv_axis(t_3vecf axis[3], t_3vecf first_axis);

/*
**	Sphere relative functions
*/

void			generate_new_sphere(t_data *data);
void			assign_sphere_function(t_obj *sphere);
t_3vecf			get_sphere_origin(t_obj *sphere, t_sphere *sphere_param,
				int sp_id);
int				ray_intersect_sphere(t_leq l, t_obj *sphere, t_dist dist,
				int sp_id);
int				check_inside_sphere(t_3vecf point, t_obj *sphere);
t_3vecf			get_normal_intersect_sphere(t_3vecf inter_point,
				t_obj *sphere, int a);
t_3vecf			get_origin_sphere(t_obj *a);
void			move_sphere(t_obj *a, t_3vecf b, double c);
void			rotate_sphere(t_obj *cone, t_3vecf b, t_33matf *c);
t_2vecf			get_text_coordinate_sphere(t_3vecf inter_point,
				t_3vecf normal_inter, t_obj *sphere);

/*
**	Plane relative functions
*/

void			generate_new_plane(t_data *data);
void			assign_plane_function(t_obj *plane);
t_3vecf			get_plane_origin(t_obj *plane, t_plane *plane_param,
				int sp_id);
int				ray_intersect_plane(t_leq l, t_obj *plane, t_dist dist,
				int sp_id);
int				check_inside_plane(t_3vecf point, t_obj *plane);
t_3vecf			get_normal_intersect_plane(t_3vecf inter_point,
				t_obj *plane, int sp_id);
t_3vecf			get_origin_plane(t_obj *a);
void			move_plane(t_obj *a, t_3vecf b, double c);
void			rotate_plane(t_obj *cone, t_3vecf a, t_33matf *b);
t_2vecf			get_text_coordinate_plane(t_3vecf inter_point,
				t_3vecf normal_inter, t_obj *plane);

/*
**	Cone relative functions
*/

void			generate_new_cone(t_data *data);
void			assign_cone_function(t_obj *cone);
t_3vecf			get_cone_origin(t_obj *cone, t_cone *cone_param, int sp_id);
t_3vecf			get_cone_tip(t_obj *cone, t_cone *cone_param, int sp_id);
int				ray_intersect_cone(t_leq l, t_obj *cone, t_dist dist,
				int sp_id);
int				check_inside_cone(t_3vecf point, t_obj *cone);
t_3vecf			get_normal_intersect_cone(t_3vecf inter_point, t_obj *cone,
				int sp_id);
t_3vecf			get_origin_cone(t_obj *cone);
void			move_cone(t_obj *cone, t_3vecf a, double b);
void			rotate_cone(t_obj *cone, t_3vecf b, t_33matf *c);
t_2vecf			get_text_coordinate_cone(t_3vecf inter_point,
				t_3vecf normal_inter, t_obj *cone);
/*
**	Cylinder relative functions
*/

void			generate_new_cylinder(t_data *data);
void			assign_cylinder_function(t_obj *cylinder);
t_3vecf			get_cylinder_origin(t_obj *cylinder,
				t_cylinder *cylinder_param, int sp_id);
t_3vecf			get_cylinder_tip(t_obj *cylinder, t_cylinder *cylinder_param,
				int sp_id);
int				ray_intersect_cylinder(t_leq l, t_obj *cylinder, t_dist dist,
				int sp_id);
int				check_inside_cylinder(t_3vecf point, t_obj *cylinder);
t_3vecf			get_normal_intersect_cylinder(t_3vecf inter_point,
				t_obj *cylinderi, int sp_id);
t_3vecf			get_origin_cylinder(t_obj *a);
void			move_cylinder(t_obj *a, t_3vecf b, double c);
void			rotate_cylinder(t_obj *cone, t_3vecf b, t_33matf *c);
t_2vecf			get_text_coordinate_cylinder(t_3vecf inter_point,
				t_3vecf normal_inter, t_obj *cylinder);

/*
**	Triangle relative functions
*/

void			generate_new_triangle(t_data *data);
void			assign_triangle_function(t_obj *triangle);
int				ray_intersect_triangle(t_leq l, t_obj *triangle, t_dist dist,
				int sp_id);
t_3vecf			get_normal_intersect_triangle(t_3vecf inter_point,
				t_obj *triangle, int a);
t_3vecf			get_origin_triangle(t_obj *a);
void			move_triangle(t_obj *a, t_3vecf b, double c);
void			rotate_triangle(t_obj *cone, t_3vecf b, t_33matf *c);
t_2vecf			get_text_coordinate_triangle(t_3vecf inter_point,
				t_3vecf normal_inter, t_obj *triangle);
/*
**	Moebius relative functions
*/

void			generate_new_moebius(t_data *data);
void			assign_moebius_function(t_obj *moebius);
t_3vecf			get_moebius_origin(t_obj *moebius, t_moebius *param,
				int sp_id);
int				ray_intersect_moebius(t_leq l, t_obj *moebius, t_dist dist,
				int sp_id);
int				check_inside_moebius(t_3vecf point, t_obj *moebius);
t_3vecf			get_normal_intersect_moebius(t_3vecf inter_point,
				t_obj *moebius, int sp_id);
t_3vecf			get_origin_moebius(t_obj *a);
void			move_moebius(t_obj *a, t_3vecf b, double c);
t_2vecf			get_text_coordinate_moebius(t_3vecf inter_point,
				t_3vecf normal_inter, t_obj *moebius);

/*
**	Ellipsoid relative functions
*/

void			generate_new_ellipsoid(t_data *data);
void			assign_ellipsoid_function(t_obj *ellipsoid);
t_3vecf			get_ellipsoid_origin(t_obj *ellipsoid,
				t_ellipsoid *ellipsoid_param, int sp_id);
int				ray_intersect_ellipsoid(t_leq l, t_obj *ellipsoid, t_dist dist,
				int sp_id);
int				check_inside_ellipsoid(t_3vecf point, t_obj *ellipsoid);
t_3vecf			get_normal_intersect_ellipsoid(t_3vecf inter_point,
				t_obj *ellipsoid, int a);
t_3vecf			get_origin_ellipsoid(t_obj *a);
void			move_ellipsoid(t_obj *a, t_3vecf b, double c);
t_2vecf			get_text_coordinate_ellipsoid(t_3vecf inter_point,
				t_3vecf normal_inter, t_obj *ellipsoid);

/*
**	Hyperboloid relative functions
*/

void			generate_new_hyperboloid(t_data *data);
void			assign_hyperboloid_function(t_obj *hyperboloid);
t_3vecf			get_hyperboloid_origin(t_obj *hyperboloid,
				t_hyperboloid *hyperboloid_param, int sp_id);
int				ray_intersect_hyperboloid(t_leq l, t_obj *hyperboloid,
				t_dist dist, int sp_id);
int				check_inside_hyperboloid(t_3vecf point, t_obj *hyperboloid);
t_3vecf			get_normal_intersect_hyperboloid(t_3vecf inter_point,
				t_obj *hyperboloid, int a);
t_3vecf			get_origin_hyperboloid(t_obj *a);
void			move_hyperboloid(t_obj *a, t_3vecf b, double c);
t_2vecf			get_text_coordinate_hyperboloid(t_3vecf inter_point,
				t_3vecf normal_inter, t_obj *hyperboloid);

/*
**	Horse saddle relative functions
*/

void			generate_new_horse_saddle(t_data *data);
void			assign_horse_saddle_function(t_obj *horse_saddle);
t_3vecf			get_horse_saddle_origin(t_obj *horse_saddle,
				t_horse_saddle *param, int sp_id);
int				ray_intersect_horse_saddle(t_leq l, t_obj *horse_saddle,
				t_dist dist, int sp_id);
int				check_inside_horse_saddle(t_3vecf point, t_obj *horse_saddle);
t_3vecf			get_normal_intersect_horse_saddle(t_3vecf inter_point,
				t_obj *horse_saddle, int a);
t_3vecf			get_origin_horse_saddle(t_obj *a);
void			move_horse_saddle(t_obj *a, t_3vecf b, double c);
t_2vecf			get_text_coordinate_horse_saddle(t_3vecf inter_point,
				t_3vecf normal_inter, t_obj *horse_saddle);

/*
**	Monkey saddle relative functions
*/

void			generate_new_monkey_saddle(t_data *data);
void			assign_monkey_saddle_function(t_obj *monkey_saddle);
t_3vecf			get_monkey_saddle_origin(t_obj *monkey_saddle,
				t_monkey_saddle *param, int sp_id);
int				ray_intersect_monkey_saddle(t_leq l, t_obj *monkey_saddle,
				t_dist dist, int sp_id);
int				check_inside_monkey_saddle(t_3vecf point,
				t_obj *monkey_saddle);
t_3vecf			get_normal_intersect_monkey_saddle(t_3vecf inter_point,
				t_obj *monkey_saddle, int a);
t_3vecf			get_origin_monkey_saddle(t_obj *a);
void			move_monkey_saddle(t_obj *a, t_3vecf b, double c);
t_2vecf			get_text_coordinate_monkey_saddle(t_3vecf inter_point,
				t_3vecf normal_inter, t_obj *monkey_saddle);

/*
**	Cyclide relative functions
*/

void			generate_new_cyclide(t_data *data);
void			assign_cyclide_function(t_obj *cyclide);
t_3vecf			get_cyclide_origin(t_obj *cyclide, t_cyclide *param,
				int sp_id);
int				ray_intersect_cyclide(t_leq l, t_obj *cyclide, t_dist dist,
				int sp_id);
int				check_inside_cyclide(t_3vecf point, t_obj *cyclide);
t_3vecf			get_normal_intersect_cyclide(t_3vecf inter_point,
				t_obj *cyclide, int sp_id);
t_3vecf			get_origin_cyclide(t_obj *cyclide);
void			move_cyclide(t_obj *cyclide, t_3vecf b, double c);
t_2vecf			get_text_coordinate_cyclide(t_3vecf inter_point,
				t_3vecf normal_inter, t_obj *cyclide);

/*
**	Fermat relative functions
*/

void			generate_new_fermat(t_data *data);
void			assign_fermat_function(t_obj *fermat);
t_3vecf			get_fermat_origin(t_obj *fermat, t_fermat *param, int sp_id);
int				ray_intersect_fermat(t_leq l, t_obj *fermat, t_dist dist,
				int sp_id);
int				check_inside_fermat(t_3vecf point, t_obj *fermat);
t_3vecf			get_normal_intersect_fermat(t_3vecf inter_point, t_obj *fermat,
				int a);
t_3vecf			get_origin_fermat(t_obj *a);
void			move_fermat(t_obj *a, t_3vecf b, double c);
t_2vecf			get_text_coordinate_fermat(t_3vecf inter_point,
				t_3vecf normal_inter, t_obj *fermat);

/*
**	Global illumination functions
*/

t_3vecf			compute_global_illumination(t_3vecf inter_point,
				t_3vecf normal_inter, t_kd_tree *photon_map, int nn_photon);
t_3vecf			compute_radiance_estimation(t_photon **nearest_n,
				t_3vecf inter_point, double farest, int nn_photon);
void			cast_photon(t_leq l, t_phtn_cast p);
void			check_photon_map(t_data *data);
void			cast_photon(t_leq l, t_phtn_cast p);
t_3vecf			add_color_bleed(t_3vecf pwr, t_4vecf obj_color);
void			refract_photon(t_leq l, t_phtn_cast p);
void			reflect_photon_spec(t_leq l, t_phtn_cast p, int photon_type);
void			reflect_photon_diff(t_leq l, t_phtn_cast p,
				t_photon photon, t_3vecf normal_inter);
void			absorb_photon(t_leq l, t_phtn_cast p, t_photon photon);
t_phtn_prob		get_prob(double absorb_prob, double refract_prob,
				double reflect_prob);
void			scatter_photon(t_photon **photon_tab, t_data *data);
t_kd_tree		*build_tree(t_photon *phtn_tab, int lw, int hi, int ax);

/*
**	Random functions
*/

t_text			generate_random_texture(t_obj *obj);
t_4vecf			generate_random_color(unsigned int x, double transp_f);
int				generate_random_enum(int e_max);
t_3vecf			get_random_dir(int rand_iter);
double			get_random_number(unsigned int x);

/*
**	Free functions
*/

void			delete_object(t_data *data, t_obj *obj);
void			free_all(t_data *data);
void			free_info(t_data *data);
void			free_data(t_data *data);
void			free_object(t_obj *obj);
void			free_composed_components(t_obj **obj_tab);
void			free_cuts(t_cut *cuts);
void			free_motions(t_motion *motions);

/*
**	Utils functions
*/

int				close_cross(t_data *data);
int				open_info(t_data *data);

/*
**	Init functions
*/

t_data			*init_data(char *file_name, t_mlx *mlx);
void			init_perlin(t_data *data);
t_33matf		init_rotation_matrix_x(double theta);
t_33matf		init_rotation_matrix_y(double theta);
t_33matf		init_rotation_matrix_z(double theta);
t_33matf		init_rotation_matrix_vec(t_3vecf a, double b);
void			init_threads(t_thread threads[NB_THREADS], t_data *data);
void			init_frames_rot_mat(t_data *data);
t_anti_al		init_anti_al(int aa, int offset, int anti_al_iter);
t_3vecf			init_ray_dir(int i, int j, t_anti_al a, t_data *data);
t_inter			init_inter(t_leq l, double closest_dist,
				t_obj *closest_obj, int sp_id);
int				init_lighted_color(t_obj *closest_obj, t_inter i,
				t_leq l, t_ilc_p p);

/*
**	Texture relative functions
*/

void			handle_perlin_inter_point(t_3vecf *int_part,
				t_3vecf *floating_part, t_3vecf inter_point, double scale);
void			set_quintic_factors(double q[3], t_3vecf f);
double			compute_3dperlin_factor(t_3vecf inter_point, double scale,
				const unsigned char permutation[512],
				const t_3vecf gradient[16]);
double			compute_3dfbm_factor(t_3vecf inter_point, double scale);
double			compute_wood_factor(t_3vecf inter_point, double scale,
				const unsigned char permutation[512],
				const t_3vecf gradient[16]);
double			compute_marble_factor(t_3vecf inter_point, double scale,
				const unsigned char permutation[512],
				const t_3vecf gradient[16]);
double			linear_interpolate(double a, double b, double val);
t_4vecf			get_uni_color(t_3vecf inter_point, t_3vecf normal_inter,
				t_obj *obj);
t_4vecf			get_grid_color(t_3vecf inter_point, t_3vecf normal_inter,
				t_obj *obj);
t_4vecf			get_perlin_color(t_3vecf inter_point, t_3vecf normal_inter,
				t_obj *obj);
t_4vecf			get_fbm_color(t_3vecf inter_point, t_3vecf normal_inter,
				t_obj *obj);
t_4vecf			get_marble_color(t_3vecf inter_point, t_3vecf normal_inter,
				t_obj *obj);
t_4vecf			get_wood_color(t_3vecf inter_point, t_3vecf normal_inter,
				t_obj *obj);
t_4vecf			get_image_color(t_3vecf inter_point, t_3vecf normal_inter,
				t_obj *obj);
t_3vecf			get_bump_mapping_water(t_3vecf inter_point,
				t_3vecf normal_inter, t_obj *obj);
t_3vecf			get_bump_mapping_perlin(t_3vecf inter_point,
				t_3vecf normal_inter, t_obj *obj);
t_3vecf			get_bump_mapping_fbm(t_3vecf inter_point, t_3vecf normal_inter,
				t_obj *obj);
t_3vecf			get_bump_mapping_marble(t_3vecf inter_point,
				t_3vecf normal_inter, t_obj *obj);
t_3vecf			get_bump_mapping_wood(t_3vecf inter_point,
				t_3vecf normal_inter, t_obj *obj);
void			set_shininess(double *shininess);

/*
**	Hook functions
*/

int				key_press(int keycode, void *param);
int				key_release(int keycode, void *param);
int				check_pscreen_key_press(int keycode, t_data *data);
int				check_gen_key_press(int keycode, t_data *data);
int				check_delete_key_press(int keycode, t_data *data);
int				check_switch_key_press(int keycode, t_data *data);
int				check_esc_key_press(int keycode, t_data *data);
int				check_mov_key_press(int keycode, t_data *data);
int				check_rot_key_press(int keycode, t_data *data);
int				mouse_hook(int button, int x, int y, void *param);

/*
**	Real-time rendering functions
*/

int				print_loop_image(void *param);
void			manage_obj_move(t_data *data, int *ret);
void			manage_obj_rotation(t_data *data, int *ret);
void			manage_cam_rotation(t_data *data, int *ret);
void			manage_cam_move(t_data *data, int *ret);

/*
**	Math functions
*/

int				is_null(double value);
int				is_null_3vecf(t_3vecf t);
double			degree_to_radian(double degree);
t_3vecf			solve_cubic(double a, double b, double c, double d);
t_2vecf			solve_quadratic(double a, double b, double c);

/*
**	Cut relative functions
*/

t_obj			*check_cuts(t_leq l, t_dist dist, t_cut_fparam cp,
				t_data *data);
void			move_cut_plane(t_cut *cut, t_3vecf dir, double fact);
void			move_cut_sphere(t_cut *cut, t_3vecf dir, double fact);
void			move_cut_cube(t_cut *cut, t_3vecf dir, double fact);

void			push_object(t_obj *obj, int composed, t_data *data,
				t_composed *from);

int				parse_sphere_element(char **line, int *ret,
				t_sphere *sphere_param);
int				check_sphere_param(t_sphere *sphere_param, int ret);
int				parse_cut_uv(char **line, t_cut *cut);
int				parse_cut_static_real(char **line, t_cut *cut,
				t_cut_type cut_type);
void			free_composed_components(t_obj **obj_tab);
void			rotate_cut_plane(t_cut *cut, t_3vecf orig,
				t_33matf rot_mat[2]);
t_cut			*copy_cut(t_cut *src);

#endif
