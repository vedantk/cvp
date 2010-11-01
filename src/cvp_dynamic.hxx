/*
 * specialized region-growing for noise reduction
 *  
 * Copyright (c) 2009 Vedant Kumar <vminch@gmail.com>
 */

#ifndef CVP_DYNAMIC_HXX
#define CVP_DYNAMIC_HXX

#include "image.hpp"
#include <map>

const ushort edge = 175;

static bool locate_seed(img* mra_volume, vector<ushort>& rar, vector<ushort>& car, vector<ushort>& zar) {
	for (ushort q2=edge; q2 < 512-edge; ++q2) {
		for (ushort q3=edge; q3 < 512-edge; ++q3) {
			if (mra_volume[num_imgs-1].pix[q2][q3] == white) {
				rar.clear();
				car.clear();
				zar.clear();
				
				rar.push_back(q2);
				car.push_back(q3);
				zar.push_back(num_imgs-1);
				
				return true;
			}
		}
	}
	
	return false;
}

void cvp_region_growing(img* mra_volume) {
	dp("cvp region-growing...");
	int i, j, z, k3, k2, k1;
	uint arc;
	uint visiting_pts;
	
	vector<ushort> rar, car, zar;
	
	map<uint, uint> regions; // flag : fcount
	uint flag = 0;
	
	while (locate_seed(mra_volume, rar, car, zar)) {
		arc = 1;
		visiting_pts = 0;
		flag++;

		while (arc != visiting_pts)	{
			i = rar[visiting_pts];
			j = car[visiting_pts];
			z = zar[visiting_pts];

			for (k3 = z-1; k3 <= z+1; k3++) {
				if (k3 < 0 || k3 >= num_imgs) continue;
				
				for (k1 = i-1; k1 <= i+1; k1++) {
					if (k1 < 0 || k1 > 511) continue;
					
					for (k2 = j-1; k2 <= j+1; k2++) {
						if (k2 < 0 || k2 > 511) continue;
						
						if (mra_volume[k3].pix[k1][k2] == white) {
							rar.push_back(k1);
							car.push_back(k2);
							zar.push_back(k3);
							arc++;
							mra_volume[k3].pix[k1][k2] = flag;
						}
					}
				}
			}

			visiting_pts += 1;
		}
		
		regions[flag] = arc;
	}
	
	dp("stripping voxels...");
	uint wcount = regions[ white ];
	for (ushort q=0; q < num_imgs; ++q) {
		for (ushort q2=0; q2 < 512; ++q2) {
			for (ushort q3=0; q3 < 512; ++q3) {
				if (regions[ mra_volume[q].pix[q2][q3] ] < 100) {
					mra_volume[q].pix[q2][q3] = 0;
				} else {
					mra_volume[q].pix[q2][q3] = white;
				} 
			}
		}
	}
}

#endif	
