/*
 * a modified, 3-class expectation-maximization algorithm
 *  
 * Copyright (c) 2009 Vedant Kumar <vminch@gmail.com>
 */

#ifndef CVP_STATISTICAL_HXX
#define CVP_STATISTICAL_HXX

#include "image.hpp"

static inline num gauss(num x, num miu, num sigma) {
	num con = 1.0 / (sqrt(2 * PI) * sqrt(sigma));
	num d = -(0.5 * square(x - miu)) / sigma;
	return con * exp(d);
}

void general_em(num* model, vector<num>& init_mu, num* x_image, ushort iters,
				num num_gauss, num** pdf) {
	num* variance = new num[(ushort) num_gauss];
	num* prob = new num[(ushort) num_gauss];
	num* P = new num[(ushort) num_gauss];
	num* A = new num[(ushort) num_gauss];

	num* result_ng = new num[(ushort) num_gauss];
	num result_256[256];

	num** dclass = new num*[(ushort) num_gauss];
	for (ushort k=0; k < num_gauss; ++k) {
		dclass[k] = new num[256];
		
		variance[k] = 5;
		prob[k] = 1.0 / num_gauss;
		P[k] = NUM_EPSILON;
	}
	
	for (ushort k=0; k < iters; ++k) {
		for (ushort i=0; i < 256; ++i) {
			num sumprob = NUM_EPSILON;
			
			for (ushort z=0; z < num_gauss; ++z) {
				P[z] = gauss(i, init_mu[z], variance[z]);
				sumprob += P[z] * prob[z];
			}

			for (ushort z=0; z < num_gauss; ++z) {
				dclass[z][i] = ((prob[z] * P[z]) / sumprob) * model[i];
			}
		}

		for (ushort q=0; q < num_gauss; ++q) {
			num A_sum = sum_of(dclass[q], 256);
			A[q] = A_sum / 256;
			
			mat_op(dclass[q], x_image, 256, mul, result_256);
			init_mu[q] = sum_of(result_256, 256) / A_sum;

			mat_op(x_image, init_mu[q], 256, sub, result_256);
			m_apply(result_256, abs, 256, result_256); 
			mat_op(result_256, 2, 256, pown, result_256);
			mat_op(dclass[q], result_256, 256, mul, result_256);
			variance[q] = sum_of(result_256, 256) / A_sum;
		}
		
		num sumof_A = sum_of(A, (uint) num_gauss);
		for (ushort k=0; k < num_gauss; ++k) {
			prob[k] = A[k] / sumof_A;
		}
	}
	
	for (ushort k=0; k < num_gauss; ++k) {
		for (ushort q=0; q < 256; ++q) {
			num i = 1.0 / sqrt(2.0 * PI * variance[k]);
			num i2 = square(q - init_mu[k]) / (2.0 * variance[k]);
			pdf[k][q] = prob[k] * i * exp(-i2);
		}
	}
	
	del_num_table(dclass, (uint) num_gauss);
	delete[] variance;
	delete[] prob;
	delete[] P;
	delete[] A;
	delete[] result_ng;
}

