/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   cut.c                                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/02/03 23:03:19 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/28 05:22:19 by pduhard-         ###   ########lyon.fr   */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

t_obj	*check_cuts(t_3vecf orig, t_3vecf dir, t_obj *closest_obj, double min_dist, double max_dist, double *closest_dist, t_obj *objs, int sp_id, t_data *data, int negative)
{
	t_obj	*cuts;
//	t_obj	*next_obj;
//	double	next_dist;
//	double	cut_dist;
//	double	cut_dist_tmp;
//	t_obj	*cut_save;
	t_3vecf	inter_point;
	t_plane	*param;
	
	cuts = closest_obj->cuts;
	inter_point.val[0] = orig.val[0] + dir.val[0] * *closest_dist;
	inter_point.val[1] = orig.val[1] + dir.val[1] * *closest_dist;
	inter_point.val[2] = orig.val[2] + dir.val[2] * *closest_dist;

//	cut_dist = MAX_VIEW;
//	cut_save = NULL;
//	int	check = 0;
	while (cuts)
	{
		if (cuts->obj_type == OBJ_PLANE)
		{
			param = (t_plane *)cuts->obj_param;
			if (dot_product_3vecf(sub_3vecf(param->origin, inter_point), param->normal) > 0)
			{
			//	next_dist = MAX_VIEW;
			//	next_obj = ray_first_intersect(orig, dir, *closest_dist, max_dist, &next_dist, objs, sp_id, data);
			/*	cut_dist_tmp = MAX_VIEW;
				check = 1;
				if (cuts->ray_intersect(orig, dir, cuts, &cut_dist_tmp, *closest_dist, max_dist, sp_id))
				{
					inter_point.val[0] = orig.val[0] + dir.val[0] * cut_dist_tmp;
					inter_point.val[1] = orig.val[1] + dir.val[1] * cut_dist_tmp;
					inter_point.val[2] = orig.val[2] + dir.val[2] * cut_dist_tmp;
					if ((cut_dist_tmp > cut_dist || !cut_save) && closest_obj->check_inside(inter_point, closest_obj))
					{
						cut_dist = cut_dist_tmp;
						cut_save = cuts;
						//		return (cuts);
					}
				}
	*/		//	*closest_dist = next_dist;
			//	return (next_obj);
		//		return (ray_intersect_);
				return (negative ? NULL : ray_first_intersect(orig, dir, *closest_dist, max_dist, closest_dist, objs, sp_id, data));
			}
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
		/*	t_text_img	*text = (t_text_img *)closest_obj->text.text_param;

			coord2d = closest_obj->get_text_coordinate(inter_point, closest_obj->get_normal_inter(inter_point, closest_obj, sp_id), closest_obj);
			coord2d.val[0] *= closest_obj->text.scale.val[0];
			coord2d.val[1] *= closest_obj->text.scale.val[1];
			if (coord2d.val[1] < 0)
				col = (int)((1 - ((-coord2d.val[1] - (int)-coord2d.val[1]))) * (double)(text->width));
			else
				col = (int)((coord2d.val[1] - (int)coord2d.val[1]) * (double)(text->width));
			if (coord2d.val[0] < 0)
				row = (int)((1 - (-coord2d.val[0] - (int)-coord2d.val[0])) * (double)(text->height));
			else
				row = (int)((coord2d.val[0] - (int)coord2d.val[0]) * (double)(text->height));
			row += closest_obj->text.offset.val[0] * (double)(text->height);
			col += (closest_obj->text.offset.val[1]) * (double)(text->width);
	//		printf("%d %d => %d*%d\n", row, col, text->height, text->width);
		//	coord2d.val[0] += closest_obj->text.offset.val[0];
		//	coord2d.val[1] += closest_obj->text.offset.val[1];
			if (row > text->height || row < 0 || col > text->width || col < 0)
			*/
			if (coord2d.val[0] > 1 || coord2d.val[0] < 0 || coord2d.val[1] > 1 || coord2d.val[1] < 0)
				return (negative ? NULL : ray_first_intersect(orig, dir, *closest_dist, max_dist, closest_dist, objs, sp_id, data));
		}
		else if (cuts->obj_type == OBJ_SPHERE)
		{
			t_sphere	*param;

			param = (t_sphere *)cuts->obj_param;
			if (get_length_3vecf(sub_3vecf(inter_point, param->origin)) > param->radius)
				return (negative ? NULL : ray_first_intersect(orig, dir, *closest_dist, max_dist, closest_dist, objs, sp_id, data));
		}
		else if (cuts->obj_type == OBJ_CUBE)
		{
			t_cube	*param;

			param = (t_cube *)cuts->obj_param;
			if (inter_point.val[0] < param->x_range.val[0] || inter_point.val[0] > param->x_range.val[1]
				|| inter_point.val[1] < param->y_range.val[0] || inter_point.val[1] > param->y_range.val[1]
					|| inter_point.val[2] < param->z_range.val[0] || inter_point.val[2] > param->z_range.val[1])
				return (negative ? NULL : ray_first_intersect(orig, dir, *closest_dist, max_dist, closest_dist, objs, sp_id, data));
		}

		cuts = cuts->next;
	}
/*	if (cut_save)
	{
		next_dist = MAX_VIEW;
		next_obj = ray_first_intersect(orig, dir, *closest_dist, max_dist, &next_dist, objs, sp_id, data);
	//	exit(0);
		if (cut_dist < next_dist || !next_obj)
		{
			*closest_dist = cut_dist;
			return (cut_save);
		}
		*closest_dist = next_dist;
		return (next_obj);
		
	}
	if (check)
		return (ray_first_intersect(orig, dir, *closest_dist, max_dist, closest_dist, objs, sp_id, data));
*/	
	return (closest_obj);
	(void)min_dist;
}
