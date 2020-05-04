/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   photon_map.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/02/11 10:49:10 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/03/05 10:54:39 by pduhard-         ###   ########lyon.fr   */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

int			check_light_type(t_light *light)
{
	while (light)
	{
		if (light->light_type != LIGHT_AMBIENT)
			return (1);
		light = light->next;
	}
	return (0);
}

void	cast_photon(t_3vecf orig, t_3vecf dir, t_light *light, t_data *data, int *ind_i, int *caus_i, t_photon **photon_tab, int depth, unsigned int rand_iter, t_3vecf pwr, int photon_type)
{
	t_photon	photon;
	t_obj		*obj;
	double		dist;
	double		rr_f;
//	int			depth;
	int			inside;

//	depth = 0;
	inside = 0;
//	photon.position = orig;
//	while (depth++ < PHOTON_DEPTH)
//	{
		if (!depth || is_null_3vecf(pwr) || !(obj = ray_first_intersect(orig, dir, BIAS, MAX_VIEW, &dist, data->objs, 0, data)))
			return ;
		rr_f = get_random_number(rand_iter * 0xfab1dede << ((rand_iter + depth) & 7));
		photon.position.val[0] = orig.val[0] + dir.val[0] * dist;
		photon.position.val[1] = orig.val[1] + dir.val[1] * dist;
		photon.position.val[2] = orig.val[2] + dir.val[2] * dist;
//	printf("rusian %f\n", rr_f);
		if (obj->refraction)
		{
			t_3vecf	normal_inter = obj->get_normal_inter(photon.position, obj, 0);
			t_4vecf	obj_color = obj->get_text_color(photon.position, normal_inter, obj);

			if (dot_product_3vecf(normal_inter, dir) > 0 && (inside = 1))
				normal_inter = assign_3vecf(-normal_inter.val[0], -normal_inter.val[1], -normal_inter.val[2]);
			if (obj->get_bump_mapping)
				normal_inter = obj->get_bump_mapping(photon.position, normal_inter, obj);

			double	fresnel_ratio = compute_fresnel_ratio(dir, normal_inter, obj->refraction, 0);//inside);
			double	absorb_prob = (1 - fresnel_ratio) * (1 - obj_color.val[3]);
			double	refract_prob = (1 - fresnel_ratio) * obj_color.val[3];
			double	reflect_prob = fresnel_ratio;
			double	reflect_prob_spe = reflect_prob * SPEC_PROB;
			double	reflect_prob_dif = reflect_prob * DIFF_PROB;
		//	double	reflect_prob = fresnel_ratio;

			printf("refr abs %f refr %f refl sp %f refl diff %f => %f\n", absorb_prob, refract_prob, reflect_prob_spe, reflect_prob_dif, absorb_prob + refract_prob + reflect_prob_spe + reflect_prob_dif);
			if (rr_f < absorb_prob) //absorb
			{
				if (depth == PHOTON_DEPTH)
					return ;
				photon.color.val[0] = pwr.val[0] / (photon_type == 1 ? (double)NB_INDIRECT_PHOTON : (double)NB_CAUSTIC_PHOTON);
				photon.color.val[1] = pwr.val[1] / (photon_type == 1 ? (double)NB_INDIRECT_PHOTON : (double)NB_CAUSTIC_PHOTON);
				photon.color.val[2] = pwr.val[2] / (photon_type == 1 ? (double)NB_INDIRECT_PHOTON : (double)NB_CAUSTIC_PHOTON);
		//		photon.color.val[0] = pwr.val[0] / (photon_type == 1 ? (double)NB_INDIRECT_PHOTON : (double)NB_INDIRECT_PHOTON);
		//		photon.color.val[1] = pwr.val[1] / (photon_type == 1 ? (double)NB_INDIRECT_PHOTON : (double)NB_INDIRECT_PHOTON);
		//		photon.color.val[2] = pwr.val[2] / (photon_type == 1 ? (double)NB_INDIRECT_PHOTON : (double)NB_INDIRECT_PHOTON);
				photon.direction = assign_3vecf(-dir.val[0], -dir.val[1], -dir.val[2]);
				if (photon_type == 1 && *ind_i != NB_INDIRECT_PHOTON)
				{
					photon_tab[1][*ind_i] = photon;
					*ind_i += 1;
					printf("indirect photon refract New photon %f %f %f after %d bounce\n", photon.position.val[0], photon.position.val[1], photon.position.val[2], PHOTON_DEPTH - depth);
				}
				if (photon_type == 0 && *caus_i != NB_CAUSTIC_PHOTON)
				{
					photon_tab[0][*caus_i] = photon;
					*caus_i += 1;
					printf("caustic photon refract New photon %f %f %f after %d bounce\n", photon.position.val[0], photon.position.val[1], photon.position.val[2], PHOTON_DEPTH - depth);
				}
				return ;
			}
			else
			{
				pwr.val[0] *= obj_color.val[0];
				pwr.val[1] *= obj_color.val[1];
				pwr.val[2] *= obj_color.val[2];
			}
			if (rr_f < refract_prob + absorb_prob) //refract
			{
				printf("salut refraction \n");
				dir = refract_ray(dir, normal_inter, obj->refraction, 0);//inside);
				normalize_3vecf(&dir);
				cast_photon(photon.position, dir, light, data, ind_i, caus_i, photon_tab, depth - 1, rand_iter + 1, pwr, photon_type);
				return ;
			}
			else if (rr_f < refract_prob + absorb_prob + reflect_prob_spe) // = reflect
			{
				dir = reflect_ray(assign_3vecf(-dir.val[0], -dir.val[1], -dir.val[2]), normal_inter);
				normalize_3vecf(&dir);
				cast_photon(photon.position, dir, light, data, ind_i, caus_i, photon_tab, depth - 1, rand_iter + 1, pwr, photon_type);
				//cast_photon(photon.position, dir, light, data, i, photon_tab, depth - 1, rand_iter + 1, pwr);
				return ;
			}
			else // reflect diffuse
			{

				dir = assign_3vecf(get_random_number(rand_iter * 0xcac00aca << ((rand_iter + 23) % 23)) - 0.5, get_random_number(rand_iter * 0x123fddef << ((rand_iter + 5) % 16)) - 0.5, get_random_number(rand_iter * 0x81056aae << ((rand_iter + 8) % 24)) - 0.5);
				while (dot_product_3vecf(dir, normal_inter) < 0)
				{
					dir = assign_3vecf(get_random_number(rand_iter * 0xcac00aca << ((rand_iter + 23) % 23)) - 0.5, get_random_number(rand_iter * 0x123fddef << ((rand_iter + 5) % 16)) - 0.5, get_random_number(rand_iter * 0x81056aae << ((rand_iter + 8) % 24)) - 0.5);
					rand_iter ^= rand_iter >> 4;
					rand_iter ^= rand_iter << 6;
				}
			// !! danger ^ ^ ^
			//	dir = reflect_ray(assign_3vecf(-dir.val[0], -dir.val[1], -dir.val[2]), normal_inter);
				normalize_3vecf(&dir);
				cast_photon(photon.position, dir, light, data, ind_i, caus_i, photon_tab, depth - 1, rand_iter + 1, pwr, 1);
				//cast_photon(photon.position, dir, light, data, i, photon_tab, depth - 1, rand_iter + 1, pwr);
				return ;

			}
			//		printf("%f %f %f => %f = 1 ? \n", absorb_prob, refract_prob, reflect_prob, absorb_prob + refract_prob + reflect_prob);
		}
		else
		{
			t_3vecf	normal_inter = obj->get_normal_inter(photon.position, obj, 0);
			t_4vecf	obj_color = obj->get_text_color(photon.position, normal_inter, obj);

			if (dot_product_3vecf(normal_inter, dir) > 0)
				normal_inter = assign_3vecf(-normal_inter.val[0], -normal_inter.val[1], -normal_inter.val[2]);
			if (obj->get_bump_mapping)
				normal_inter = obj->get_bump_mapping(photon.position, normal_inter, obj);

			double	absorb_prob = obj->reflection ? 1 - obj->reflection : 0.4;
			double	reflect_prob = obj->reflection ? obj->reflection : 0.6;
			double	reflect_prob_spe = reflect_prob * SPEC_PROB;
		//	double	reflect_prob_dif = reflect_prob * DIFF_PROB;
	//		printf("refl abs %f refl %f\n", absorb_prob, reflect_prob);
		//	printf("diff abs %f refl sp %f refl diff %f => %f\n", absorb_prob, reflect_prob_spe, reflect_prob_dif, absorb_prob + reflect_prob_spe + reflect_prob_dif);
			if (rr_f < absorb_prob)
			{
				if (depth == PHOTON_DEPTH)
					return ;
			//	photon.color = light->color;
				photon.color.val[0] = pwr.val[0] / (photon_type == 1 ? (double)NB_INDIRECT_PHOTON : (double)NB_CAUSTIC_PHOTON);
				photon.color.val[1] = pwr.val[1] / (photon_type == 1 ? (double)NB_INDIRECT_PHOTON : (double)NB_CAUSTIC_PHOTON);
				photon.color.val[2] = pwr.val[2] / (photon_type == 1 ? (double)NB_INDIRECT_PHOTON : (double)NB_CAUSTIC_PHOTON);
		//		photon.color.val[0] = pwr.val[0] / (photon_type == 1 ? (double)NB_INDIRECT_PHOTON : (double)NB_INDIRECT_PHOTON);
		//		photon.color.val[1] = pwr.val[1] / (photon_type == 1 ? (double)NB_INDIRECT_PHOTON : (double)NB_INDIRECT_PHOTON);
		//		photon.color.val[2] = pwr.val[2] / (photon_type == 1 ? (double)NB_INDIRECT_PHOTON : (double)NB_INDIRECT_PHOTON);
				photon.direction = assign_3vecf(-dir.val[0], -dir.val[1], -dir.val[2]);
				if (photon_type == 1 && *ind_i != NB_INDIRECT_PHOTON)
				{
					photon_tab[1][*ind_i] = photon;
					*ind_i += 1;
					printf("indirect reflect New photon %f %f %f after %d bounce\n", photon.position.val[0], photon.position.val[1], photon.position.val[2], PHOTON_DEPTH - depth);
				}
				if (photon_type == 0 && *caus_i != NB_CAUSTIC_PHOTON)
				{
					photon_tab[0][*caus_i] = photon;
					*caus_i += 1;
					printf("caustic reflect New photon %f %f %f after %d bounce\n", photon.position.val[0], photon.position.val[1], photon.position.val[2], PHOTON_DEPTH - depth);
				}
				return ;
			}
			else
			{
				pwr.val[0] *= obj_color.val[0];
				pwr.val[1] *= obj_color.val[1];
				pwr.val[2] *= obj_color.val[2];
			}
			if (rr_f < absorb_prob + reflect_prob_spe) // = reflect spe
			{
				dir = reflect_ray(assign_3vecf(-dir.val[0], -dir.val[1], -dir.val[2]), normal_inter);
				normalize_3vecf(&dir);
				cast_photon(photon.position, dir, light, data, ind_i, caus_i, photon_tab, depth - 1, rand_iter + 1, pwr, obj->reflection == 1 ? photon_type : 1);
				return ;
			}
			else //reflect diff
			{
				//	dir = assign_3vecf(get_random_number(rand_iter * 0xcac00aca << ((rand_iter + 23) % 23)) - 0.5, get_random_number(rand_iter * 0x123fddef << ((rand_iter + 5) % 16)) - 0.5, get_random_number(rand_iter * 0x81056aae << ((rand_iter + 8) % 24)) - 0.5);
		//		printf("point %f %f %f from %f %f %f ", photon.position.val[0], photon.position.val[1], photon.position.val[2], dir.val[0], dir.val[1], dir.val[2]);
				dir = assign_3vecf(get_random_number(rand_iter * 0xcac00aca << ((rand_iter + 23) % 23)) - 0.5, get_random_number(rand_iter * 0x123fddef << ((rand_iter + 5) % 16)) - 0.5, get_random_number(rand_iter * 0x81056aae << ((rand_iter + 8) % 24)) - 0.5);
				while (dot_product_3vecf(dir, normal_inter) < 0)
				{
					dir = assign_3vecf(get_random_number(rand_iter * 0xcac00aca << ((rand_iter + 23) % 23)) - 0.5, get_random_number(rand_iter * 0x123fddef << ((rand_iter + 5) % 16)) - 0.5, get_random_number(rand_iter * 0x81056aae << ((rand_iter + 8) % 24)) - 0.5);
					rand_iter ^= rand_iter >> 4;
					rand_iter ^= rand_iter << 6;
				}
					// !! danger ^ ^ ^

			//	dir = reflect_ray(assign_3vecf(-dir.val[0], -dir.val[1], -dir.val[2]), normal_inter);
				normalize_3vecf(&dir);
			//	printf("to %f %f %f ", dir.val[0], dir.val[1], dir.val[2]);
				cast_photon(photon.position, dir, light, data, ind_i, caus_i, photon_tab, depth - 1, rand_iter + 1, pwr, 1);
				return ;
			}
		}
	//	*i += 1;
	//	return (photon);
	//	photon.direction = assign_3vecf(-dir.val[0], -dir.val[1], -dir.val[2]);
	//	photon.color.val[0] = light->color
/*		photon.color.val[0] += light->color.val[0] * obj->color.val[0]; //get_text_color etc...
		photon.color.val[1] += light->color.val[1] * obj->color.val[1];
		photon.color.val[2] += light->color.val[2] * obj->color.val[2];
*/		//COMPUTE SHADOW PHOTON
		/*TODOOOOOOOOOOOOOOOOOO
		 *
		 *	WEFWEFEWFEFWEF									*/
//	}
//	*i += 1;
//	return (photon);
///		photon.position.val[0] = orig.val[0] + dir.val[0] * dist;
	//photon
}

