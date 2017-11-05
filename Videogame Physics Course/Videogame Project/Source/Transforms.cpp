/*
 * Transforms.cpp
 *
 *  Created on: 16/01/2014
 *      Author: Iván
 */

#include "Transforms.h"
#include <cmath>
#include <list>

using namespace std;

list<Matrix4> Transforms::matrixStack;

void Transforms::pushMatrix(Matrix4 m) {
	matrixStack.push_front(m);
}

Matrix4 Transforms::popMatrix() {
	if(matrixStack.empty()) {
		Matrix4 m;
		return m;
	}
	return matrixStack.front();
}

void Transforms::loadIdentity() {
	matrixStack.empty();
}

void Transforms::translate(Matrix4 &m, float tx, float ty, float tz) {
	Matrix4 tm;
	tm.set(3, 0, tx);
	tm.set(3, 1, ty);
	tm.set(3, 2, tz);
	m = m * tm;
}

void Transforms::rotateX(Matrix4 &m, float angle) {
	Matrix4 rm;
	float rad = angle * M_PI / 180.0;
	rm.set(1, 1, cos(rad));
	rm.set(2, 1, -sin(rad));
	rm.set(1, 2, sin(rad));
	rm.set(2, 2, cos(rad));
	m = m * rm;
}

void Transforms::rotateY(Matrix4 &m, float angle) {
	Matrix4 rm;
	float rad = angle * M_PI / 180.0;
	rm.set(0, 0, cos(rad));
	rm.set(2, 0, sin(rad));
	rm.set(0, 2, -sin(rad));
	rm.set(2, 2, cos(rad));
	m = m * rm;
}

void Transforms::rotateZ(Matrix4 &m, float angle) {
	Matrix4 rm;
	float rad = angle * M_PI / 180.0;
	rm.set(0, 0, cos(rad));
	rm.set(1, 0, -sin(rad));
	rm.set(0, 1, sin(rad));
	rm.set(1, 1, cos(rad));
	m = m * rm;
}

void Transforms::scale(Matrix4 &m, float sx, float sy, float sz) {
	Matrix4 sm;
	sm.set(0, 0, sx);
	sm.set(1, 1, sy);
	sm.set(2, 2, sz);
	m = m * sm;
}
