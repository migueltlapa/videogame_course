/*
 * Texture.h
 *
 *  Created on: 10/05/2014
 *      Author: Iván
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "TextureManager.h"

using namespace std;

class Texture {
private:
	unsigned int textureId;
	unsigned int wrapS;
	unsigned int wrapT;
	unsigned int magFilter;
	unsigned int minFilter;

public:
	Texture() {
		textureId = 0;
		wrapS = GLContext::REPEAT_WRAP;
		wrapT = GLContext::REPEAT_WRAP;
		magFilter = GLContext::LINEAR_FILTER;
		minFilter = GLContext::LINEAR_FILTER;
	}

	void load(string filename) {
		textureId = TextureManager::load(filename, wrapS, wrapT, magFilter, minFilter);
	}

	void bind() {
		GLContext::bindTexture(this->textureId);
	}

	void setMagFilterLinear() {
		magFilter = GLContext::LINEAR_FILTER;
	}

	void setMinFilterLinear() {
		minFilter = GLContext::LINEAR_FILTER;
	}

	void setMagFilterNearest() {
		magFilter = GLContext::NEAREST_FILTER;
	}

	void setMinFilterNearest() {
		minFilter = GLContext::NEAREST_FILTER;
	}

	void setWrapSClamp() {
		wrapS = GLContext::CLAMP_WRAP;
	}

	void setWrapTClamp() {
		wrapT = GLContext::CLAMP_WRAP;
	}

	void setWrapSRepeat() {
		wrapS = GLContext::REPEAT_WRAP;
	}

	void setWrapTRepeat() {
		wrapT = GLContext::REPEAT_WRAP;
	}

	unsigned int getId() {
		return this->textureId;
	}

	virtual ~Texture() {
		GLContext::deleteTexture(this->textureId);
	}
};

#endif /* TEXTURE_H_ */
