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

t_3vecf get_random_dir(int rand_iter)
{
	t_3vecf dir;

	dir = assign_3vecf(
		get_random_number(rand_iter * 0xcac00aca << (rand_iter % 23)) - 0.5,
		get_random_number(rand_iter * 0x123fddef << (rand_iter % 16)) - 0.5,
		get_random_number(rand_iter * 0x81056aae << (rand_iter % 24)) - 0.5);
	normalize_3vecf(&dir);
	return (dir);
}

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

t_phtn_prob	get_prob(double absorb_prob, double refract_prob, double reflect_prob)
{
	t_phtn_prob	prob;

	prob.absorb_prob = absorb_prob;
	prob.refract_prob = refract_prob;
	prob.reflect_prob_spe = reflect_prob * SPEC_PROB;
	prob.reflect_prob_dif = reflect_prob * DIFF_PROB;
	return (prob);
}

void  absorb_photon(t_leq l, t_phtn_cast p, t_photon photon)
{
	if (p.depth == PHOTON_DEPTH)
		return ;
	// printf("%f %f %f\n", p.pwr.val[0],p.pwr.val[1],p.pwr.val[2]);

	photon.color.val[0] = p.pwr.val[0] / (p.photon_type == 1 ? (double)NB_INDIRECT_PHOTON : (double)NB_CAUSTIC_PHOTON);
	photon.color.val[1] = p.pwr.val[1] / (p.photon_type == 1 ? (double)NB_INDIRECT_PHOTON : (double)NB_CAUSTIC_PHOTON);
	photon.color.val[2] = p.pwr.val[2] / (p.photon_type == 1 ? (double)NB_INDIRECT_PHOTON : (double)NB_CAUSTIC_PHOTON);
	if (photon.color.val[0] != photon.color.val[0] || photon.color.val[1] != photon.color.val[1] || photon.color.val[2] != photon.color.val[2])
		exit(0);
	photon.direction = assign_3vecf(-l.dir.val[0], -l.dir.val[1], -l.dir.val[2]);
	if (p.photon_type == 1 && *(p.ind_i) != NB_INDIRECT_PHOTON)
	{
		p.photon_tab[1][*(p.ind_i)] = photon;
		*(p.ind_i) += 1;
	}
	if (p.photon_type == 0 && *(p.caus_i) != NB_CAUSTIC_PHOTON)
	{
		p.photon_tab[0][*(p.caus_i)] = photon;
		*(p.caus_i) += 1;
	}
	return ;
}

t_3vecf add_color_bleed(t_3vecf pwr, t_4vecf obj_color)
{
	// return (pwr);

		pwr.val[0] *= obj_color.val[0];
		pwr.val[1] *= obj_color.val[1];
		pwr.val[2] *= obj_color.val[2];
		return (pwr);
}

void  refract_photon(t_leq l, t_phtn_cast p)
{
	normalize_3vecf(&l.dir);
	p.depth--;
	p.rand_iter++;
	cast_photon(l, p);
	return ;
}

void  reflect_photon_spec(t_leq l, t_phtn_cast p, int photon_type)
{
	normalize_3vecf(&l.dir);
	p.depth--;
	p.rand_iter++;
	p.photon_type = photon_type;
	cast_photon(l, p);
	return ;
}

void  reflect_photon_diff(t_leq l, t_phtn_cast p,
	t_photon photon, t_3vecf normal_inter)
{
	l.dir = get_random_dir(p.rand_iter);
	while (dot_product_3vecf(l.dir, normal_inter) < 0)
	{
		l.dir = get_random_dir(p.rand_iter);
		p.rand_iter ^= p.rand_iter >> 4;
		p.rand_iter ^= p.rand_iter << 7;
		p.rand_iter ^= p.rand_iter >> 8;
	}
	p.depth--;
	p.rand_iter++;
	p.photon_type = 1;
	cast_photon((t_leq){photon.position, l.dir}, p);
	return ;
}

