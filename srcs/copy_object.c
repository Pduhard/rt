/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_object.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 01:09:31 by aplat             #+#    #+#             */
/*   Updated: 2020/05/19 20:41:07 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		*copy_obj_param(void *obj_param, t_obj_type type)
{
	if (type == OBJ_SPHERE)
		return (ft_memcpy(ft_memalloc(sizeof(t_sphere)),
			obj_param, sizeof(t_sphere)));
	else if (type == OBJ_CYLINDER)
		return (ft_memcpy(ft_memalloc(sizeof(t_cylinder)),
			obj_param, sizeof(t_cylinder)));
	else if (type == OBJ_PLANE)
		return (ft_memcpy(ft_memalloc(sizeof(t_plane)),
			obj_param, sizeof(t_plane)));
	else if (type == OBJ_CONE)
		return (ft_memcpy(ft_memalloc(sizeof(t_cone)),
			obj_param, sizeof(t_cone)));
	else if (type == OBJ_TRIANGLE)
		return (ft_memcpy(ft_memalloc(sizeof(t_triangle))
			, obj_param, sizeof(t_triangle)));
	else
		return (NULL);
}

static t_cut	*copy_cut(t_cut *src)
{
	t_cut	*cut;

	cut = ft_memcpy(ft_memalloc(sizeof(t_cut)), src, sizeof(t_cut));
	if (src->cut_type == CUT_STATIC || src->cut_type == CUT_REAL)
		cut->cut_param = ft_memcpy(ft_memalloc(sizeof(t_cut_classic)),
			src->cut_param, sizeof(t_cut_classic));
	else if (src->cut_type == CUT_SPHERE)
		cut->cut_param = ft_memcpy(ft_memalloc(sizeof(t_sphere)),
			src->cut_param, sizeof(t_sphere));
	else if (src->cut_type == CUT_TEXTURE)
		cut->cut_param = NULL;
	else if (src->cut_type == CUT_UV)
		cut->cut_param = ft_memcpy(ft_memalloc(sizeof(t_cut_uv)),
			src->cut_param, sizeof(t_cut_uv));
	cut->next = NULL;
	return (cut);
}

static t_text	copy_text(t_text src)
{
	t_text		cpy;
	t_text_img	*p;

	cpy = src;
	if (src.text_type == TEXT_IMAGE)
	{
		if (!(cpy.text_param = malloc(sizeof(t_text_img))))
			ft_mem_error();
		p = (t_text_img *)src.text_param;
		((t_text_img *)cpy.text_param)->pixels = ft_memcpy(ft_memalloc(p->width
			* p->height * sizeof(int)), p->pixels, p->width *
				p->height * sizeof(int));
	}
	else
	{
		if (!(cpy.text_param = malloc(sizeof(t_text_proc))))
			ft_mem_error();
		*((t_text_proc *)cpy.text_param) = *((t_text_proc *)src.text_param);
	}
	return (cpy);
}

t_obj			*copy_object(t_obj *src)
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
	obj->text = copy_text(src->text);
	return (obj);
}
