/*
 * Ghost.h
 *
 *  Created on: 10/05/2014
 *      Author: HugoIván
 */

#ifndef GHOST_H_
#define GHOST_H_

#include "Character.h"

class Ghost : public Character {

private:
	int status;

public:

	Ghost() : Character(0, 0, 0) {
		this->status = 0;
	}

	Ghost(float x, float y) : Character(x, y, 0) {
		this->status = 0;
	}

	void update() {
		x -= 0.02;
		y += 0.03;
		if(abs(x) >= 10) x *= -1;
		if(abs(y) >= 10) y *= -1;
	}

	void draw() {
		if(program == NULL || vertexArray == NULL) return;
		program->bind();
		texture->bind();
		vertexArray->bind();
		GLContext::drawTriangleStrip(vertexArray->getVertexCount());
	}

};

#endif /* GHOST_H_ */
