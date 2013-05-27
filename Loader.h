#ifndef __LOADER_H_
#define __LOADER_H_

#include "Structs.h"
#include "Color.h"
#include "Vector.h"
#include "Object.h"
#include "Source.h"
#include "Light.h"
#include "libs/pugixml.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

class Loader{
	int width;
	int height;
	int dpi;
	int depth;
	double threshold;
	string author;
	string modelName;
	string date;
	Vector camPos;
	Vector lookAt;
	vector<Source*> lightSources;
public:
	Loader(const char*,vector<Source*>&);
	int getWidth();
	int getHeight();
	int getDpi();
	int getDepth();
	double getThreshold();

	string getModelName();
	string getAuthor();
	string getDate();

	Vector getCameraPosition();
	Vector getLookAt();
	//void getLightSources();
};

#endif
