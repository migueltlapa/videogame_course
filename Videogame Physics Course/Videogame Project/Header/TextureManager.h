/*
 * TextureManager.h
 *
 *  Created on: 09/05/2014
 *      Author: Iván
 */

#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include <GL/glew.h>
#include <fstream>
#include <iostream>

class TextureManager {

private:
	static bool loadBMP(const char* filename, unsigned char **pdata, unsigned int &width, unsigned int &height) {
		unsigned char header[54];
	    FILE * file = fopen(filename, "rb");
	    fread(header, 1, 54, file);
	    if(header[0] != 'B' || header[1] != 'M' ) {
	    	printf("Not a correct BMP file\n");
	    	return false;
	    }
	    unsigned char *bytePointerW      = &(header[0x12]);
	    unsigned int  *fourBytesPointerW = (unsigned int*) bytePointerW;
	    width = *fourBytesPointerW;
	    unsigned char *bytePointerH      = &(header[0x16]);
	    unsigned int  *fourBytesPointerH = (unsigned int*) bytePointerH;
	    height = *fourBytesPointerH;
	    *pdata = new unsigned char[width * height * 3];
	    fread(*pdata, 1, width * height * 3, file);
	    fclose(file);
	    return true;
	}

public:

	static GLuint load(string filename, unsigned int wrapS, unsigned int wrapT, unsigned int magFilter, unsigned int minFilter) {
		unsigned char *data = NULL;
		unsigned int width = 0, height = 0;
		if(!loadBMP(filename.c_str(), &data, width, height)) return 0;
		GLuint texId;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		delete[] data;
		return texId;
	}
};

#endif /* TEXTUREMANAGER_H_ */
