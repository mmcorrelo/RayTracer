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

Color Plane::getColor(){
	return color;
}

Vector Plane::getNormalAt(Vector point){
	return normal;
}

double Plane::findIntersection(Ray ray){
	Vector rayDirection = ray.getRayDirection();
	double a = rayDirection.dot(normal);

	if (a == 0){
		// ray is parallel to the plane
		return -1.0;
	}
	else{
		double b = normal.dot(ray.getRayOrigin().add(normal.mult(distance).negative()));
		return -1.0 * b / a;
	}

}