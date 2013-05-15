#include "Camera.h"


Camera::Camera(){
	this->campos = Vector(0,0,0);
	this->camdir = Vector(0,0,1);
	this->camright = Vector(0,0,0);
	this->camdown = Vector(0,0,0);
}


Camera::Camera(Vector pos, Vector dir, Vector right, Vector down){
	this->campos = pos;
	this->camdir = dir;
	this->camright = right;
	this->camdown = down;
}