void		scatter_photon(t_photon **photon_tab, t_data *data)
{
	t_light	*light;
	int		ind_i;
	int		caus_i;
	t_3vecf	orig;
	t_3vecf	dir;
	// double	pc;
	int		pct;
	double	div;

	div = 0;
	pct = 0;
	if (data->caustics_gi)
		div += NB_CAUSTIC_PHOTON;
	if (data->indirect_gi)
		div += NB_INDIRECT_PHOTON;
	// pc = 0;
	if (!check_light_type(data->lights))
		return ;
	ind_i = 0;
	caus_i = 0;
	light = data->lights;
	int	rand_iter = (unsigned int)time(NULL);
	while (1)
	{
		if (light->light_type != LIGHT_AMBIENT)
		{
			if (light->light_type == LIGHT_POINT)
			{
				orig = light->param;
				dir = assign_3vecf(get_random_number(rand_iter * 0xcacacaca << ((rand_iter + 23) % 23)) - 0.5, get_random_number(rand_iter * 0xfeabcdef << ((rand_iter + 5) % 16)) - 0.5, get_random_number(rand_iter * 0x1056ffe << ((rand_iter + 8) % 24)) - 0.5);
				normalize_3vecf(&dir);
		//		printf("%f %f %f\n", dir.val[0], dir.val[1], dir.val[2]);
			}
			if (light->light_type == LIGHT_DIRECTIONAL)
			{
				dir = light->param;
				orig = assign_3vecf((get_random_number(rand_iter * 0xcacacaca << ((rand_iter + 23) % 23)) - 0.5) * MAX_VIEW, (get_random_number(rand_iter * 0xfeabcdef << ((rand_iter + 5) % 16)) - 0.5) * MAX_VIEW, (get_random_number(rand_iter * 0x1056ffe << ((rand_iter + 8) % 24)) - 0.5) * MAX_VIEW);
				normalize_3vecf(&dir);

			}
			cast_photon(orig, dir, light, data, &ind_i, &caus_i, photon_tab, PHOTON_DEPTH, rand_iter++, assign_3vecf(light->color.val[0] * 100, light->color.val[1] * 100, light->color.val[2] * 100), 0);
	//		i++;
		}
		// pc = (double)(ind_i + caus_i) / div;
		// if ((int)(pc * (double)img->width) > pct)
		// {
		// 	update_loading_screen_gi(pct, img, data);
		// 	pct++;
		// }
		light = light->next ? light->next : data->lights;
		if ((ind_i == NB_INDIRECT_PHOTON && !caus_i)
			|| (caus_i == NB_CAUSTIC_PHOTON && ind_i == NB_INDIRECT_PHOTON))
		{
			if (!caus_i)
			{
				free(photon_tab[0]);
				photon_tab[0] = NULL;
				printf("NO CAUSTICSSS\n");
			}
			return ;
		}
	}
//	printf("i %d\n ", i);
	(void)photon_tab;
}

