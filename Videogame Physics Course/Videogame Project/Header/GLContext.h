/*
 * Context.h
 *
 *  Created on: 09/05/2014
 *      Author: Iván
 */

#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <iostream>
#include "Matrix4.h"

using namespace std;

class GLContext {

public:

	static const unsigned int VERTEX_SHADER   = GL_VERTEX_SHADER;
	static const unsigned int FRAGMENT_SHADER = GL_FRAGMENT_SHADER;
	static const unsigned int GEOMETRY_SHADER = GL_GEOMETRY_SHADER;
	static const unsigned int REPEAT_WRAP     = GL_REPEAT;
	static const unsigned int CLAMP_WRAP      = GL_CLAMP;
	static const unsigned int NEAREST_FILTER  = GL_NEAREST;
	static const unsigned int LINEAR_FILTER   = GL_LINEAR;

	static void init() {
		glewInit();
	}

	static void onDisplay(void (*displayFunc)()) {
		glutDisplayFunc(displayFunc);
	}

	static void onReshape(void (*reshapeFunc)(int, int)) {
		glutReshapeFunc(reshapeFunc);
	}

	static void setBackground(float r, float g, float b) {
		glClearColor(r, g, b, 1);
	}

	static void clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	static void viewPort(int width, int height) {
		glViewport(0, 0, width, height);
	}

	static void Enable(GLenum tag){
		glEnable(tag);
	}

	static void swap() {
		glutSwapBuffers();
	}

	static void repaint() {
		glutPostRedisplay();
	}

	static GLuint genVertexArray() {
		unsigned int vaId;
		glGenVertexArrays(1, &vaId);
	    return vaId;
	}

	static void bindVertexArray(GLuint vaId) {
		glBindVertexArray(vaId);
	}

	static void deleteVertexArray(GLuint vaId) {
		glDeleteVertexArrays(1, &vaId);
	}

	static unsigned int* createBuffers(unsigned int count) {
		unsigned int *buffers = new unsigned int[count];
	    glGenBuffers(count, buffers);
	    return buffers;
	}

	static void bindBuffer(GLuint bufferId, float *array, int arraySize, int componentCount, GLuint attributeLoc, bool staticDraw) {
	    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	    glBufferData(GL_ARRAY_BUFFER, arraySize, array, staticDraw? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	    glVertexAttribPointer(attributeLoc, componentCount, GL_FLOAT, 0, 0, 0);
	    glEnableVertexAttribArray(attributeLoc);
	}

	static unsigned int  getAttributeLocation(GLuint programId, string attribute) {
		 return glGetAttribLocation(programId, attribute.c_str());
	}

	static unsigned int  buildProgram(GLuint vertexShaderId, GLuint geometryShaderId, GLuint fragmentShaderId) {
		GLuint programId = glCreateProgram();
		if(vertexShaderId > 0)   glAttachShader(programId, vertexShaderId);
		if(geometryShaderId > 0) glAttachShader(programId, geometryShaderId);
		if(fragmentShaderId > 0) glAttachShader(programId, fragmentShaderId);
		glLinkProgram(programId);
		return programId;
	}

	static void bindProgram(GLuint programId) {
		glUseProgram(programId);
	}

	static void deleteProgram(GLuint programId) {
		glDeleteProgram(programId);
	}

	static unsigned int compileShader(string source_str, unsigned int shaderType) {
		char const *source = source_str.c_str();
		GLuint shaderId = glCreateShader(shaderType);
		glShaderSource(shaderId, 1, &source, 0);
		glCompileShader(shaderId);
		return shaderId;
	}

	static int getShaderCompileStatus(unsigned int shaderId) {
		int params;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &params);
		return params;
	}

	static void printShaderCompileLog(unsigned int shaderId) {
		int maxLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);
		vector<char> errors(maxLength);
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errors[0]);
		vector<char>::iterator iterador = errors.begin();
		while(iterador != errors.end()) {
			char valor = *iterador;
		    cout << valor;
		    iterador ++;
		}
		cout << endl;
	}

	static void bindTexture(unsigned int textureId) {
		glBindTexture(GL_TEXTURE_2D, textureId);
	}

	static void deleteTexture(unsigned int textureId) {
		glDeleteTextures(1, &textureId);
	}

	static void drawTriangleStrip(unsigned int vertexCount) {
		glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount);
	}

	static void setMatrix(unsigned int programId, string matrixName, Matrix4 &matrix) {
		unsigned int matrixLoc = glGetUniformLocation(programId, matrixName.c_str());
		glUseProgram(programId);
		glUniformMatrix4fv(matrixLoc, 1, true, matrix.values);
	}

};

#endif /* CONTEXT_H_ */
