#ifndef VECTOR
#define VECTOR

#include "Point.h"

class Vector : public Point
{
	public:
		float norma;
		Vector();
		Vector(int, int);
		Vector(float, float);
};

Vector :: Vector():Point(){}
Vector :: Vector(int X, int Y) : Point(X, Y){
	this->norma = sqrt(pow(this->X, 2) * pow(this->Y, 2));
}

Vector :: Vector(float X, float Y) : Point(X, Y){
	this->norma = sqrt(pow(this->X, 2) * pow(this->Y, 2));
}

#endif