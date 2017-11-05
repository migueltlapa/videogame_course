/*
 * WallVA.h
 *
 *  Created on: May 10, 2014
 *      Author: joelmart
 */

#ifndef WALLVA_H_
#define WALLVA_H_

#include "VertexArray.h"
#include <iostream>

#define SMALL_SIDE		1
#define LARGE_SIDE		1

class WallVA: public VertexArray {

public:
	WallVA(std::string wallType) : VertexArray(2) {
		vertexCount = 10;
		positions = new float[vertexCount*3];
		texcoords = new float[vertexCount*2];
		if(wallType == "Vertical"){
			float x = LARGE_SIDE/2.0, y = LARGE_SIDE/2.0, z = SMALL_SIDE/2.0;
			positions[ 0] = -x;  positions[ 1] = -y;  positions[ 2] =  z;
			positions[ 3] = -x;  positions[ 4] =  y;  positions[ 5] =  z;
			positions[ 6] =  x;  positions[ 7] = -y;  positions[ 8] =  z;
			positions[ 9] =  x;  positions[10] =  y;  positions[11] =  z;
			positions[12] =  x;  positions[13] = -y;  positions[14] = -z;
			positions[15] =  x;  positions[16] =  y;  positions[17] = -z;
			positions[18] = -x;  positions[19] = -y;  positions[20] = -z;
			positions[21] = -x;  positions[22] =  y;  positions[23] = -z;
			positions[24] = -x;  positions[25] = -y;  positions[26] =  z;
			positions[27] = -x;  positions[28] =  y;  positions[29] =  z;
		}
		else if(wallType == "Horizontal"){
			float x = SMALL_SIDE/2.0, y = LARGE_SIDE/2.0, z = LARGE_SIDE/2.0;
			positions[ 0] = -x;  positions[ 1] = -y;  positions[ 2] =  z;
			positions[ 3] = -x;  positions[ 4] =  y;  positions[ 5] =  z;
			positions[ 6] =  x;  positions[ 7] = -y;  positions[ 8] =  z;
			positions[ 9] =  x;  positions[10] =  y;  positions[11] =  z;
			positions[12] =  x;  positions[13] = -y;  positions[14] = -z;
			positions[15] =  x;  positions[16] =  y;  positions[17] = -z;
			positions[18] = -x;  positions[19] = -y;  positions[20] = -z;
			positions[21] = -x;  positions[22] =  y;  positions[23] = -z;
			positions[24] = -x;  positions[25] = -y;  positions[26] =  z;
			positions[27] = -x;  positions[28] =  y;  positions[29] =  z;
		}

		texcoords[ 0] = 0; texcoords[ 1] = 0;
		texcoords[ 2] = 0; texcoords[ 3] = 1;
		texcoords[ 4] = 1; texcoords[ 5] = 0;
		texcoords[ 6] = 1; texcoords[ 7] = 1;
		texcoords[ 8] = 0; texcoords[ 9] = 0;
		texcoords[10] = 0; texcoords[11] = 1;
		texcoords[12] = 1; texcoords[13] = 0;
		texcoords[14] = 1; texcoords[15] = 1;
		texcoords[16] = 0; texcoords[17] = 0;
		texcoords[18] = 0; texcoords[19] = 1;

	}

	void bindPositions(GLuint positionLoc) {
		GLContext::bindVertexArray(vertexArrayId);
		GLContext::bindBuffer(buffers[0], positions, sizeof(float)*vertexCount*3, 3, positionLoc, false);
	}

	void bindTexcoords(GLuint texcoordLoc) {
		GLContext::bindVertexArray(vertexArrayId);
		GLContext::bindBuffer(buffers[1], texcoords, sizeof(float)*vertexCount*2, 2, texcoordLoc, false);
	}

};



#endif /* SPRITEWALLVA_H_ */
