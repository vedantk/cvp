/*
 * cvp_pgm.hxx
 * read/write access to 512x512 PGM files.
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

#ifndef CVP_PGM_HXX
#define CVP_PGM_HXX

#include "../cvp_common.hxx"
#include "cvp_image.hxx"

static int PNMReaderGetChar(FILE* fp) {
	char c;
	int result;

	if ((result = getc(fp)) == EOF)	{
		return '\0';
	}

	c = (char) result;
	if (c == '#') {
		do {
			if ((result = getc(fp)) == EOF) {
				return '\0';
			}
			c = (char) result;
		} while (c != '\n');
	}

	return c;
}

static int PNMReaderGetInt(FILE* fp) {
	char c;
	int result = 0;

	do {
		c = PNMReaderGetChar(fp);
	} while ((c < '1') || (c > '9'));

	do {
		result = result * 10 + (c - '0');
		c = PNMReaderGetChar(fp);
	} while ((c >= '0') && (c <= '9'));

	ungetc(c, fp);
	return result;
}

static img pgm_read(FILE* fptr) {
	img dat;
	char magic[3];
	int i;
	char c;
	
	do {
		c = PNMReaderGetChar(fptr);
	} while (c != 'P');

	magic[0] = c;
	magic[1] = PNMReaderGetChar(fptr);
	magic[2] = '\0';

	i = PNMReaderGetInt(fptr); // rows
	i = PNMReaderGetInt(fptr); // cols
	i = PNMReaderGetInt(fptr); // max grayscale value
	
	c = getc(fptr);
	if (c == 0x0d) {
		c = getc(fptr);
		if (c != 0x0a) {
			ungetc(c, fptr);
		}
	}

	if (strncmp(magic, "P5", 2)) {
		fclose(fptr);
		img_err("~*.pgm", "Unsupported PGM Binary.");
	}

	for (int j=0; j < 512; ++j) {
		fread((char*) Buffer, 1, 512, fptr);
		
		for (ushort i=0; i < 512; ++i) {
			dat.pix[j][i] = Buffer[i];
			dat.hist[ dat.pix[j][i] ] += 1;
		}
	}
	
	fclose(fptr);
	return dat;
};

static bool pgm_write(img* dat, FILE* fptr) {
	fprintf(fptr, "P5\n512 512\n255\n");
	
	for (int j=0; j < 512; ++j) {
		for (ushort i=0; i < 512; ++i) {
			Buffer[i] = dat->pix[j][i];
		}

		fwrite((char*) Buffer, 1, 512, fptr);
	}
	
	fclose(fptr);
	return true;
};

#endif
