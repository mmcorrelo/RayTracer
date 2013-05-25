#include "Color.h"

Color::Color(){
	red = 0.0;
	green = 0.0;
	blue = 0.0;
}

Color::Color(double r, double g, double b, double s){
	red = r;
	blue = b;
	green = g;
	special = s;
}

double Color::brightness(){
	return (red + green + blue)/3;
}

Color Color::scalar(double scalar){
	return Color (red * scalar, green * scalar, blue * scalar, special);
}

Color Color::add(Color color){
	return Color(red + color.getColorRed(), green + color.getColorGreen(), blue + color.getColorBlue(), special);
}

Color Color::mult(Color color){
	return Color (red * color.getColorRed(), green * color.getColorGreen(), blue * color.getColorBlue(), special);
}

Color Color::average(Color color){
	return Color((red + color.getColorRed())/2, (green + color.getColorGreen())/2, (blue + color.getColorBlue())/2, special);
}

Color Color::clip(){
	double allRight = red + green + blue;
	double excessLight = allRight - 3;
	if (excessLight > 0){
		red = red + excessLight * (red/allRight);
		green = green + excessLight * (green/allRight);
		blue = blue + excessLight * (blue/allRight);
	}
	if (red > 1) {red = 1;}
	if (green > 1) {green = 1;}
	if (blue > 1) {blue = 1;}

	if (red < 0) {red = 0;}
	if (green < 0) {green = 0;}
	if (blue < 0) {blue = 0;}

	return Color(red, green, blue, special);
}
