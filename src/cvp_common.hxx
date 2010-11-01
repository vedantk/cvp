/*
 * cvp_common.hxx
 * simple code used by cvp components.
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

#ifndef CVP_COMMON_HXX
#define CVP_COMMON_HXX

#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

typedef unsigned int uint;
typedef unsigned short ushort;
typedef double num;

#define PI 3.141592653589793
#define DEBUG__
const ushort white = 255;

class Table {
private:
	num* data;
	ushort nr, nc, x, y;

public:
	Table(ushort rows, ushort cols) {
		nr = rows;
		nc = cols;
		data = new num[nr * nc];
	};

	~Table() {
		delete[] data;
	};

	inline num& operator() (ushort r, ushort c) {
		return data[r*nr + c];
	}

	inline num operator() (ushort r, ushort c) const {
		return data[r*nr + c];
	};
};

template<class c> void dp(c obj) {
	cout << obj << "\n";
}

template<class c> void ar(c& arr, ushort amt) {
	cout << "[";
	for (ushort q=0; q < amt; ++q) {
		cout << arr[q] << ", ";
	}
	cout << "empty]\n";
}

void hit_enter() {
	cout << "Press 'enter' or 'return' to continue.\n";
	cin.get();
}

inline num sum_of(num* array) {
	num t = 0;
	for (ushort q=0; q < 256; ++q) {
		t += array[q];
	}
	return t;
}

#endif
