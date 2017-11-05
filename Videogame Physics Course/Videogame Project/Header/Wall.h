/*
 * Wall.h
 *
 *  Created on: May 10, 2014
 *      Author: joelmart
 */

#ifndef WALL_H_
#define WALL_H_

#include "Character.h"

class Wall: public Character{

private:
	int status;

public:

	Wall() : Character(0, 0, 0) {
		this->status = 0;
	}

	Wall(float x, float y, float z) : Character(x, y, z) {
		this->status = 0;
	}

	virtual ~Wall(){ /* Empty */ }

	void update() {
		x += 0.05;
		y -= 0.09;
//		if(abs(x) >= 10) x *= -1;
//		if(abs(y) >= 10) y *= -1;
	}

	void draw() {
		if(program == NULL || vertexArray == NULL) return;
		program->bind();
		texture->bind();
		vertexArray->bind();
		GLContext::drawTriangleStrip(vertexArray->getVertexCount());
	}

};



#endif /* Wall_H_ */
