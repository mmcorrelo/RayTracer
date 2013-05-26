#ifndef __UTIL_H_
#define __UTIL_H_

#include "Structs.h"
#include "Color.h"
#include "Vector.h"
#include "Object.h"
#include "Source.h"
#include <fstream>
#include <sstream>
#include <math.h>
#include <iostream>

#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <ctime>

#include <stdlib.h>
#include <stdio.h>

class Util{
public:
	static Color getColorAt(Vector intersectionPosition, Vector intersectionRayDirection, std::vector<Object*> sceneObjects, int indexOfWinningObject, std::vector<Source*> lightSources, double accuracy, double ambientLight);
	static int winningObjectIndex(std::vector<double> object_intersections);
	static void savebmp(const char *filename, int w, int h, int dpi, RGBType *data);
};

#endif

