/*
 * format-agnostic architecture for image manipulation
 * 
 * Copyright (c) 2009 Vedant Kumar <vminch@gmail.com>
 */

#pragma once

#include "common.hpp"

typedef uint8_t Byte;
typedef uint16_t Word;
typedef uint32_t DWord;

typedef struct rgbaPix {
    Byte r, g, b, a;
} rgbaPix;

typedef struct img {
    Byte pix[512][512];
} img;

extern Byte Buffer[512];
extern Byte Buffer24[(512 * 24) / 8];

extern img (*imr)(FILE*);
extern bool (*imw)(img*, FILE*);

void img_err(const char* fp, const char* reason);
rgbaPix numToRGBA(ushort n);
void s_fread(char* arptr, size_t size, ushort elems, FILE* fptr);

img bmp_read(FILE* fptr);
bool bmp_write(img* dat, FILE* fptr);
img pgm_read(FILE* fptr);
bool pgm_write(img* dat, FILE* fptr);

void hist(img* vol, ushort z, uint32_t* hist);
void cum_hist(img* vol, num* ohist);

ushort find_thresh(num* vhist);
void median_volume_filter(img* volume, bool binary);
void mgrf_apply(img* volume);

enum seg_type {
    RM_ABOVE_T,
    RM_UNDER_T,
    RM_EQNOT_T,
    RM_EQUAL_T,
};

void segment(img* volume, ushort thresh, seg_type keep, ushort rm_val=0);
