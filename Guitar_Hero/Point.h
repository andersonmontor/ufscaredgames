#ifndef POINT
#define POINT

#include <math.h>
#include "Vector.h"


class Point{
	public:
		float X;
		float Y;
		Point();
		Point(int, int);
		Point(float, float);
		Point operator+ (Vector);
		Point operator- (Vector);
		Vector operator- (Point);
};


Point :: Point(){
	X = 0;
	Y = 0;
}
Point :: Point(int X, int Y){
	this->X = (float)X;
	this->Y = (float)Y;
}
Point :: Point(float, float){
	this->X = X;
	this->Y = Y;
}
Point Point::operator+ (Vector V){
	return Point(this->X + V.X, this->Y + V.Y);
}
Point Point::operator- (Vector V){
	return Point(this->X - V.X, this->Y - V.Y);
}
Vector Point::operator-(Point P){ // atenção: ao subtria P1 de P2 obtêm-se um vetor de P2 para P1 :3
	return Vector(this->X - P.X, this->Y - V.Y);
}

#endif