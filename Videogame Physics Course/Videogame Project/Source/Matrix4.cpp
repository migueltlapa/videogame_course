/*
 * Matrix3.cpp
 *
 *  Created on: 14/01/2014
 *      Author: Iván
 */

#include "Matrix4.h"
#include <cmath>
#include <iostream>

using namespace std;

Matrix4::Matrix4() {
	setIdentity();
}

Matrix4::~Matrix4() {
}

void Matrix4::setIdentity() {
	for(int i = 1; i < 16; i ++) values[i] = 0;
	values[0]  = 1;
	values[5]  = 1;
	values[10] = 1;
	values[15] = 1;
}

void Matrix4::set(int c, int r, float v) {
	if(c < 0 || r < 0 || c > 3 || r > 3) return;
	values[r * 4 + c] = v;
}

float Matrix4::get(int c, int r) const {
	if(c < 0 || r < 0 || c > 3 || r > 3)  return 0;
	return values[r * 4 + c];
}

Matrix4 Matrix4::operator *(const Matrix4& m) {
	Matrix4 res;
	for(int c = 0; c < 4; c ++) {
		for(int r = 0; r < 4; r ++) {
			float sum = 0;
			for(int k = 0; k < 4; k ++) {
				sum += get(k, r) * m.get(c, k);
			}
			res.set(c, r, sum);
		}
	}
	return res;
}

ostream& operator<<(ostream& o, const Matrix4& m) {
	for(int r = 0; r < 4; r ++) {
		for(int c = 0; c < 4; c ++) {
			o << m.get(c, r) << " ";
		}
			o << endl;
	}
	return o;
}

	void Matrix4::setOrtho(float ratio, float left, float right, float bottom, float top, float near, float far) {
		setIdentity();
		if(ratio > 1.0) {
			left *= ratio;
			right *= ratio;
		} else {
			top /= ratio;
			bottom /= ratio;
		}
		set(0, 0, 2 / (right - left));
		set(1, 1, 2 / (top - bottom));
		set(2, 2, 2 / (near - far));
		set(3, 0, -(left + right) / (right - left));
		set(3, 1, -(bottom + top) / (top - bottom));
		set(3, 2, -(near + far) / (near - far));
	}

	void Matrix4::setPerspective(float fov, float aspect, float nearZ, float farZ) {
		setIdentity();
		float f = 1.0 / tan (fov * (M_PI / 360.0));
		set(0, 0, f / aspect);
		set(1, 1, f);
		set(2, 2, (nearZ - farZ) / (2 * nearZ * farZ));
		set(2, 3, (-farZ - nearZ) / (farZ - nearZ));
		set(3, 2, -1.0f);

//		set(2, 2, (farZ + nearZ) / (nearZ - farZ));
//		set(2, 3, (2.0f * farZ * nearZ) / (nearZ - farZ));
//		set(3, 2, -1.0f);
		set(3, 3, 0.0f);
	}

