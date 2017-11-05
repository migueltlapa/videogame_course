/*
 * Transforms.h
 *
 *  Created on: 16/01/2014
 *      Author: Iván
 */

#ifndef TRANSFORMS_H_
#define TRANSFORMS_H_

#include "Matrix4.h"
#include <list>

using namespace std;

class Transforms {

private:
	static list<Matrix4> matrixStack;

public:
	static void pushMatrix(Matrix4 m);
	static Matrix4 popMatrix();
	static void loadIdentity();
	static void translate(Matrix4 &m, float tx, float ty, float tz);
	static void rotateX(Matrix4 &m, float angle);
	static void rotateY(Matrix4 &m, float angle);
	static void rotateZ(Matrix4 &m, float angle);
	static void scale(Matrix4 &m, float sx, float sy, float sz);
};

#endif /* TRANSFORMS_H_ */
