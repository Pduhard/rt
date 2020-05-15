/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_perlin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 19:03:51 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 19:06:52 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	fl(t_data *data, char *src, int offset)
{
	ft_memcpy(data->permutation + offset, src, 14);
}

static void	init_permutation(t_data *d, int i)
{
	while (i < 512)
	{
		fl(d, "\x97\xa0\x89\x5b\x5a\x0f\x83\x0d\xc9\x5f\x60\x35\xc2\xe9", 0 + i);
		fl(d, "\x07\xe1\x8c\x24\x67\x1e\x45\x8e\x08\x63\x25\xf0\x15\x0a", 14 + i);
		fl(d, "\x17\xbe\x06\x94\xf7\x78\xea\x4b\x00\x1a\xc5\x3e\x5e\xfc", 28 + i);
		fl(d, "\xdb\xcb\x75\x23\x0b\x20\x39\xb1\x21\x58\xed\x95\x38\x57", 42 + i);
		fl(d, "\xae\x14\x7d\x88\xab\xa8\x44\xaf\x4a\xa5\x47\x86\x8b\x30", 56 + i);
		fl(d, "\x1b\xa6\x4d\x92\x9e\xe7\x53\x6f\xe5\x7a\x3c\xd3\x85\xe6", 70 + i);
		fl(d, "\xdc\x69\x5c\x29\x37\x2e\xf5\x28\xf4\x66\x8f\x36\x41\x19", 84 + i);
		fl(d, "\x3f\xa1\x01\xd8\x50\x49\xd1\x4c\x84\xbb\xd0\x59\x12\xa9", 98 + i);
		fl(d, "\xc8\xc4\x87\x82\x74\xbc\x9f\x56\xa4\x64\x6d\xc6\xad\xba", 112 + i);
		fl(d, "\x03\x40\x34\xd9\xe2\xfa\x7c\x7b\x05\xca\x26\x93\x76\x7e", 126 + i);
		fl(d, "\xff\x52\x55\xd4\xcf\xce\x3b\xe3\x2f\x10\x3a\x11\xb6\xbd", 140 + i);
		fl(d, "\x1c\x2a\xdf\xb7\xaa\xd5\x77\xf8\x98\x02\x2c\x9a\xa3\x46", 154 + i);
		fl(d, "\xdd\x99\x65\x9b\xa7\x2b\xac\x09\x81\x16\x27\xfd\x13\x62", 168 + i);
		fl(d, "\x6c\x6e\x4f\x71\xe0\xe8\xb2\xb9\x70\x68\xda\xf6\x61\xe4", 182 + i);
		fl(d, "\xfb\x22\xf2\xc1\xee\xd2\x90\x0c\xbf\xb3\xa2\xf1\x51\x33", 196 + i);
		fl(d, "\x91\xeb\xf9\x0e\xef\x6b\x31\xc0\xd6\x1f\xb5\xc7\x6a\x9d", 210 + i);
		fl(d, "\xb8\x54\xcc\xb0\x73\x79\x32\x2d\x7f\x04\x96\xfe\x8a\xec", 224 + i);
		fl(d, "\xcd\x5d\xde\x72\x43\x1d\x18\x48\xf3\x8d\x80\xc3\x4e\x42", 238 + i);
		ft_memcpy(d->permutation + 252 + i, "\xd7\x3d\x9c\xb4", 4);
		i += 256;
	}
}

void		init_perlin(t_data *data)
{
	init_permutation(data, 0);
	data->gradient[0] = assign_3vecf(1, 1, 0);
	data->gradient[1] = assign_3vecf(-1, 1, 0);
	data->gradient[2] = assign_3vecf(1, -1, 0);
	data->gradient[3] = assign_3vecf(-1, -1, 0);
	data->gradient[4] = assign_3vecf(1, 0, 1);
	data->gradient[5] = assign_3vecf(-1, 0, 1);
	data->gradient[6] = assign_3vecf(1, 0, -1);
	data->gradient[7] = assign_3vecf(-1, 0, -1);
	data->gradient[8] = assign_3vecf(0, 1, 1);
	data->gradient[9] = assign_3vecf(0, -1, 1);
	data->gradient[10] = assign_3vecf(0, 1, -1);
	data->gradient[11] = assign_3vecf(0, -1, -1);
	data->gradient[12] = assign_3vecf(1, 1, 0);
	data->gradient[13] = assign_3vecf(-1, 1, 0);
	data->gradient[14] = assign_3vecf(0, -1, 1);
	data->gradient[15] = assign_3vecf(0, -1, -1);
}
