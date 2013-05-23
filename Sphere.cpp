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

Vector Sphere::getNormalAt(Vector point){
	//normal always ponts away from the center of a sphere
	Vector normalVector = point.add(center.negative()).normalize();
	return normalVector;
}

double Sphere::findIntersection(Ray ray){
	Vector rayOrigin = ray.getRayOrigin();
	double rayOriginX = rayOrigin.getVectorX();
	double rayOriginY = rayOrigin.getVectorY();
	double rayOriginZ = rayOrigin.getVectorZ();

	Vector rayDirection = ray.getRayDirection();
	double rayDirectionX = rayDirection.getVectorX();
	double rayDirectionY = rayDirection.getVectorY();
	double rayDirectionZ = rayDirection.getVectorZ();

	Vector sphereCenter = center;
	double sphereCenterX = sphereCenter.getVectorX();
	double sphereCenterY = sphereCenter.getVectorY();
	double sphereCenterZ = sphereCenter.getVectorZ();

	double a = 1; //normalized
	
	double b = (2 * (rayOriginX - sphereCenterX) * rayDirectionX) 
	+ (2 * (rayOriginY - sphereCenterY) * rayDirectionY) 
	+ (2 * (rayOriginZ - sphereCenterZ) * rayDirectionZ);
	
	double c = pow(rayOriginX - sphereCenterX, 2) 
	+ pow(rayOriginY - sphereCenterY, 2) 
	+ pow(rayOriginZ - sphereCenterZ, 2)
	- (radius * radius); 

	double discriminant = b*b - 4*a*c;

	if (discriminant > 0.0){
		//the ray instercepts the sphere in two sides

		//first root
		double root1 = ((-1*b - sqrt(discriminant)) / 2) - 0.000001;

		if (root1 > 0.0){
			//the first root is the smallest positive root
			return root1;
		}
		else{
			//the second root is the smallest positive root
			double root2 = ((sqrt(discriminant) - b ) / 2) - 0.000001;
			return root2;
		}

	}
	else{
		// the ray missed the sphere
		return -1;
	}

}