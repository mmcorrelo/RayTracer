#include "Vector.h"


Vector::Vector(){
	x = 0;
	y = 0;
	z = 0;
}


Vector::Vector(double xx, double yy, double zz){
	x = xx;
	y = yy;
	z = zz;
}

double Vector::magnitude(){
	return sqrt((x*x)+(y*y)+(z*z));
}

Vector Vector::normalize(){
	double mag = magnitude();
	return Vector(x/mag,y/mag,z/mag);
}

Vector Vector::negative(){
	return Vector(-x,-y,-z);
}

double Vector::dot(Vector v){
	return x * v.getVectorX() + y * v.getVectorY() + z * v.getVectorZ(); 
}

Vector Vector::cross(Vector v){
	return Vector(y * v.getVectorZ() - z * v.getVectorY(), z * v.getVectorX() - x * v.getVectorZ(), x * v.getVectorY() - y * v.getVectorX());
}

Vector Vector::add(Vector v){
	return Vector(x + v.getVectorX(), y + v.getVectorY(), z + v.getVectorZ());
}

Vector Vector::mult(double scalar){
	return Vector(x * scalar, y * scalar, z * scalar);
}
