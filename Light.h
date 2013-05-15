#ifndef __LIGHT_H_
#define __LIGHT_H_

#include "Vector.h"
#include "Color.h"
class Light{
	Vector position;
	Color color;
public:
	Light();
	Light(Vector, Color);

	Vector getLightPosition(){ return position;}
	Color getLightColor(){ return color;}

};
#endif

