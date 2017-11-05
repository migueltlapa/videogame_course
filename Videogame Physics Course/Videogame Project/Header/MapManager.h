/*
 * MapManager.h
 *
 *  Created on: May 10, 2014
 *      Author: joelmart
 */

#ifndef MAPMANAGER_H_
#define MAPMANAGER_H_

#include <iostream>
#include <sstream>
#include "Texture.h"
#include "Program.h"
#include "WallVA.h"
#include "Wall.h"
#include "Matrix4.h"
#include "Floor.h"
#include "FloorVA.h"

using namespace std;

class MapManager{
private:
	char* mapArray;
	Texture* floorTexture;
	FloorVA* floorVA;
	Floor*   floor;

	Texture* wallTexture;
	WallVA*  wallVA;
	Wall*    vertical;

	Program *mapProgram;

	int max_columns;
	int max_row;

public:
	MapManager(const char* filename){
		floorTexture = NULL;
		floorVA = NULL;
		floor = NULL;
		wallTexture = NULL;
		wallVA = NULL;
		vertical = NULL;
		mapProgram = NULL;

		ifstream sceneFile(filename);
	   	string token = "";
	   	// Read column length
	    sceneFile >> token;  // Read c
	    sceneFile >> token;  // Read #
		{
	    	stringstream ss(token);
	    	ss >> max_columns;
		}

		// Read row length
	    sceneFile >> token;  // Read r
	    sceneFile >> token;  // Read #
		{
	    	stringstream ss(token);
	    	ss >> max_row;
		}

		// create memory to store the scene
		mapArray = new char[(max_columns * max_row)];

		// read file char by char
		stringstream buffer;
		buffer << sceneFile.rdbuf();
		string contents(buffer.str());
		int index = 0;
		for (unsigned int i = 0; i < contents.length(); i++)
		{
			if(contents[i] == '#') break;
			if(contents[i] == '\n') {
				continue;
			}
			else{
				mapArray[index] = contents[i];
				index++;
			}
		}

	}

	virtual ~MapManager(){
		delete[] mapArray;
		delete vertical;
		delete floorTexture;
		delete floorVA;
		delete floor;
		delete wallTexture;
		delete wallVA;
	}

	void initMap(){
		wallTexture = new Texture();
		wallTexture->load("textures/la.bmp");
		floorTexture = new Texture();
		floorTexture->load("textures/Metal.bmp");

		mapProgram = new Program();
		mapProgram->setVertexShader("shaders/wall.vsh");
		mapProgram->setFragmentShader("shaders/wall.fsh");
		mapProgram->build(true, false, true);

		wallVA = new WallVA("Vertical");
		wallVA->bindPositions(mapProgram->getAttributeLocation("position"));
		wallVA->bindTexcoords(mapProgram->getAttributeLocation("texCoord"));

		floorVA = new FloorVA(max_columns*LARGE_SIDE, -1, max_row*LARGE_SIDE);
		floorVA->bindPositions(mapProgram->getAttributeLocation("position"));
		floorVA->bindTexcoords(mapProgram->getAttributeLocation("texCoord"));

		vertical = new Wall();
		vertical->setProgram(mapProgram);
		vertical->setTexture(wallTexture);
		vertical->setVertexArray(wallVA);

		floor = new Floor();
		floor->setProgram(mapProgram);
		floor->setTexture(floorTexture);
		floor->setVertexArray(floorVA);
	}

	void draw(Matrix4 modelMatrix){
		modelMatrix.setIdentity();
		mapProgram->setMatrix("model", modelMatrix);
		floor->draw();

		int i = 0;
		float r_start = -( ( ( max_row/2.0 ) *  LARGE_SIDE ) - (LARGE_SIDE / 2.0) );
		float r_end = ( ( max_row/2.0 ) * LARGE_SIDE ) - (LARGE_SIDE / 2.0);
		float r_incre = LARGE_SIDE;
		float c_start = -( ( (max_columns/2.0) * LARGE_SIDE) - (LARGE_SIDE / 2.0) );
		float c_end = ( (max_columns/2.0) * LARGE_SIDE) - (LARGE_SIDE / 2.0);
		float c_incre = LARGE_SIDE;

		for (float r = r_start; r <= r_end; r += r_incre) {
			for (float c = c_start; c <= c_end; c += c_incre, i++) {
				modelMatrix.setIdentity();
				float x = c;
				float y = 0;
				float z = r-5;
				if(mapArray[i] == '-'){
					Transforms::translate(modelMatrix, x, y, z);
					mapProgram->setMatrix("model", modelMatrix);
					vertical->draw();
				}
				if(mapArray[i] == '|'){
					Transforms::translate(modelMatrix, x, y, z);
					mapProgram->setMatrix("model", modelMatrix);
					vertical->draw();
				}
			}
		}
	}

	Program* getProgram(){
		return mapProgram;
	}

};



#endif /* MAPMANAGER_H_ */
