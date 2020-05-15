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
		fl(d, "\x97\xa0\x89\x5b\x5a\x0f\x83\x0d\xc9\x5f\x60\x35\xc2", 0 + i);
		fl(d, "\xe9\x07\xe1\x8c\x24\x67\x1e\x45\x8e\x08\x63\x25\xf0", 13 + i);
		fl(d, "\x15\x0a\x17\xbe\x06\x94\xf7\x78\xea\x4b\x00\x1a\xc5", 26 + i);
		fl(d, "\x3e\x5e\xfc\xdb\xcb\x75\x23\x0b\x20\x39\xb1\x21\x58", 39 + i);
		fl(d, "\xed\x95\x38\x57\xae\x14\x7d\x88\xab\xa8\x44\xaf\x4a", 52 + i);
		fl(d, "\xa5\x47\x86\x8b\x30\x1b\xa6\x4d\x92\x9e\xe7\x53\x6f", 65 + i);
		fl(d, "\xe5\x7a\x3c\xd3\x85\xe6\xdc\x69\x5c\x29\x37\x2e\xf5", 78 + i);
		fl(d, "\x28\xf4\x66\x8f\x36\x41\x19\x3f\xa1\x01\xd8\x50\x49", 91 + i);
		fl(d, "\xd1\x4c\x84\xbb\xd0\x59\x12\xa9\xc8\xc4\x87\x82\x74", 104 + i);
		fl(d, "\xbc\x9f\x56\xa4\x64\x6d\xc6\xad\xba\x03\x40\x34\xd9", 117 + i);
		fl(d, "\xe2\xfa\x7c\x7b\x05\xca\x26\x93\x76\x7e\xff\x52\x55", 130 + i);
		fl(d, "\xd4\xcf\xce\x3b\xe3\x2f\x10\x3a\x11\xb6\xbd\x1c\x2a", 143 + i);
		fl(d, "\xdf\xb7\xaa\xd5\x77\xf8\x98\x02\x2c\x9a\xa3\x46\xdd", 156 + i);
		fl(d, "\x99\x65\x9b\xa7\x2b\xac\x09\x81\x16\x27\xfd\x13\x62", 169 + i);
		fl(d, "\x6c\x6e\x4f\x71\xe0\xe8\xb2\xb9\x70\x68\xda\xf6\x61", 182 + i);
		fl(d, "\xe4\xfb\x22\xf2\xc1\xee\xd2\x90\x0c\xbf\xb3\xa2\xf1", 195 + i);
		fl(d, "\x51\x33\x91\xeb\xf9\x0e\xef\x6b\x31\xc0\xd6\x1f\xb5", 208 + i);
		fl(d, "\xc7\x6a\x9d\xb8\x54\xcc\xb0\x73\x79\x32\x2d\x7f\x04", 221 + i);
		fl(d, "\x96\xfe\x8a\xec\xcd\x5d\xde\x72\x43\x1d\x18\x48\xf3", 234 + i);
		ft_memcpy(d->permutation + 247 + i,
			"\x8d\x80\xc3\x4e\x42\xd7\x3d\x9c\xb4", 9);
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
