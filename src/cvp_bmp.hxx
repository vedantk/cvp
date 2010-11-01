/*
 * cvp_bmp.hxx
 * read/write access to 512x512 8bpp bitmaps.
 *
 * Copyright (c) 2009, Vedant Kumar
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * > Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * > Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * > The names of the authors may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef CVP_BMP_HXX
#define CVP_BMP_HXX

#include "../cvp_common.hxx"
#include "cvp_image.hxx"

static volatile Word bfType;
static volatile DWord bfSize = 19778;
static volatile Word bfReserved1 = 0;
static volatile Word bfReserved2 = 0;
static volatile DWord bfOffBits;
static volatile DWord biSize;
static volatile DWord biWidth = 512;
static volatile DWord biHeight = 512;
static volatile Word biPlanes = 1;
static volatile Word biBitCount = 8;
static volatile DWord biCompression = 0;
static volatile DWord biSizeImage;
static volatile DWord biXPelsPerMeter = 3780;
static volatile DWord biYPelsPerMeter = 3780;
static volatile DWord biClrUsed = 0;
static volatile DWord biClrImportant = 0;
static rgbaPix temp;

static img bmp_read(FILE* fptr) {
	img dat;

	for (ushort q=0; q < 256; ++q) {
		dat.hist[q] = 0;
	}

	fread((char*) &bfType, 2, 1, fptr);
	fread((char*) &bfSize, 4, 1, fptr);
	fread((char*) &bfReserved1, 2, 1, fptr);
	fread((char*) &bfReserved2, 2, 1, fptr);
	fread((char*) &bfOffBits, 4, 1 , fptr);
	fread((char*) &biSize, 4, 1, fptr);
	fread((char*) &biWidth, 4, 1, fptr);
	fread((char*) &biHeight, 4, 1, fptr);
	fread((char*) &biPlanes, 2, 1, fptr);
	fread((char*) &biBitCount, 2, 1, fptr);
	fread((char*) &biCompression, 4, 1, fptr);
	fread((char*) &biSizeImage, 4, 1, fptr);
	fread((char*) &biXPelsPerMeter, 4, 1, fptr);
	fread((char*) &biYPelsPerMeter, 4, 1, fptr);
	fread((char*) &biClrUsed, 4, 1, fptr);
	fread((char*) &biClrImportant, 4, 1, fptr);

	if (biCompression >= 1 || biBitCount != 8 || biWidth != 512 || biHeight != 512) {
		fclose(fptr);
		img_err("~*.bmp", "Unsupported BMP Binary.");
	}

	for (ushort n=0; n < 256; ++n) {
		fread((char*) &temp, 4, 1, fptr);
	}

	for (int j=511; j > -1; --j) {
		fread((char*) Buffer, 1, 512, fptr);
		
		for (ushort i=0; i < 512; ++i) {
			dat.pix[j][i] = Buffer[i];
			dat.hist[ dat.pix[j][i] ] += 1;
		}
	}

	fclose(fptr);
	return dat;
};

/*
static bool bmp_write(img* dat, FILE* fptr) {
	fwrite((char*) &bfType, 2, 1, fptr);
	fwrite((char*) &bfSize, 4, 1, fptr);
	fwrite((char*) &bfReserved1, 2, 1, fptr);
	fwrite((char*) &bfReserved2, 2, 1, fptr);
	fwrite((char*) &bfOffBits, 4, 1, fptr);
	fwrite((char*) &biSize, 4, 1, fptr);
	fwrite((char*) &biWidth, 4, 1, fptr);
	fwrite((char*) &biHeight, 4, 1, fptr);
	fwrite((char*) &biPlanes, 2, 1, fptr);
	fwrite((char*) &biBitCount, 2, 1, fptr);
	fwrite((char*) &biCompression, 4, 1, fptr);
	fwrite((char*) &biSizeImage, 4, 1, fptr);
	fwrite((char*) &biXPelsPerMeter, 4, 1, fptr);
	fwrite((char*) &biYPelsPerMeter, 4, 1, fptr);
	fwrite((char*) &biClrUsed, 4, 1, fptr);
	fwrite((char*) &biClrImportant, 4, 1, fptr);

	for (ushort n=0; n < 256; ++n) {
		temp = numToRGBA(n);
		fwrite((char*) &temp, 4, 1, fptr);
	}

	for (int j=511; j > -1; --j) {
		for (ushort i=0; i < 512; ++i) {
			Buffer[i] = dat->pix[j][i];
		}

		fwrite((char*) Buffer, 1, 512, fptr);
	}

	fclose(fptr);
	return true;	
};
*/

#endif