ushort cvp_estimate_density(num* vhist) {
	num mu1_o = 25.0, mu2_o = 100.0, mu3_o = 245.0;
	num prb1_o = 1.0/3.0, prb2_o = 1.0/3.0, prb3_o = 1.0/3.0;
	num seg1_o = 10.0, seg2_o = 10.0, seg3_o = 10.0;

	num dclass1[256];
	num dclass2[256];
	num dclass3[256];
	memset(dclass1, 0, sizeof(num) * 256);
	memset(dclass2, 0, sizeof(num) * 256);
	memset(dclass3, 0, sizeof(num) * 256);
	
	num x_image[256];
	num tmp[256];
	
	for (ushort it=0; it < 256; ++it) {
		x_image[it] = it;
	}

	for (ushort q=0; q < 100; ++q) {
		for (ushort q2=0; q2 < 256; ++q2) {
			num pclass1 = gauss(q2, mu1_o, seg1_o);
			num pclass2 = gauss(q2, mu2_o, seg2_o);
			num pclass3 = gauss(q2, mu3_o, seg3_o);

			num pc1s = prb1_o * pclass1;
			num pc2s = prb2_o * pclass2;
			num pc3s = prb3_o * pclass3;
			num psum = pc1s + pc2s + pc3s;

			if (psum == 0) {
				continue;
			}
			
			dclass1[q2] = (pc1s / psum) * vhist[q2];
			dclass2[q2] = (pc2s / psum) * vhist[q2];
			dclass3[q2] = (pc3s / psum) * vhist[q2];
		}

		num sd1 = sum_of(dclass1, 256);
		num sd2 = sum_of(dclass2, 256);
		num sd3 = sum_of(dclass3, 256);
		
		num pn1 = sd1 / 256.0;
		num pn2 = sd2 / 256.0;
		num pn3 = sd3 / 256.0;
		num pnsum = (pn1 + pn2 + pn3);
		prb1_o = pn1 / pnsum;
		prb2_o = pn2 / pnsum;
		prb3_o = pn3 / pnsum;

		mat_op(dclass1, x_image, 256, mul, tmp);
		mu1_o = sum_of(tmp, 256) / sd1;
		mat_op(dclass2, x_image, 256, mul, tmp);
		mu2_o = sum_of(tmp, 256) / sd2;
		mat_op(dclass3, x_image, 256, mul, tmp);
		mu3_o = sum_of(tmp, 256) / sd3;
		
		mat_op(x_image, mu1_o, 256, sub, tmp);
		mat_op(tmp, 2, 256, pown, tmp);
		mat_op(tmp, dclass1, 256, mul, tmp);
		seg1_o = sum_of(tmp, 256) / sd1;
		mat_op(x_image, mu2_o, 256, sub, tmp);
		mat_op(tmp, 2, 256, pown, tmp);
		mat_op(tmp, dclass2, 256, mul, tmp);
		seg2_o = sum_of(tmp, 256) / sd2;
		mat_op(x_image, mu3_o, 256, sub, tmp);
		mat_op(tmp, 2, 256, pown, tmp);
		mat_op(tmp, dclass3, 256, mul, tmp);
		seg3_o = sum_of(tmp, 256) / sd3;
	}
	
	num sd1 = sum_of(dclass1, 256);
	num sd2 = sum_of(dclass2, 256);
	num sd3 = sum_of(dclass3, 256);
	num pn1 = sd1 / 256.0;
	num pn2 = sd2 / 256.0;
	num pn3 = sd3 / 256.0;
	num pnsum = (pn1 + pn2 + pn3);
	prb1_o = pn1 / pnsum;
	prb2_o = pn2 / pnsum;
	prb3_o = pn3 / pnsum;

	num a1_const = 1 / sqrt(2 * PI * seg1_o);
	num a2_const = 1 / sqrt(2 * PI * seg2_o);
	num a3_const = 1 / sqrt(2 * PI * seg3_o);
	num pclass1 = (2 * seg1_o);
	num pclass2 = (2 * seg2_o);
	num pclass3 = (2 * seg3_o);

	num p1[256], p2[256], p3[256];
	num destimate[256];
	num error[256], sign_error[256], in_img[256];

	for (ushort q=0; q < 256; ++q) {
		num b1_const = square(q - mu1_o) / pclass1;
		num b2_const = square(q - mu2_o) / pclass2;
		num b3_const = square(q - mu3_o) / pclass3;

		p1[q] = prb1_o * a1_const * exp(-b1_const);
		p2[q] = prb2_o * a2_const * exp(-b2_const);
		p3[q] = 7 * prb3_o * a3_const * exp(-b3_const);

		destimate[q] = p1[q] + p2[q] + p3[q];
	}

	num tval = sum_of(destimate, 256);
	
	for (ushort q=0; q < 256; ++q) {
		destimate[q] /= tval;
		error[q] = vhist[q] - destimate[q];
		sign_error[q] = sign(error[q]);
	}
	
	ar("destimate", destimate, 256);
	ar("error", error, 256);

	vector<num> posn;
	vector<num> init_mu;
	
	posn.push_back(0);
	for (ushort q=0; q < 255; ++q) {
		if (sign_error[q] != sign_error[q+1]) {
			if (q != 0) { posn.push_back(q); }
		}
	}
	posn.push_back(255);
	
	ar("posn vector", posn, posn.size());

	for (ushort q=0; q < posn.size() - 1; ++q) {
		if ((posn[q+1] - posn[q]) > 10) {
			init_mu.push_back((posn[q+1] + posn[q]) / 2.0);
		}
	}

	num num_gauss = init_mu.size();
	num** pdf = new num*[(ushort) num_gauss];
	for (ushort k=0; k < num_gauss; ++k) {
		pdf[k] = new num[256];
	}
	
	m_apply(error, abs, 256, tmp);
	num const_ = sum_of(tmp, 256);
	mat_op(tmp, const_, 256, divn, in_img);
	
	ar("init_mu: pre gen_em", init_mu, (uint) num_gauss);
	general_em(in_img, init_mu, x_image, 50, num_gauss, pdf);
	ar("init_mu: post gen_em", init_mu, (uint) num_gauss);

	ushort t1=10, t2;
	while (p1[t1] > p2[t1]) {
		t1++;
	}

	t2 = t1 + 10;
	while (p2[t2] > p3[t2]) {
		t2++;
	}

	for (ushort q=0; q < init_mu.size() /*3*/; ++q) {
		mat_op(sign_error, pdf[q], 256, mul, tmp);

		if (init_mu[q] <= t1+5) {
			mat_op(p1, tmp, 256, add, p1);
		} else if ((init_mu[q] > t1) && (init_mu[q] < t2)) {
			mat_op(p2, tmp, 256, add, p2);
		} else {
			mat_op(p3, tmp, 256, add, p3); 
		}
	}

	for (ushort q=0; q < 256; ++q) {
		p1[q] = abs(p1[q]);
		p2[q] = abs(p2[q]);
		p3[q] = abs(p3[q]);
	}
	
	ar("p1", p1, 256);
	ar("p2", p2, 256);
	ar("p3", p3, 256);

	dp("determining segmentation threshold");
	ushort q = 15;
	ushort cap = 15;
	while (p1[q] > p2[q]) {
		q++;
	}

	cap += q;

	while (p2[cap] > (1.5 * p3[cap])) {
		cap++;
	}
	
	del_num_table(pdf, (uint) num_gauss);
	
	return cap;
}

#endif
