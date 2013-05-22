#ifndef __OBJECT_H_
#define __OBJECT_H_

#include "Vector.h"
#include "Color.h"
#include "Ray.h"

class Object{
public:

	Object();

	Color getObjectColor(){ return Color(0.0, 0.0, 0.0, 0.0);}
	double findIntersection(Ray ray){ return 0; }

};
#endif
