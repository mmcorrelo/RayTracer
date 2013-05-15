#ifndef __VECTOR_H_
#define __VECTOR_H_

//#include <math>

class Vector{
	double x,y,z;
public:
	Vector();
	Vector(double, double, double);

	double getVectorX(){ return x; }
	double getVectorY(){ return y; }
	double getVectorZ(){ return z; }
};
#endif

