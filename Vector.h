#ifndef __VECTOR_H_
#define __VECTOR_H_

#include <math.h>

class Vector{
	double x,y,z;

	public:
		Vector();
		Vector(double, double, double);

		double getVectorX(){ return x; }
		double getVectorY(){ return y; }
		double getVectorZ(){ return z; }
		double magnitude();
		Vector normalize();
		Vector negative();
		double dot(Vector);
		Vector cross(Vector);
		Vector add(Vector);
		Vector mult(double);
};
#endif

