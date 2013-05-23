#ifndef __SPHERE_H_
#define __SPHERE_H_

#include "Vector.h"
#include "Object.h"
#include "Color.h"
#include "math.h"

class Sphere: public Object{
	Vector center;
	double radius; 
	Color color;
public:

	Sphere();
	Sphere(Vector, double, Color);

	Vector getSphereCenter(){ return center; }
	double getSphereRadius(){ return radius; }
	virtual Color getColor();

	double findIntersection(Ray);
	virtual Vector getNormalAt(Vector);
};
#endif

