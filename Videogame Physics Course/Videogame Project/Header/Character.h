/*
 * Character.h
 *
 *  Created on: 10/05/2014
 *      Author: Iván
 */

#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "Program.h"
#include "VertexArray.h"
#include "Texture.h"
#include <cmath>

class Character {

protected:
	Program* program;
	VertexArray* vertexArray;
	Texture* texture;

public:
	float x, y, z;
	float yaw, pitch, roll;  // Y-angle, X-angle, Z-angle
	float scaleX, scaleY, scaleZ;

	Character(float x, float y, float z) {
		this->program = NULL;
		this->vertexArray = NULL;
		this->texture = NULL;
		this->x      = x;
		this->y      = y;
		this->z      = z;
		this->yaw    = 0;
		this->pitch  = 0;
		this->roll   = 0;
		this->scaleX = 1;
		this->scaleY = 1;
		this->scaleZ = 1;
	}

	void setVertexArray(VertexArray *vertexArray) {
		this->vertexArray = vertexArray;
	}

	void setProgram(Program *program) {
		this->program = program;
	}

	void setTexture(Texture *texture) {
		this->texture = texture;
	}

	virtual void draw()   = 0;
	virtual void update() = 0;
};

#endif /* CHARACTER_H_ */
