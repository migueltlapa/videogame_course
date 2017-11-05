/*
 * Program.h
 *
 *  Created on: 09/05/2014
 *      Author: HugoIván
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <string>
#include "Matrix4.h"

using namespace std;

class Program {
private:
	unsigned int programId;
	unsigned int vertexShaderId;
	unsigned int fragmentShaderId;
	unsigned int geometryShaderId;
public:
	Program();
	bool setVertexShader(string filename);
	bool setFragmentShader(string filename);
	bool setGeometryShader(string filename);
	bool build(bool attachVertexShader, bool attachGeometryShader, bool attachFragmentShader);
	unsigned int getAttributeLocation(string attribute);
	virtual ~Program();
	unsigned int getId();
	void bind();
	void setMatrix(string variable, Matrix4 &matrix);
};

#endif /* PROGRAM_H_ */
