/*
 * Main.cpp
 *
 *  Created on: 08/05/2014
 *      Author: HugoIván
 */

#include "GLContext.h"
#include "Window.h"
#include "MazeGame.h"

MazeGame game;

void keyReleasedListener(int key, int x, int y){
	game.motionDetected(0);
}

void keyPressedListener(unsigned char key, int x, int y){
	switch(key){
	case 's':
		game.motionDetected(1);
		break;
	case 'w':
		game.motionDetected(2);
		break;
	case 'd':
		game.motionDetected(3);
		break;
	case 'a':
		game.motionDetected(4);
		break;
	default:
		break;
	}
}

void reshape(int width, int height) {
//	GLContext::viewPort(width, height); // moved to game.reshape method.
	game.reshape(width, height);
}

void display() {
	GLContext::clear();
	game.draw();
	GLContext::swap();
}

int main(int argc, char **argv) {
	Window::init(argc, argv);
    Window::create("Face the maze", 800, 600, 10);
    GLContext::init();
    game.init();
	GLContext::Enable(GL_DEPTH_TEST); // Not sure it this line should be here "?"
    GLContext::onDisplay(display);
    GLContext::onReshape(reshape);
    Window::addKeyListener(keyPressedListener);
    Window::show();
	return 0;
}