void  cast_photon_refractive_mat(t_leq l, t_phtn_cast p, t_obj *obj, t_photon photon)
{
	double			rr_f;
	t_3vecf			normal_inter;
	t_4vecf			clr;
	t_phtn_prob	prob;
	double			fr_r;

	rr_f = get_random_number(p.rand_iter * 0xfab1dede << (p.rand_iter & 7));
	normal_inter = obj->get_normal_inter(photon.position, obj, 0);
	clr = obj->get_text_color(photon.position, normal_inter, obj);
	if (dot_product_3vecf(normal_inter, l.dir) > 0)
		normal_inter = neg_3vecf(normal_inter);
	if (obj->get_bump_mapping)
		normal_inter = obj->get_bump_mapping(photon.position, normal_inter, obj);
	fr_r = compute_fresnel_ratio(l.dir, normal_inter, obj->refraction, 0);
	prob = get_prob((1 - fr_r) * (1 - clr.val[3]), (1 - fr_r) * clr.val[3], fr_r);
	if (rr_f < prob.absorb_prob)
		return (absorb_photon(l, p, photon));
	p.pwr = add_color_bleed(p.pwr, clr);
	if (rr_f < prob.refract_prob + prob.absorb_prob)
		return (refract_photon((t_leq){photon.position,
			refract_ray(l.dir, normal_inter, obj->refraction, 0)}, p));
	else if (rr_f < prob.refract_prob + prob.absorb_prob + prob.reflect_prob_spe)
		return (reflect_photon_spec((t_leq){photon.position,
			reflect_ray(neg_3vecf(l.dir), normal_inter)}, p, p.photon_type));
	return (reflect_photon_diff(l, p, photon, normal_inter));
}

void  cast_photon_diffuse_mat(t_leq l, t_phtn_cast p,
	t_obj *obj, t_photon photon)
{
	double rr_f;
	t_3vecf	normal_inter;
	t_4vecf obj_color;
	t_phtn_prob	prob;

	normal_inter = obj->get_normal_inter(photon.position, obj, 0);
	obj_color = obj->get_text_color(photon.position, normal_inter, obj);
	rr_f = get_random_number(p.rand_iter * 0xfab1dede << (p.rand_iter & 7));
	if (dot_product_3vecf(normal_inter, l.dir) > 0)
		normal_inter = neg_3vecf(normal_inter);
	if (obj->get_bump_mapping)
		normal_inter = obj->get_bump_mapping(photon.position, normal_inter, obj);
	prob = get_prob(obj->reflection ? 1 - obj->reflection : 0.4,
		0., obj->reflection ? obj->reflection : 0.6);
	if (rr_f < prob.absorb_prob)
		return (absorb_photon(l, p, photon));
	p.pwr = add_color_bleed(p.pwr, obj_color);
	if (rr_f < prob.absorb_prob + prob.reflect_prob_spe) // = reflect spe
		return (reflect_photon_spec(
			(t_leq){photon.position, reflect_ray(neg_3vecf(l.dir), normal_inter)},
			p, obj->reflection == 1 ? p.photon_type : 1));
	return (reflect_photon_diff(l, p, photon, normal_inter));
}

void  cast_photon(t_leq l, t_phtn_cast p)
{
	t_photon	photon;
	t_obj		*obj;
	double		dist;
	int			inside;

	inside = 0;
	if (!p.depth || is_null_3vecf(p.pwr))
		return ;
	obj = ray_first_intersect(l, (t_dist){&dist, BIAS, MAX_VIEW},
		p.data->objs, 0);
	if (!obj)
		return ;
	photon.position = add_3vecf(l.orig, product_c3vecf(l.dir, dist));
	if (obj->refraction)
		return (cast_photon_refractive_mat(l, p, obj, photon));
	return (cast_photon_diffuse_mat(l, p, obj, photon));
}

int     check_end_of_scattering(int ind_i, int caus_i, t_photon **photon_tab)
{
	if ((ind_i == NB_INDIRECT_PHOTON && !caus_i)
		|| (caus_i == NB_CAUSTIC_PHOTON && ind_i == NB_INDIRECT_PHOTON))
	{
		if (!caus_i)
		{
			free(photon_tab[0]);
			photon_tab[0] = NULL;
		}
		return (1);
	}
	return (0);
}

t_3vecf get_photon_power(t_light *light)
{
	return (assign_3vecf(
		light->color.val[0] * DFLT_POWER,
		light->color.val[1] * DFLT_POWER,
		light->color.val[2] * DFLT_POWER));
}

