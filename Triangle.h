#ifndef __TRIANGLE_H_
#define __TRIANGLE_H_

#include "Vector.h"
#include "Object.h"
#include "Color.h"
#include "math.h"

class Triangle: public Object{
	Vector A,B,C;
	Vector normal;
	double distance; 
	Color color;
public:

	Triangle();
	Triangle(Vector, Vector, Vector, Color);

	Vector getTriangleNormal();
	double getTriangleDistance();
	virtual Color getColor();

	virtual Vector getNormalAt(Vector);
	virtual double findIntersection(Ray);
};
#endif

