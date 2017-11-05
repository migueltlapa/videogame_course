/*
 * Program.cpp
 *
 *  Created on: 09/05/2014
 *      Author: HugoIván
 */

#include "GLContext.h"
#include "Program.h"
#include "ShaderManager.h"

Program::Program() {
	programId        = 0;
	vertexShaderId   = 0;
	fragmentShaderId = 0;
	geometryShaderId = 0;
}

bool Program::setVertexShader(string filename) {
	vertexShaderId = ShaderManager::compile(filename, GLContext::VERTEX_SHADER);
	if(!ShaderManager::compiled(vertexShaderId)) vertexShaderId = 0;
	return vertexShaderId != 0;
}

bool Program::setFragmentShader(string filename) {
	fragmentShaderId = ShaderManager::compile(filename, GLContext::FRAGMENT_SHADER);
	if(!ShaderManager::compiled(fragmentShaderId)) fragmentShaderId = 0;
	return fragmentShaderId != 0;
}

bool Program::setGeometryShader(string filename) {
	geometryShaderId = ShaderManager::compile(filename, GLContext::GEOMETRY_SHADER);
	if(!ShaderManager::compiled(geometryShaderId)) geometryShaderId = 0;
	return geometryShaderId != 0;
}

bool Program::build(bool attachVertexShader, bool attachGeometryShader, bool attachFragmentShader) {
	if(attachVertexShader && vertexShaderId == 0) return false;
	if(attachGeometryShader && geometryShaderId == 0) return false;
	if(attachFragmentShader && fragmentShaderId == 0) return false;
	programId = GLContext::buildProgram(attachVertexShader?   vertexShaderId   : 0,
						                attachGeometryShader? geometryShaderId : 0,
						                attachFragmentShader? fragmentShaderId : 0);
	return true;
}

Program::~Program() {
	GLContext::deleteProgram(this->programId);
}

unsigned int Program::getId() {
	return this->programId;
}

unsigned int Program::getAttributeLocation(string attribute) {
	return GLContext::getAttributeLocation(this->programId, attribute.c_str());
}

void Program::bind() {
	GLContext::bindProgram(this->programId);
}

void Program::setMatrix(string name, Matrix4 &matrix) {
	GLContext::setMatrix(this->programId, name, matrix);
}
