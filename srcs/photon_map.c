/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   photon_map.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/02/11 10:49:10 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/11 19:20:05 by pduhard-    ###    #+. /#+    ###.fr     */
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

t_photon	cast_photon(t_3vecf orig, t_3vecf dir, t_light *light, t_data *data, int *i)
{
	t_photon	photon;
	t_obj		*obj;
	double		dist;
	int			depth;

	depth = 0;
	/* TO DELETE*/
	*i += 1;
	/*				*/
	while (depth++ < PHOTON_DEPTH)
	{
		if (!(obj = ray_first_intersect(orig, dir, BIAS, MAX_VIEW, &dist, data->objs, 0, data)))
			return (photon);
		photon.position.val[0] = orig.val[0] + dir.val[0] * dist;
		photon.position.val[1] = orig.val[1] + dir.val[1] * dist;
		photon.position.val[2] = orig.val[2] + dir.val[2] * dist;
		photon.direction = dir;
/*		photon.color.val[0] += light->color.val[0] * obj->color.val[0]; //get_text_color etc...
		photon.color.val[1] += light->color.val[1] * obj->color.val[1];
		photon.color.val[2] += light->color.val[2] * obj->color.val[2];
*/		//COMPUTE SHADOW PHOTON
		/*TODOOOOOOOOOOOOOOOOOO
		 *
		 *	WEFWEFEWFEFWEF									*/
	}
	*i += 1;
	return (photon);
///		photon.position.val[0] = orig.val[0] + dir.val[0] * dist;
	//photon
	(void)light;
}

void		scatter_photon(t_photon photon_tab[NB_PHOTON], t_data *data)
{
	t_light	*light;
	int		i;
	t_3vecf	orig;
	t_3vecf	dir;

	if (!check_light_type(data->lights))
		return ;
	i = 0;
	light = data->lights;
	while (i < NB_PHOTON)
	{
		if (light->light_type != LIGHT_AMBIENT)
		{
			if (light->light_type == LIGHT_POINT)
			{
				orig = light->param;
				dir = assign_3vecf(get_random_number(i * 0xcacacaca << ((i + 23) % 23)) - 0.5, get_random_number(i * 0xfeabcdef << ((i + 5) % 16)) - 0.5, get_random_number(i * 0x1056ffe << ((i + 8) % 24)) - 0.5);
				normalize_3vecf(&dir);
		//		printf("%f %f %f\n", dir.val[0], dir.val[1], dir.val[2]);
			}
			photon_tab[i] = cast_photon(orig, dir, light, data, &i);
	//		i++;
		}
		light = light->next ? light->next : data->lights;
	}
	(void)photon_tab;
}

t_kd_tree	*create_photon_map(t_data *data)
{
	t_photon	photon_tab[NB_PHOTON];

	scatter_photon(photon_tab, data);
	(void)data;
	return (malloc(sizeof(t_kd_tree)));
}
