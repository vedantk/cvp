/*
 * common header
 * 
 * Copyright (c) 2009 Vedant Kumar <vminch@gmail.com>
 */

#pragma once

#if defined (_MSC_VER) || defined (__CYGWIN__) || defined (__MINGW32__)
# define MSWIN
#else
# define POSIX
#endif

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <cmath>
#include <iostream>
#include <map>
#include <vector>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

using namespace std;

typedef unsigned int uint;
typedef unsigned short ushort;
typedef long double num;

const num PI = 3.141592653589793;

const ushort white = 0xff;

extern ushort num_imgs;
extern num NUM_EPSILON;

template<class c> void dp(c obj) {
	cout << obj << "\n";
}

template<class c> void ar(const char* name, c& arr, ushort amt) {
	cout << name << " = [";
	for (ushort q=0; q < amt; ++q) {
		cout << arr[q] << ", ";
	}
	cout << "empty]\n";
}

void hit_enter();
void error(const char* reason);

num sum_of(num* array, uint len);
num square(num x);
num sign(num val);

enum m_op {
	mul, divn, sub, pown, add
};

void mat_op(num* lhs, num* rhs, uint len, m_op op, num* result);
void mat_op(num* lhs, num delta, uint len, m_op op, num* result);
void del_num_table(num** t, ushort rows);

#define m_apply(lhs, func, len, result) { \
		for (uint it=0; it < len; ++it) { \
			result[it] = func(lhs[it]); \
		} \
	}
	
