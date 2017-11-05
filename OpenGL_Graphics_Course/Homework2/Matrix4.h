/*
 * Matrix4.h
 *
 *  Created on: Feb 10, 2014
 *      Author: mtlapa
 */

#ifndef MATRIX4_H_
#define MATRIX4_H_
#include <iostream>
using namespace std;

namespace mat4 {

class Matrix4 {

public:
	float values[16];
	Matrix4();
	virtual ~Matrix4();
	void setIdentity();
	void set(int col, int row, float value);
	float get(int col, int row)const;
	Matrix4 operator*(const Matrix4 &C);
	friend ostream& operator <<(ostream &o, const Matrix4 &c);
};

} /* namespace mat4 */

#endif /* MATRIX4_H_ */
