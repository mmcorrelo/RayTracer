#ifndef __LIGHT_H_
#define __LIGHT_H_

#include "Vector.h"
#include "Color.h"
#include "Source.h"

class Light: public Source{
	Vector position;
	Color color;
public:
	Light();
	Light(Vector, Color);

	virtual Vector getLightPosition(){ return position;}
	virtual Color getLightColor(){ return color;}

};
#endif

