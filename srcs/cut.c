/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   cut.c                                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/02/03 23:03:19 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/06 07:26:15 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

t_obj	*check_cuts(t_3vecf orig, t_3vecf dir, t_obj *closest_obj, double min_dist, double max_dist, double *closest_dist, t_obj *objs, int sp_id)
{
	t_obj	*cuts;
//	t_obj	*next_obj;
//	double	next_dist;
//	double	cut_dist;
	t_3vecf	inter_point;
	t_plane	*param;
	
	cuts = closest_obj->cuts;
	inter_point.val[0] = orig.val[0] + dir.val[0] * *closest_dist;
	inter_point.val[1] = orig.val[1] + dir.val[1] * *closest_dist;
	inter_point.val[2] = orig.val[2] + dir.val[2] * *closest_dist;
	
	while (cuts)
	{
		if (cuts->obj_type == OBJ_PLANE)
		{
			param = (t_plane *)cuts->obj_param;
			if (dot_product_3vecf(sub_3vecf(param->origin, inter_point), param->normal) > 0)
				return (ray_first_intersect(orig, dir, *closest_dist, max_dist, closest_dist, objs, sp_id));
		}
		else if (cuts->obj_type == OBJ_CUT_TEXTURE)
		{
		//	printf("ABCD\n");
			t_2vecf	coord2d;
			coord2d = closest_obj->get_text_coordinate(inter_point, closest_obj->get_normal_inter(inter_point, closest_obj, sp_id), closest_obj);
			coord2d.val[0] *= closest_obj->text.scale.val[0];
			coord2d.val[1] *= closest_obj->text.scale.val[1];
			coord2d.val[0] += closest_obj->text.offset.val[0];
			coord2d.val[1] += closest_obj->text.offset.val[1];
			if (coord2d.val[0] > 1 || coord2d.val[0] < 0 || coord2d.val[1] > 1 || coord2d.val[1] < 0)
				return (ray_first_intersect(orig, dir, *closest_dist, max_dist, closest_dist, objs, sp_id));
		}
	//	else if ()
		cuts = cuts->next;
	}
	return (closest_obj);
	(void)min_dist;
}
