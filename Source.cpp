#include "Source.h"

Source::Source(){}

Vector Source::getLightPosition(){
	return Vector(0.0, 0.0, 0.0);
}

Color Source::getColor(){
	return Color(1.0, 1.0, 1.0, 0);
}