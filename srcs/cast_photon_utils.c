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
