/*
 * Window.h
 *
 *  Created on: 08/05/2014
 *      Author: HugoIván
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <GL/freeglut.h>
#include <string>
#include <list>

using namespace std;

typedef void (*KeyFunc) (unsigned char, int, int);
typedef void (*SpecialKeyFunc) (int, int, int);

class Window {
private:
	static int delay;
	static list<KeyFunc> keyFuncList;
	static list<SpecialKeyFunc> specialKeyFuncList;

	static void callKeyFuncList(unsigned char key, int x, int y) {
		list<KeyFunc>::iterator ite;
		for(ite = keyFuncList.begin(); ite != keyFuncList.end(); ite ++) {
			KeyFunc keyFunc = *ite;
			keyFunc(key, x, y);
		}
	}

	static void callSpecialKeyFuncList(int code, int x, int y) {
		list<SpecialKeyFunc>::iterator ite;
		for(ite = specialKeyFuncList.begin(); ite != specialKeyFuncList.end(); ite ++) {
			SpecialKeyFunc specialKeyFunc = *ite;
			specialKeyFunc(code, x, y);
		}
	}

public:
	static void escFunc(unsigned char key, int x, int y) {
		if (key == 27) exit(0);
	}

	static void altF4Func(int key, int x, int y) {
		if(key == GLUT_KEY_F4 && glutGetModifiers() == GLUT_ACTIVE_ALT) exit(0);
	}

	static void timerFunc(int timerId) {
	    glutTimerFunc(delay, timerFunc, timerId);
	    glutPostRedisplay();
	}

	static void init(int argc, char **argv) {
		glutInit(&argc, argv);
	}

	static void create(string _title, int _width, int _height, int _delay) {
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		int swidth  = glutGet(GLUT_SCREEN_WIDTH);
		int sheight = glutGet(GLUT_SCREEN_HEIGHT);
	    glutInitWindowSize(_width, _height);
	    glutInitWindowPosition((swidth - _width) / 2, (sheight - _height) / 2);
		glutCreateWindow(_title.c_str());
		keyFuncList.push_back(escFunc);
		specialKeyFuncList.push_back(altF4Func);
		glutKeyboardFunc(callKeyFuncList);
		glutSpecialFunc(callSpecialKeyFuncList);
		glutSpecialUpFunc(callSpecialKeyUpFunc);
		if(_delay > 0) {
			Window::delay = _delay;
			glutTimerFunc(delay, timerFunc, 0);
		}
	}

	static void addKeyListener(KeyFunc kf) {
		keyFuncList.push_back(kf);
	}

	static void addSpecialKeyListener(SpecialKeyFunc skf) {
		specialKeyFuncList.push_back(skf);
	}

	static void callSpecialKeyUpFunc(int key, int x, int y){

	}

	static void show() {
		glutMainLoop();
	}
};

int Window::delay = 10;
list<KeyFunc> Window::keyFuncList;
list<SpecialKeyFunc> Window::specialKeyFuncList;

#endif /* WINDOW_H_ */
