#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Vector.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Object.h"
#include "Sphere.h"
#include "Plane.h"

using namespace std;

struct RGBType{
	double r;
	double g;
	double b;
};

void savebmp(const char *filename, int w, int h, int dpi, RGBType *data){
	FILE *f;
	int k = w*h;
	int s = 4*k;
	int filesize = 54 + s;
	double factor = 39.375;
	int m = static_cast<int> (factor);

	int ppm = dpi*m;
	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};
 
	bmpfileheader[ 2] = (unsigned char) (filesize);
	bmpfileheader[ 3] = (unsigned char) (filesize >> 8);
	bmpfileheader[ 4] = (unsigned char) (filesize >> 16);
	bmpfileheader[ 5] = (unsigned char) (filesize >> 24);

	bmpinfoheader[ 4] = (unsigned char) (w);
	bmpinfoheader[ 5] = (unsigned char) (w >> 8);
	bmpinfoheader[ 6] = (unsigned char) (w >> 16);
	bmpinfoheader[ 7] = (unsigned char) (w >> 24);

	bmpinfoheader[ 8] = (unsigned char) (h);
	bmpinfoheader[ 9] = (unsigned char) (h >> 8);
	bmpinfoheader[10] = (unsigned char) (h >> 16);
	bmpinfoheader[11] = (unsigned char) (h >> 24);

	bmpinfoheader[21] = (unsigned char) (s);
	bmpinfoheader[22] = (unsigned char) (s >> 8);
	bmpinfoheader[23] = (unsigned char) (s >> 16);
	bmpinfoheader[24] = (unsigned char) (s >> 24);

	bmpinfoheader[25] = (unsigned char) (ppm);
	bmpinfoheader[26] = (unsigned char) (ppm >> 8);
	bmpinfoheader[27] = (unsigned char) (ppm >> 16);
	bmpinfoheader[28] = (unsigned char) (ppm >> 24);

	bmpinfoheader[29] = (unsigned char) (ppm);
	bmpinfoheader[30] = (unsigned char) (ppm >> 8);
	bmpinfoheader[31] = (unsigned char) (ppm >> 16);
	bmpinfoheader[32] = (unsigned char) (ppm >> 24);

	f = fopen(filename, "wb");
	fwrite(bmpfileheader, 1,14,f);
	fwrite(bmpinfoheader, 1,40,f);

	for (int i = 0; i < k; i++){
		RGBType rbg = data[i];
		double red = (data[i].r)*255;
		double green = (data[i].g)*255;
		double blue = (data[i].b)*255;

		unsigned char color[3] = {(int) floor(blue), (int) floor(green), (int) floor(red)};

		fwrite(color,1,3,f);
	}
	fclose(f);
}

int winningObjectIndex(std::vector<double> object_intersections){
	// return the index of the winning intersection
	int indexOfMinimumValue;

	//prevent unnecessary calculations
	
}

int main(int argv, char *argc[]){

	cout << "rendering ..." << endl;
	int dpi = 72;
	int width = 640;
	int height = 480;
	int n = width * height;
	RGBType *pixels = new RGBType[n];
	int thisone;
	double xamnt, yamnt; 
	double aspectRatio = (double) width / (double) height;


	Vector O (0,0,0);
	Vector X (1,0,0);
	Vector Y (0,1,0);
	Vector Z (0,0,1);

	Vector camPos (3,1.5, -4);
	Vector lookAt (0,0,0);
	Vector diff_btw(camPos.getVectorX() - lookAt.getVectorX(), camPos.getVectorY() - lookAt.getVectorY(),camPos.getVectorZ() - lookAt.getVectorZ());
	Vector camDir = diff_btw.negative().normalize();
	Vector camRight = Y.cross(camDir).normalize();
	Vector camDown = camRight.cross(camDir);
	Camera scaneCam (camPos, camDir, camRight, camDown);

	Color whiteLight (1.0, 1.0, 1.0, 0.0);
	Color prettyGreen (0.5, 1.0, 0.5, 0.3);
	Color gray (0.5, 0.5, 0.5, 0.0);
	Color back (0.0, 0.0, 0.0, 0.0);
	Color marron (0.5, 0.2, 0.25, 0.0);

	Vector lightPosition (-7, 10, -10);
	Light sceneLight (lightPosition, whiteLight);

	//scene objects
	Sphere sphere(O, 1, prettyGreen);
	Plane plane(Y, -1.0, marron);

	std::vector<Object*> sceneObjects;
	sceneObjects.push_back(dynamic_cast<Object*>(&sphere));
	sceneObjects.push_back(dynamic_cast<Object*>(&plane));

	for (int x = 0; x < width; x++){
		for (int y = 0; y < height; y++){
			thisone = y * width + x;

			//start with anti-aliasing
			if (width > height){
				//the image is wider than it is all
				xamnt = ((x+0.5)/width)*aspectRatio - (((width - height)/(double) height)/2);
				yamnt =((height - y) + 0.5)/height;

			}
			else if (height > width){
				//the image is taller than it is wide
				xamnt = (x+ 0.5) / width;
				yamnt = (((height - y) + 0.5)/height)/ aspectRatio - (((height - width)/ (double) width)/2);
			}
			else{
				// the image is square
				xamnt = (x + 0.5) / width;
				yamnt = ((height - y) + 0.5)/height;
			}

			Vector cameraRayOrigin = scaneCam.getCameraPosition();
			Vector cameraRayDirection = camDir.add(camRight.mult(xamnt - 0.5).add(camDown.mult(yamnt- 0.5))).normalize();

			Ray cameraRay(cameraRayOrigin, cameraRayDirection);

			std::vector<double> intersections;
			//find instersection for each object scene
			for (int index = 0; sceneObjects.size(); index++){
				intersections.push_back(sceneObjects.at(index)->findIntersection(cameraRay));
			}
			int indexOfWinningObject = winningObjectIndex(intersections);


			pixels[thisone].r = 23;
			pixels[thisone].g = 222;
			pixels[thisone].b = 10;
		}
	}
	savebmp("scene.bmp", width, height, dpi, pixels);
	return 0;
}
