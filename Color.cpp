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

double Color::brightness(){
	return (red + blue + green)/3;
}

Color Color::scalar(double scalar){
	return Color(red * scalar, green * scalar, blue * scalar, special);
}

Color Color::add(Color color){
	return Color(red + color.getColorRed(), green + color.getColorGreen(), blue + color.getColorBlue(), special);
}

Color Color::mult(Color color){
	return Color(red + color.getColorRed(), green * color.getColorGreen(), blue * color.getColorBlue(), special);	
}

Color Color::average(Color color){
	return Color((red + color.getColorRed())/2, (green + color.getColorGreen())/2, (blue + color.getColorBlue())/2, special);
}