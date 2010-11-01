/*
 * format-agnostic architecture for image manipulation
 * 
 * Copyright (c) 2009 Vedant Kumar <vminch@gmail.com>
 */
 
#include "image.hpp"

Byte Buffer[512];
Byte Buffer24[(512 * 24) / 8];

img (*imr)(FILE*) = NULL;
bool (*imw)(img*, FILE*) = NULL;

void img_err(const char* fp, const char* reason) {
    cerr << ":: " << fp << " could not be processed.\n:: " << reason << "\n";
    hit_enter();
    exit(1);
}

rgbaPix numToRGBA(ushort n) {
    rgbaPix temp;
    temp.r = n;
    temp.g = n;
    temp.b = n;
    temp.a = 0;
    return temp;
}

void s_fread(char* arptr, size_t size, ushort elems, FILE* fptr) {
    if (fread(arptr, size, elems, fptr) != (size * elems)) {
		img_err("*", "corrupted file read");
    }
}
