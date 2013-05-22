#include "Color.h"

Color::Color(){
	red = 0.5;
	green = 0.5;
	blue = 0.5;
}

Color::Color(double r, double g, double b, double s){
	red = r;
	blue = b;
	green = g;
	special = s;
}