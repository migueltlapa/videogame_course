/*
 * Game.h
 *
 *  Created on: 09/05/2014
 *      Author: Iván
 */

#ifndef MAZE_GAME_H_
#define MAZE_GAME_H_

#include "GLContext.h"
#include "Program.h"
#include "Transforms.h"
#include "MapManager.h"

class MazeGame {

private:
	MapManager* theMap;
	Program* program;
	Matrix4 modelMatrix;
	Matrix4 projMatrix;
	Matrix4 viewMatrix;
	int   motionType;
	float observerX, observerZ;

	void initObjects() {
		motionType = 0;
		observerX = 0;
		observerZ = 0;
	}

	void initMap(){
		theMap = new MapManager("map1.txt");
		theMap->initMap();
	}

public:
	void init() {
	    GLContext::setBackground(0.7, 0.9, 1.0);
	    initObjects();
	    initMap();
	    program = theMap->getProgram();
	}

	void draw() {
		viewMatrix.setIdentity();
		Transforms::translate(viewMatrix, observerX, 0, observerZ);
		program->setMatrix("view", viewMatrix);
		theMap->draw(modelMatrix);
	}

	void reshape(int w, int h) {
		if(h == 0) h = 1;
		GLContext::viewPort(w, h);
		float aspect = (float) w / h;
		projMatrix.setPerspective(53, aspect, -1, -800);
		program->setMatrix("projection", projMatrix);
	}

	void motionDetected(int motion){
		switch(motion){
		case 1: observerZ -= 0.1; break;
		case 2: observerZ += 0.1; break;
		case 3: observerX -= 0.1; break;
		case 4: observerX += 0.1; break;
		}
	}

	virtual ~MazeGame() {
		delete theMap;
	}

};

#endif /* MAZE_GAME_H_ */
