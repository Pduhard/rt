#include "rt.h"

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