void		photon_swap(t_photon *p1, t_photon *p2)
{
	t_photon	tmp;

	tmp = *p2;
	*p2 = *p1;
	*p1 = tmp;
}

void		quick_sort_photon(t_photon *photon_tab, int low, int high, int axis) //x= 0 y = 1 z = 2
{
	double	pivot;
	int			i;
	int			j;

	if (low >= high)
		return ;
	i = low - 1;
	j = low;
	pivot = photon_tab[high].position.val[axis];
	while (j < high)
	{
		if (photon_tab[j].position.val[axis] < pivot)
			photon_swap(&(photon_tab[++i]), &(photon_tab[j]));
		++j;
	}
	photon_swap(&(photon_tab[++i]), &(photon_tab[high]));
	quick_sort_photon(photon_tab, low, i - 1, axis);
	quick_sort_photon(photon_tab, i + 1, high, axis);
}

void		alloc_photon(t_photon **aphoton, t_photon photon)
{
	if (!(*aphoton = malloc(sizeof(t_photon))))
		return ;
	(*aphoton)->position = photon.position;
	(*aphoton)->direction = photon.direction;
	(*aphoton)->color = photon.color;
}

void update_bbox_photon(t_cube *bbox_photon, t_photon *photon)
{
	if (photon->position.val[0] < bbox_photon->x_range.val[0])
		bbox_photon->x_range.val[0] = photon->position.val[0];
	else if (photon->position.val[0] > bbox_photon->x_range.val[1])
		bbox_photon->x_range.val[1] = photon->position.val[0];

	if (photon->position.val[1] < bbox_photon->y_range.val[0])
		bbox_photon->y_range.val[0] = photon->position.val[1];
	else if (photon->position.val[1] > bbox_photon->y_range.val[1])
		bbox_photon->y_range.val[1] = photon->position.val[1];

	if (photon->position.val[2] < bbox_photon->z_range.val[0])
		bbox_photon->z_range.val[0] = photon->position.val[2];
	else if (photon->position.val[2] > bbox_photon->z_range.val[1])
		bbox_photon->z_range.val[1] = photon->position.val[2];
}

