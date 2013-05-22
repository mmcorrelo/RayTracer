#include "Sphere.h"

Sphere::Sphere(){
	center = Vector(0,0,0);
	radius = 1.0;
	color = Color(0.5, 0.5, 0.5, 0.0);
}

Sphere::Sphere(Vector centerValue, double radiusValue, Color colorValue){
	center = centerValue;
	radius = radiusValue;
	color = colorValue;
}