void		scatter_photon(t_photon **photon_tab, t_data *data)
{
	t_light	*light;
	int			ind_i;
	int			caus_i;
	t_leq		l;
	int			rand_iter;

	if (!check_light_type(data->lights))
		return ;
	ind_i = 0;
	caus_i = 0;
	light = data->lights;
	rand_iter = (unsigned int)time(NULL);
	while (!check_end_of_scattering(ind_i, caus_i, photon_tab))
	{
		if (light->light_type != LIGHT_AMBIENT)
		{
			l = light->light_type == LIGHT_POINT ?
			(t_leq){light->param, get_random_dir(rand_iter)} :
			(t_leq){get_random_dir(rand_iter), light->param};
			cast_photon(l, (t_phtn_cast){data, &ind_i, &caus_i, photon_tab,
				PHOTON_DEPTH, rand_iter++, get_photon_power(light), 0});
		}
		light = light->next ? light->next : data->lights;
	}
}

void		photon_swap(t_photon *p1, t_photon *p2)
{
	t_photon	tmp;

	tmp = *p2;
	*p2 = *p1;
	*p1 = tmp;
}

void		quick_sort_photon(t_photon *photon_tab, int lw, int hi, int ax)
{
	double	pivot;
	int			i;
	int			j;

	if (lw >= hi)
		return ;
	i = lw - 1;
	j = lw;
	pivot = photon_tab[hi].position.val[ax];
	while (j < hi)
	{
		if (photon_tab[j].position.val[ax] < pivot)
			photon_swap(&(photon_tab[++i]), &(photon_tab[j]));
		++j;
	}
	photon_swap(&(photon_tab[++i]), &(photon_tab[hi]));
	quick_sort_photon(photon_tab, lw, i - 1, ax);
	quick_sort_photon(photon_tab, i + 1, hi, ax);
}

t_kd_tree		*alloc_photon(t_kd_tree *kd_tree, t_photon photon)
{
	kd_tree->photon->position = photon.position;
	kd_tree->photon->direction = photon.direction;
	kd_tree->photon->color = photon.color;
	return (kd_tree);
}

t_kd_tree	*build_tree(t_photon *phtn_tab, int lw, int hi, int ax)
{
	t_kd_tree	*tree;

	if (!(tree = ft_memalloc(sizeof(t_kd_tree))) ||
			!(tree->photon = ft_memalloc(sizeof(t_photon))))
		return (NULL);
	if (lw == hi)
		return (alloc_photon(tree, phtn_tab[lw]));
	else if (hi - lw == 1)
	{
		if (phtn_tab[lw].position.val[ax] < phtn_tab[hi].position.val[ax])
		{
			tree->right = build_tree(phtn_tab, hi, hi, (ax + 1) % 3);
			return (alloc_photon(tree, phtn_tab[lw]));
		}
		else
		{
			tree->left = build_tree(phtn_tab, lw, lw, (ax + 1) % 3);
			return (alloc_photon(tree, phtn_tab[hi]));
		}
	}
	quick_sort_photon(phtn_tab, lw, hi, ax);
	tree->left = build_tree(phtn_tab, lw, (hi - lw) / 2 + lw - 1, (ax + 1) % 3);
	tree->right = build_tree(phtn_tab, (hi - lw) / 2 + lw + 1, hi, (ax + 1) % 3);
	return (alloc_photon(tree, phtn_tab[(hi - lw) / 2 + lw]));
}

int		create_photon_map(t_data *data)
{
	t_photon	**photon_tab;

	if (!data->objs)
		return (0);
	if (!(photon_tab = malloc(sizeof(t_photon *) * 2)) ||
			!(photon_tab[0] = malloc(sizeof(t_photon) * NB_CAUSTIC_PHOTON)) ||
			!(photon_tab[1] = malloc(sizeof(t_photon) * NB_INDIRECT_PHOTON)))
		return (0);
	scatter_photon(photon_tab, data);
	if (photon_tab[0])
		data->caustic_map = build_tree(photon_tab[0], 0, NB_CAUSTIC_PHOTON - 1, 0);
	data->indirect_map = build_tree(photon_tab[1], 0, NB_INDIRECT_PHOTON - 1, 0);
	free(photon_tab[0]);
	free(photon_tab[1]);
	free(photon_tab);
	return (data->caustic_map || data->indirect_map ? 1 : 0);
}
