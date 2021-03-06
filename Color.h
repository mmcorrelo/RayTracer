#ifndef __COLOR_H_
#define __COLOR_H_

class Color{
	double red, green, blue, special;
public:

	Color();
	Color(double, double, double, double);

	double getColorRed() { return red; }
	double getColorGreen() { return green; }
	double getColorBlue() {return blue;}
	double getColorSpecial() {return special;}

	void setColorRed(double value){ red = value; }
	void setColorGreen(double value){ green = value; }
	void setColorBlue(double value){ blue = value; }
	void setColorSpecial(double value){ special = value; }

	double brightness();
	Color scalar(double);
	Color add(Color);
	Color mult(Color);
	Color average(Color);
	Color clip();
};
#endif

