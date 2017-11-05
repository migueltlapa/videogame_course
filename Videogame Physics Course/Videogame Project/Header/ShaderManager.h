/*
 * Shader.h
 *
 *  Created on: 09/05/2014
 *      Author: HugoIván
 */

#ifndef SHADER_H_
#define SHADER_H_

#include "GLContext.h"
#include <iostream>
#include <fstream>

using namespace std;

class ShaderManager {
private:
	static string load(string filename) {
		string contents;
	    ifstream stream(filename.c_str());

	    if(stream.is_open()) {
	    	string line = "";
	        while(getline(stream, line)) {
	        	contents += line + "\n";
	        }
	        stream.close();
	    } else {
	    	return "No se pudo leer el archivo";
	    }
	    return contents;
	}

public:

	static unsigned int compile(string filename, unsigned int shaderType) {
		string source_str = load(filename);
		unsigned int shaderId = GLContext::compileShader(source_str, shaderType);
		return shaderId;
	}

	static bool compiled(unsigned int shaderId) {
		int params = GLContext::getShaderCompileStatus(shaderId);
		if(params == 1) return true;
		GLContext::printShaderCompileLog(shaderId);
		return false;
	}

};

#endif /* SHADER_H_ */
