#include "Plane.h"

Plane::Plane(){
	normal = Vector(1, 0, 0);
	distance = 1.0;
	color = Color(0.5, 0.5, 0.5, 0);
}
Plane::Plane(Vector normalValue, double distanceValue, Color colorValue){
	normal = normalValue;
	distance = distanceValue;
	color = colorValue;
}