#ifndef __CAMARA_H_
#define __CAMARA_H_

#include "Vector.h"
class Camera{
	Vector campos, camdir, camright, camdown;
public:
	Camera();
	Camera(Vector, Vector, Vector, Vector);

	Vector getCameraPosition() {return campos;}
	Vector getCameraDirection() {return camdir;}
	Vector getCameraRight() {return camright;}
	Vector getCameradown() {return camdown;}
};
#endif

