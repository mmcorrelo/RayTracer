#ifndef __SPHERE_H_
#define __SPHERE_H_

#include "Vector.h"
#include "Color.h"
#include "math.h"

class Sphere{
	Vector center;
	double radius; 
	Color color;
public:

	Sphere();
	Sphere(Vector, double, Color);

	Vector getSphereCenter(){ return center; }
	double getSphereRadius(){ return radius; }
	Color getSphereColor(){ return color;}
};
#endif

