#ifndef __RAY_H_
#define __RAY_H_

#include "Vector.h"
class Ray{
	Vector origin, direction;
public:
	Ray();
	Ray(Vector, Vector);

	Vector getRayOrigin() {return origin;}
	Vector getRayDirection() {return direction;}
};
#endif

