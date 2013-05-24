#ifndef __PLANE_H_
#define __PLANE_H_

#include "Vector.h"
#include "Object.h"
#include "Color.h"
#include "math.h"

class Plane: public Object{
	Vector normal;
	double distance; 
	Color color;
public:

	Plane();
	Plane(Vector, double, Color);

	Vector getSphereNormal(){ return normal; }
	double getSphereDistance(){ return distance; }
	virtual Color getColor();

	virtual Vector getNormalAt(Vector);
	virtual double findIntersection(Ray);
};
#endif

