/*
 * Floor.h
 *
 *  Created on: 09/05/2014
 *      Author: Iván
 */

#ifndef FLOOR_H_
#define FLOOR_H_

#include "Character.h"

class Floor : public Character {

private:
	int status;

public:

	Floor() : Character(0, 0, 0) {
		this->status = 0;
	}

	Floor(float x, float y, float z) : Character(x, y, z) {
		this->status = 0;
	}

	void update() {
	}

	void draw() {
		if(program == NULL || vertexArray == NULL) return;
		program->bind();
		texture->bind();
		vertexArray->bind();
		GLContext::drawTriangleStrip(vertexArray->getVertexCount());
	}

};

#endif /* BALL_H_ */

