/*
 * SpriteVA.h
 *
 *  Created on: 09/05/2014
 *      Author: Iván
 */

#ifndef SPRITEVA_H_
#define SPRITEVA_H_

#include "VertexArray.h"

class SpriteVA: public VertexArray {

public:
	SpriteVA() : VertexArray(2) {
		vertexCount = 4;
		positions = new float[8];
		texcoords = new float[8];
		positions[0] = 0;
		positions[1] = 0;
		positions[2] = 0;
		positions[3] = 1;
		positions[4] = 1;
		positions[5] = 0;
		positions[6] = 1;
		positions[7] = 1;
		texcoords[0] = 0;
		texcoords[1] = 0;
		texcoords[2] = 0;
		texcoords[3] = 1;
		texcoords[4] = 1;
		texcoords[5] = 0;
		texcoords[6] = 1;
		texcoords[7] = 1;
	}

	void bindPositions(GLuint positionLoc) {
		GLContext::bindVertexArray(vertexArrayId);
		GLContext::bindBuffer(buffers[0], positions, sizeof(float) * 8, 2, positionLoc, false);
	}

	void bindTexcoords(GLuint texcoordLoc) {
		GLContext::bindVertexArray(vertexArrayId);
		GLContext::bindBuffer(buffers[1], texcoords, sizeof(float) * 8, 2, texcoordLoc, false);
	}

};

#endif /* SPRITEVA_H_ */
