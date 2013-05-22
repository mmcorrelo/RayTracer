#include "Ray.h"

Ray::Ray(){
	origin = Vector(0,0,0);
	direction = Vector(1,0,0);
}

Ray::Ray(Vector o, Vector d){
	this->origin = o;
	this->direction = d;
}