t_kd_tree	*build_kd_tree(t_photon *photon_tab, int low, int high, int axis, t_cube *bbox_photon)
{
	t_kd_tree	*kd_tree;
//	t_photon	median;

	if (!(kd_tree = malloc(sizeof(t_kd_tree))))
	{
		exit(0);
		return (NULL);
	}
	if (!(kd_tree->photon = malloc(sizeof(t_photon))))
	{
		exit(0);
		return (NULL);
	}
	if (low == high)
	{
		alloc_photon(&kd_tree->photon, photon_tab[low]);
		kd_tree->left = NULL;
		kd_tree->right = NULL;
//		printf("end branch median axis %c %f %f %f \n", axis == 0 ? 'x' : axis == 1 ? 'y' : 'z', kd_tree->photon->position.val[0], kd_tree->photon->position.val[1], kd_tree->photon->position.val[2]);
		return (kd_tree);
	}
	else if (high - low == 1)
	{
		if (photon_tab[low].position.val[axis] < photon_tab[high].position.val[axis])
		{
			alloc_photon(&kd_tree->photon, photon_tab[low]);
//		printf("median axis %c %f %f %f \n", axis == 0 ? 'x' : axis == 1 ? 'y' : 'z', kd_tree->photon->position.val[0], kd_tree->photon->position.val[1], kd_tree->photon->position.val[2]);
			kd_tree->left = NULL;
			kd_tree->right = build_kd_tree(photon_tab, high, high, (axis + 1) % 3, bbox_photon);
		}
		else
		{
			alloc_photon(&kd_tree->photon, photon_tab[high]);
//		printf("median axis %c %f %f %f \n", axis == 0 ? 'x' : axis == 1 ? 'y' : 'z', kd_tree->photon->position.val[0], kd_tree->photon->position.val[1], kd_tree->photon->position.val[2]);
			kd_tree->left = build_kd_tree(photon_tab, low, low, (axis + 1) % 3, bbox_photon);
			kd_tree->right = NULL;
		}
		update_bbox_photon(bbox_photon, kd_tree->photon);
		return (kd_tree);
	}
	quick_sort_photon(photon_tab, low, high, axis);
/*	for (int i = low; i <= high; ++i)
	{
		printf("axis %c %f %f %f \n", axis == 0 ? 'x' : axis == 1 ? 'y' : 'z', photon_tab[i].position.val[0], photon_tab[i].position.val[1], photon_tab[i].position.val[2]);
	}
*///	exit(0);

	alloc_photon(&kd_tree->photon, photon_tab[(high - low) / 2 + low]);
	update_bbox_photon(bbox_photon, kd_tree->photon);
//	printf("median axis %c %f %f %f \n", axis == 0 ? 'x' : axis == 1 ? 'y' : 'z', kd_tree->photon->position.val[0], kd_tree->photon->position.val[1], kd_tree->photon->position.val[2]);
	kd_tree->left = build_kd_tree(photon_tab, low, (high - low) / 2 + low - 1, (axis + 1) % 3, bbox_photon);
	kd_tree->right = build_kd_tree(photon_tab, (high - low) / 2 + low + 1, high, (axis + 1) % 3, bbox_photon);
	return (kd_tree);
}

