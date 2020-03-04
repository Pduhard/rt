/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_composed_object.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 21:55:11 by pduhard-          #+#    #+#             */
/*   Updated: 2020/03/04 01:26:34 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	move_cut_plane(t_cut *cut, t_3vecf dir, double fact)
{
	t_plane	*param;

	param = (t_plane *)cut->cut_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
}

void	move_cut_sphere(t_cut *cut, t_3vecf dir, double fact)
{
	t_sphere	*param;

	param = (t_sphere *)cut->cut_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
}

void	move_cut_cube(t_cut *cut, t_3vecf dir, double fact)
{
	t_cube	*param;

	param = (t_cube *)cut->cut_param;
	param->x_range.val[0] += dir.val[0] * fact;
	param->x_range.val[1] += dir.val[1] * fact;
	param->y_range.val[0] += dir.val[0] * fact;
	param->y_range.val[1] += dir.val[1] * fact;
	param->z_range.val[0] += dir.val[0] * fact;
	param->z_range.val[1] += dir.val[1] * fact;
}

void	rotate_cut_plane(t_cut *cut, t_3vecf orig, t_33matf rot_mat[2])
{
	t_plane	*param;

	param = (t_plane *)cut->cut_param;
	param->origin = sub_3vecf(param->origin, orig);
	param->origin = mult_3vecf_33matf(param->origin, rot_mat[1]);
	param->origin = mult_3vecf_33matf(param->origin, rot_mat[0]);
	param->normal = mult_3vecf_33matf(param->normal, rot_mat[1]);
	param->normal = mult_3vecf_33matf(param->normal, rot_mat[0]);
	param->origin = add_3vecf(param->origin, orig);
}

int		parse_composed_model(char **line, t_data *data)
{
	char		stripe;
	int			ret;
	t_composed	*composed;

	stripe = 0;
	ret = 1;
	if (!(composed = ft_memalloc(sizeof(t_composed))))
		return (0);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n%s\n", *line);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		printf("\n%s\n", *line);
		if (!(ft_strncmp_case(*line, "objects", 7)))
			ret = parse_objects(line, data, composed);
		else if (!(ft_strncmp_case(*line, "name", 4)))
			ret = parse_name(line, &composed->name);
	}
	//exit(0);
	//	goto_next_element(line);
	printf("\n%s\n\n name : %s\n", *line, composed->name);
	if (data->composed_objs)
		composed->next = data->composed_objs;
	else
		composed->next = NULL;
	data->composed_objs = composed;
	return (ret);
}

void	*copy_obj_param(void *obj_param, t_obj_type type)
{
	if (type == OBJ_SPHERE)
		return (ft_memcpy(ft_memalloc(sizeof(t_sphere)), obj_param, sizeof(t_sphere)));
	else if (type == OBJ_CYLINDER)
		return (ft_memcpy(ft_memalloc(sizeof(t_cylinder)), obj_param, sizeof(t_cylinder)));
	else if (type == OBJ_PLANE)
		return (ft_memcpy(ft_memalloc(sizeof(t_plane)), obj_param, sizeof(t_plane)));
	else if (type == OBJ_CONE)
		return (ft_memcpy(ft_memalloc(sizeof(t_cone)), obj_param, sizeof(t_cone)));
	else if (type == OBJ_TRIANGLE)
		return (ft_memcpy(ft_memalloc(sizeof(t_triangle)), obj_param, sizeof(t_triangle)));
	return (NULL);
}

t_cut	*copy_cut(t_cut *src)
{
	t_cut	*cut;

	cut = ft_memcpy(ft_memalloc(sizeof(t_cut)), src, sizeof(t_cut));
	if (src->cut_type == CUT_STATIC || src->cut_type == CUT_REAL)
		cut->cut_param = ft_memcpy(ft_memalloc(sizeof(t_cut_classic)), src->cut_param, sizeof(t_cut_classic));
	else if (src->cut_type == CUT_CUBE)
		cut->cut_param = ft_memcpy(ft_memalloc(sizeof(t_cube)), src->cut_param, sizeof(t_cube));
	else if (src->cut_type == CUT_SPHERE)
		cut->cut_param = ft_memcpy(ft_memalloc(sizeof(t_sphere)), src->cut_param, sizeof(t_sphere));
	else if (src->cut_type == CUT_TEXTURE)
		cut->cut_param = NULL;//ft_memcpy(ft_memalloc(sizeof(t_plane)), src->cut_param, sizeof(t_sphere));
	else if (src->cut_type == CUT_UV)
		cut->cut_param = ft_memcpy(ft_memalloc(sizeof(t_cut_uv)), src->cut_param, sizeof(t_cut_uv));
	return (cut);
}

t_obj	*copy_object(t_obj *src)
{
	t_obj	*obj;
	t_cut	*cuts_obj;
	t_cut	*cuts;

	if (!(obj = ft_memalloc(sizeof(t_obj))))
		return (NULL);
	*obj = *src;
	cuts = src->cuts;
	obj->cuts = NULL;
	while (cuts)
	{
		if (!obj->cuts && !(obj->cuts = copy_cut(cuts)))
			return (NULL);
		else
		{
			if (!(cuts_obj = copy_cut(cuts)))
				return (NULL);
			cuts_obj->next = obj->cuts;
			obj->cuts = cuts_obj;
		}
		cuts = cuts->next;
	}
	obj->obj_param = copy_obj_param(src->obj_param, src->obj_type);
	obj->next = NULL;
	return (obj);
}
int		is_composed_object(char **line, t_data *data, int *ret)
{
	t_composed	*composed;
	t_composed	*list;
	//	t_obj		*obj;
	t_obj		**obj_tab;
	t_3vecf		origin;
	t_2vecf		rotation;
	t_33matf	rot_mat[2];
	int			i;
	char		stripe;

	stripe = '\0';
	list = data->composed_objs;
	//	printf("ICI?? :\n\n%s\n", *line);
	composed = NULL;
	while (list)
	{
		if (!ft_strncmp(*line, list->name, ft_strlen(list->name)))
			composed = list;
		list = list->next;
	}
	if (!composed)
		return (0);
	i = 0;
	while (composed->components[i])
		i++;
	if (!(obj_tab = ft_memalloc(sizeof(t_obj *) * (i + 1))))
		return (0);
	//		print_vec(rot_mat[1].val[0]);
	//	print_vec(rot_mat[1].val[1]);
	//	print_vec(rot_mat[1].val[2]);
	//	ft_putendl("sallllllllut\n");
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp_case(*line, "origin", 6)))
			*ret = parse_origin(line, &origin, 6);
		else if (!(ft_strncmp_case(*line, "rotation", 8)))
			*ret = parse_rotation(line, &rotation, 8);
		else
			;//error case
	}
	rot_mat[1] = init_rotation_matrix_y(degree_to_radian(rotation.val[1]));
	t_3vecf	tm = mult_3vecf_33matf(assign_3vecf(1, 0, 0), rot_mat[1]);
	rot_mat[0] = init_rotation_matrix_vec(tm, degree_to_radian(rotation.val[0]));

	//	ft_putendl("ici");
	i = 0;
	while (composed->components[i])
	{
		ft_putendl("ici ???");
		if (!(obj_tab[i] = copy_object(composed->components[i])))
			return (0);
		//	ft_putendl("salutt");
		//rotation
		obj_tab[i]->rotate(obj_tab[i], assign_3vecf(0, 0, 0), rot_mat);
		obj_tab[i]->move(obj_tab[i], origin, 1);
		//	printf("%p\n", composed->components[i]->ray_intersect);
		add_object(obj_tab[i], data);//composed->components[i], data);
		//	printf("%p %s \n", data->objs->ray_intersect, data->objs->obj_type == OBJ_TRIANGLE ? "tri" : "oups");
		i++;
	}
	i = 0;
	while (obj_tab[i])
	{
		//ft_putendl("icsfsdfsdfsdfi ???");
		obj_tab[i]->composed_origin = origin;
		obj_tab[i++]->composed_w = obj_tab;
	}
	/*	t_obj *o = data->objs;
		while (o)
		{
		printf("a %p %d\n", o->ray_intersect, o->obj_type);
		o = o->next;
		}
		*/	//	exit(0);
	return (1);
}
