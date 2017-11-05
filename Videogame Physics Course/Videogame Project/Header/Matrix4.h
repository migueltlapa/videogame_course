/*
 * Matrix4.h
 *
 *  Created on: 14/01/2014
 *      Author: Iván
 */

#ifndef MATRIX4_H_
#define MATRIX4_H_

#include <iostream>
using namespace std;

class Matrix4 {
public:
	float values[16];
	Matrix4();
	virtual ~Matrix4();
	void setIdentity();
	void set(int c, int r, float v);
	float get(int c, int r) const;
	Matrix4 operator*(const Matrix4 &m1);
	friend ostream& operator<<(ostream& o, const Matrix4& m);
	void setOrtho(float ratio, float left, float right, float bottom, float top, float near, float far);
	void setPerspective(float fov, float aspect, float nearZ, float farZ);
};

#endif /* MATRIX4_H_ */
