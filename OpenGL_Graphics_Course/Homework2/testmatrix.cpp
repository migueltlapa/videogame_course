/*
 * testmatrix.cpp
 *
 *  Created on: Feb 10, 2014
 *      Author: mtlapa
 */

#include "Matrix4.h"
#include <iostream>
using namespace mat4;
using namespace std;

int main(){
	Matrix4 m1,m2,m3;
	cout <<m1 << endl;
	m2.set(1, 0, 3);
	m2.set(3,1, 2);
	m2.set(0, 2, 4);
	m2.set(2, 3, 5);
	cout <<m2 << endl;
	cout <<m2.get(3, 1) << endl;
	m3 = m1*m2;
	cout <<m3 << endl;

	return 0;

}

