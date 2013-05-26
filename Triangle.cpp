#include "Triangle.h"

Triangle::Triangle(){
	A = Vector(1, 0, 0);
	B = Vector(0, 1, 0);
	C = Vector(0, 0, 1);
	color = Color(0.5, 0.5, 0.5, 0.0);
}

Triangle::Triangle(Vector pointA, Vector pointB, Vector pointC, Color colorValue){
	A = pointA;
	B = pointB;
	C = pointC;
	color = colorValue; 
}

Vector Triangle::getTriangleNormal(){
	Vector CA (C.getVectorX() - A.getVectorX(), C.getVectorY() - A.getVectorY(), C.getVectorZ() - A.getVectorZ());
	Vector BA (B.getVectorX() - A.getVectorX(), B.getVectorY() - A.getVectorY(), B.getVectorZ() - A.getVectorZ());

	normal = CA.cross(BA).normalize();
	return normal;
}
double Triangle::getTriangleDistance(){
	normal = getTriangleNormal();
	distance = normal.dot(A);

	return distance; 
}

Color Triangle::getColor(){
	return color;
}

double Triangle::findIntersection(Ray ray){
	Vector rayDirection = ray.getRayDirection();
	Vector rayOrigin = ray.getRayOrigin();
	double a = rayDirection.dot(normal);
	
	normal = getTriangleNormal();
	distance = getTriangleDistance();
	if (a == 0){
		// ray is parallel to the Triangle
		return -1.0;
	}
	else{

		double b = normal.dot(ray.getRayOrigin().add(normal.mult(distance).negative()));
		double distanceToPlane = -1.0 * b / a;

		double Qx = rayDirection.mult(distanceToPlane).getVectorX() + rayOrigin.getVectorX(); 
		double Qy = rayDirection.mult(distanceToPlane).getVectorY() + rayOrigin.getVectorY(); 
		double Qz = rayDirection.mult(distanceToPlane).getVectorZ() + rayOrigin.getVectorZ(); 

		Vector Q (Qx, Qy, Qz);

		//[CA x QA] * n >= 0
		Vector CA (C.getVectorX() - A.getVectorX(), C.getVectorY() - A.getVectorY(), C.getVectorZ() - A.getVectorZ());
		Vector QA (Q.getVectorX() - A.getVectorX(), Q.getVectorY() - A.getVectorY(), Q.getVectorZ() - A.getVectorZ());
		double test1 = (CA.cross(QA)).dot(normal);
		//[BC x QC] * n >= 0
		Vector BC (B.getVectorX() - C.getVectorX(), B.getVectorY() - C.getVectorY(), B.getVectorZ() - C.getVectorZ());
		Vector QC (Q.getVectorX() - C.getVectorX(), Q.getVectorY() - C.getVectorY(), Q.getVectorZ() - C.getVectorZ());
		double test2 = (BC.cross(QC)).dot(normal);
		//[AB x QB] * n >= 0
		Vector AB (A.getVectorX() - B.getVectorX(), A.getVectorY() - B.getVectorY(), A.getVectorZ() - B.getVectorZ());
		Vector QB (Q.getVectorX() - B.getVectorX(), Q.getVectorY() - B.getVectorY(), Q.getVectorZ() - B.getVectorZ());
		double test3 = (AB.cross(QB)).dot(normal);
		
		if ((test1 >= 0) && (test2 >= 0) && (test3 >= 0)){
			//inside Triangle
			return -1.0 * b / a;
		}
		else{
			//outside Triangle
			return -1;
		}
		
	}
}

Vector Triangle::getNormalAt(Vector point){
	normal = getTriangleNormal();
	return normal;
}
