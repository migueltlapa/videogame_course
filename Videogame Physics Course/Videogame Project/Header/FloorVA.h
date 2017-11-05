/*
 * FloorVA.h
 *
 *  Created on: 09/05/2014
 *      Author: Joel
 */

#ifndef FLOORVA_H_
#define FLOORVA_H_

#include "VertexArray.h"

class FloorVA: public VertexArray {

public:
	FloorVA(float x, float y, float z) : VertexArray(2) {
		vertexCount = 4;
		positions = new float[12];
		texcoords = new float[8];
		float xx = x/2.0;
		float yy = y/2.0;
		float zz = (z/2.0)-20;
		cout << xx << " " << yy << " " << z << endl;
		positions[ 0] = -xx;  positions[ 1] = yy;   positions[ 2] =  zz;
		positions[ 3] = -xx;  positions[ 4] = yy;   positions[ 5] = -zz;
		positions[ 6] =  xx;  positions[ 7] = yy;   positions[ 8] =  zz;
		positions[ 9] =  xx;  positions[10] = yy;   positions[11] = -zz;
		texcoords[0] = 0;   texcoords[1] = 0;
		texcoords[2] = 0;   texcoords[3] = 1;
		texcoords[4] = 1;   texcoords[5] = 0;
		texcoords[6] = 1;   texcoords[7] = 1;
	}

	void bindPositions(GLuint positionLoc) {
		GLContext::bindVertexArray(vertexArrayId);
		GLContext::bindBuffer(buffers[0], positions, sizeof(float) * 12, 3, positionLoc, false);
	}

	void bindTexcoords(GLuint texcoordLoc) {
		GLContext::bindVertexArray(vertexArrayId);
		GLContext::bindBuffer(buffers[1], texcoords, sizeof(float) * 8, 2, texcoordLoc, false);
	}

};

#endif /* FLOORVA_H_ */
