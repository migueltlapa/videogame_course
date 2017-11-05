/*
 * Matrix4.cpp
 *
 *  Created on: Feb 10, 2014
 *      Author: mtlapa
 */

#include "Matrix4.h"
#include <iostream>
#include <cmath>

namespace mat4 {
// Constructor
Matrix4::Matrix4() {
	// TODO Auto-generated constructor stub
	setIdentity();

}

Matrix4::~Matrix4() {
	using namespace std;
		cout << "se libera el espacio" << endl;
		// TODO Auto-generated destructor stub
	}

void Matrix4::setIdentity()
		{

	int position =0;
	int control = 5;

	for (position= 0; position < 16; position++){


		if (control ==5){
			values[position] = 1;
			control = 0;

			}
		else{
			values[position] = 0;
			}
		control++;
			}
		}
void Matrix4::set(int col, int row, float value){

		if(col >=4 || row >=4){
			return;
			}
		else {
			values[row*4 +col] =value;

			}

		}

float Matrix4::get(int col, int row)const
		{
		float result;
		if(col >=4 || row >=4){
			return 0;
			}

		else{
			result = values[row*4 +col];
			return result;
			}
		}

	Matrix4 Matrix4::operator*(const Matrix4 &c){
		Matrix4 m_temp;
		float result;
		int row=0;

	    for(int c_pos =0; c_pos <15; c_pos++) {

	    	for (int this_pos=0; this_pos <4; this_pos++) {
	    	result =(c.values[c_pos]*this->values[this_pos]) +
	    			(c.values[c_pos + 1 ]*this->values[this_pos  + 4]) +
	    			(c.values[c_pos + 2 ]*this->values[this_pos  + 8])+
	    			(c.values[c_pos + 3 ]*this->values[this_pos  + 12]);
	    	m_temp.set(this_pos,row,result);
	    	}
	    	row ++;
	    	c_pos = c_pos+3;
	    }
		return m_temp ;
	}



	ostream& operator<<(ostream &o, const Matrix4 &c) {
		for (int i = 0; i <16; i += 4)	{
			o<<c.values[i] << c.values[i + 1]<< c.values[i + 2]<< c.values[i + 3] << endl;


			}
		}

} /* namespace mat4 */
