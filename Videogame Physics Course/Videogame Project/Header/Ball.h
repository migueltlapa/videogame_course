/*
 * Ball.h
 *
 *  Created on: 09/05/2014
 *      Author: Iván
 */

#ifndef BALL_H_
#define BALL_H_

#include "Character.h"

class Ball : public Character {

private:
	int status;

public:

	Ball() : Character(0, 0, 0) {
		this->status = 0;
	}

	Ball(float x, float y) : Character(x, y, 0) {
		this->status = 0;
	}

	void update() {
		x += 0.05;
		y -= 0.02;
		if(abs(x) >= 10) x *= -1.0;
		if(abs(y) >= 10) y *= -1.0;
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

