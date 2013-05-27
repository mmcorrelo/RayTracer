#include "Source.h"

Source::Source(){}

Vector Source::getLightPosition(){
	return Vector(0.0, 0.0, 0.0);
}

Vector Source::getLightDirection(){
	return Vector(0.0, 0.0, 0.0);
}

Color Source::getLightColor(){
	return Color(1.0, 1.0, 1.0, 0);
}
