/*
 * Copyright (c) 2009 Vedant Kumar <vminch@gmail.com>
 */

#include "image.hpp"
#include "cvp_dynamic.hxx"
#include "cvp_statistical.hxx"

#include <limits>

const char* OUT_DIR	= "out/";
void arg_help() {
	dp("cvp [images: #] [[type: 'bmp' | 'pgm']] [[fprefix: str]]");
}

img read_img(const char* fp) {
	FILE* fptr = fopen(fp, "rb");
	if (fptr == NULL) {
		img_err(fp, "File does not exist.");
	}

	return imr(fptr);
}

bool write_img(img* dat, const char* fp) {
	FILE* fptr = fopen(fp, "wb");
	if (fptr == NULL) {
		img_err(fp, "Cannot write to file.");
	}

	return imw(dat, fptr);
}

int main(int argc, char** argv) {
	NUM_EPSILON = numeric_limits<num>::epsilon();
		
	const char* img_type;
	const char* fprefix;
	
	if (argc == 1) {
		num_imgs = 93;
		img_type = "pgm";
		fprefix = "test/";
	} else if (argc == 4) {
		num_imgs = atoi(argv[1]);
		img_type = argv[2];
		fprefix = argv[3];
	} else {
		arg_help();
		
		for (int i=0; i < argc; ++i) {
			cout << argv[i] << endl;
		}

		return EXIT_SUCCESS;
	}

	if (strcmp(img_type, "pgm") == 0 || strcmp(img_type, "PGM") == 0) {
		imr = pgm_read;
	} else if (strcmp(img_type, "bmp") == 0 || strcmp(img_type, "BMP") == 0) {
		imr = bmp_read;
	} else {
		error("unknown image type; only 'bmp' and 'pgm' currently supported");
	}
	
	imw = pgm_write;
	
	dp("reading all images...");
    img* volume = new img[num_imgs];
    num mra_hist[256];

	char fpath[512];
	for (ushort n=0; n < num_imgs; ++n) {
		sprintf(fpath, "%s%.3u.%s", fprefix, n+1, img_type);
		volume[n] = read_img(fpath);
	}

	dp("smoothing volume...");
	mgrf_apply(volume);

	dp("calculating cumulative histogram...");
	cum_hist(volume, mra_hist);
	
	dp("estimating region densities...");
	ushort cap = cvp_estimate_density(mra_hist);
	cout << "cap: " << cap << endl;
	
	dp("purifying stack...");
	segment(volume, cap, RM_UNDER_T, 0);
	segment(volume, cap-1, RM_ABOVE_T, white);

	dp("eliminating noisy regions...");
	cvp_region_growing(volume);
	
	dp("finished! writing images to disk...");
	for (ushort q=0; q < num_imgs; ++q) {
		char fp[512];
		memset(fp, 0, 512);
		sprintf(fp, "%s%.3u.pgm", OUT_DIR, q+1);
		
		if (!write_img(&(volume[q]), fp)) {
			error("could not write image to disk.");
		}
	}

	return 0;
}