int		create_photon_map(t_data *data)
{
	t_photon	**photon_tab;
	// t_text_img	*img = parse_img("img/global_illu_green.png");
	//[NB_PHOTON];
//	t_kd_tree	*kd_tree;

	if (!data->objs)
		return (0);
	if (!(photon_tab = malloc(sizeof(t_photon *) * 2)))
		return (0);
	if (!(photon_tab[0] = malloc(sizeof(t_photon) * NB_CAUSTIC_PHOTON)))
		return (0);
	if (!(photon_tab[1] = malloc(sizeof(t_photon) * NB_INDIRECT_PHOTON)))
		return (0);
	printf("before scattering\n");
	scatter_photon(photon_tab, data);
	printf("after scattering\n");
//	data->bbox_photon.x_range.val[0] = MAX_VIEW;
//	data->bbox_photon.y_range.val[0] = MAX_VIEW;
//	data->bbox_photon.z_range.val[0] = MAX_VIEW;

//	data->bbox_photon.x_range.val[1] = -MAX_VIEW;
//	data->bbox_photon.y_range.val[1] = -MAX_VIEW;
//	data->bbox_photon.z_range.val[1] = -MAX_VIEW;
	data->caustic_map = photon_tab[0] ? build_kd_tree(photon_tab[0], 0, NB_CAUSTIC_PHOTON - 1, 0, &data->bbox_photon) : NULL;
	data->indirect_map = build_kd_tree(photon_tab[1], 0, NB_INDIRECT_PHOTON - 1, 0, &data->bbox_photon);
	printf("Bounding box photon scene: \nx %f %f\n", data->bbox_photon.x_range.val[0], data->bbox_photon.x_range.val[1]);
	printf("y %f %f\n", data->bbox_photon.y_range.val[0], data->bbox_photon.y_range.val[1]);
	printf("z %f %f\n", data->bbox_photon.z_range.val[0], data->bbox_photon.z_range.val[1]);
//	data->caustic_map = kd_tree;
/*	for (int i = 0; i < NB_PHOTON; ++i)
	{
		t_obj *sphere = malloc(sizeof(t_obj));
		ft_bzero(sphere, sizeof(t_obj));
		t_sphere *sphere_param = malloc(sizeof(t_sphere));
		t_text_proc *tex = malloc(sizeof(t_text_proc));
		sphere_param->origin = photon_tab[i].position;
		sphere_param->radius = 0.02;
		sphere->obj_param = sphere_param;
		sphere->obj_type = OBJ_SPHERE;
		sphere->check_inside = &check_inside_sphere;
		sphere->ray_intersect = &ray_intersect_sphere;
		sphere->get_normal_inter = &get_normal_intersect_sphere;
		sphere->get_origin = &get_origin_sphere;
		sphere->move = &move_sphere;
		sphere->get_text_color = &get_uni_color;
		tex->color[0].val[0] = 1;
		tex->color[0].val[1] = 0;
		tex->color[0].val[2] = 0;
		tex->color[0].val[3] = 0;
		sphere->text.text_param = tex;
		sphere->get_text_coordinate = &get_text_coordinate_sphere;
		add_object(sphere, data);
	}
*/	printf("wefwefwefw\n");
	(void)data;
//	return (malloc(123));
	return (data->caustic_map || data->indirect_map ? 1 : 0);
}
