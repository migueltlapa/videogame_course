/*
 * VertexArray.h
 *
 *  Created on: 09/05/2014
 *      Author: HugoIván
 */

#ifndef VERTEXARRAY_H_
#define VERTEXARRAY_H_

class VertexArray {

protected:
	float *positions;
	float *colors;
	float *normals;
	float *texcoords;
	unsigned int *indexes;
	unsigned int vertexArrayId;
	unsigned int vertexCount;
	unsigned int *buffers;

public:
	VertexArray(int bufferCount) {
		if(bufferCount < 1) bufferCount = 1;
		this->vertexArrayId   = GLContext::genVertexArray();
		buffers = GLContext::createBuffers(bufferCount);
		positions = NULL;
		colors    = NULL;
		normals   = NULL;
		texcoords = NULL;
		indexes   = NULL;
		vertexCount = 0;
	}

	virtual void bindPositions(GLuint positionLoc) = 0;

	void bindTexcoords(GLuint texcoordLoc) {}

	void bindColors(GLuint colorLoc) {}

	void bindNormals(GLuint colorLoc) {}

	void bind() {
		GLContext::bindVertexArray(this->vertexArrayId);
	}

	unsigned int getVertexCount() {
		return vertexCount;
	}

	unsigned int getId() {
		return vertexArrayId;
	}

	virtual ~VertexArray() {
		delete[] positions;
		delete[] colors;
		delete[] normals;
		delete[] texcoords;
		delete[] buffers;
		GLContext::deleteVertexArray(vertexArrayId);
	}

};

#endif /* VERTEXARRAY_H_ */
