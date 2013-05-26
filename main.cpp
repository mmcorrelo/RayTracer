#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <ctime>

#include <stdlib.h>
#include <stdio.h>

//linux header
#include <unistd.h>
#include <sys/time.h>
//------

#include "Vector.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Source.h"
#include "Light.h"
#include "Object.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"

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
  
  // prevent unnessary calculations
  if (object_intersections.size() == 0) {
    // if there are no intersections
    return -1;
  }
  else if (object_intersections.size() == 1) {
    if (object_intersections.at(0) > 0) {
      // if that intersection is greater than zero then its our index of minimum value
      return 0;
    }
    else {
      // otherwise the only intersection value is negative
      return -1;
    }
  }
  else {
    // otherwise there is more than one intersection
    // first find the maximum value
    
    double max = 0;
    for (int i = 0; i < object_intersections.size(); i++) {
      if (max < object_intersections.at(i)) {
        max = object_intersections.at(i);
      }
    }
    
    // then starting from the maximum value find the minimum positive value
    if (max > 0) {
      // we only want positive intersections
      for (int index = 0; index < object_intersections.size(); index++) {
        if (object_intersections.at(index) > 0 && object_intersections.at(index) <= max) {
          max = object_intersections.at(index);
          indexOfMinimumValue = index;
        }
      }
      
      return indexOfMinimumValue;
    }
    else {
      // all the intersections were negative
      return -1;
    }
  }
}

Color getColorAt(Vector intersectionPosition, Vector intersectionRayDirection, std::vector<Object*> sceneObjects, int indexOfWinningObject, std::vector<Source*> lightSources, double accuracy, double ambientLight){
  Color winningObjectColor = sceneObjects.at(indexOfWinningObject)->getColor();
  Vector winningObjectNormal = sceneObjects.at(indexOfWinningObject)->getNormalAt(intersectionPosition);
  
  if (winningObjectColor.getColorSpecial() == 2){
    //checkered/tile floor pattern
    int square = (int) floor(intersectionPosition.getVectorX()) + (int) floor(intersectionPosition.getVectorZ());

    if ((square % 2) == 0){
      // black tile
      winningObjectColor.setColorRed(0.0);
      winningObjectColor.setColorGreen(0.0);
      winningObjectColor.setColorBlue(0.0);
    }
    else{
      //white tile
      winningObjectColor.setColorRed(1.0);
      winningObjectColor.setColorGreen(1.0);
      winningObjectColor.setColorBlue(1.0);
    }
  }

  Color finalColor = winningObjectColor.scalar(ambientLight);

  if (winningObjectColor.getColorSpecial() > 0 && winningObjectColor.getColorSpecial() <= 1){
    //reflection from objects with specular intensity
    double dot1 = winningObjectNormal.dot(intersectionRayDirection.negative());
    Vector scalar1 = winningObjectNormal.mult(dot1);
    Vector add1 = scalar1.add(intersectionRayDirection);
    Vector scalar2 = add1.mult(2);
    Vector add2 = intersectionRayDirection.negative().add(scalar2);
    Vector reflectionDirection = add2.normalize();

    Ray reflectionRay (intersectionPosition, reflectionDirection);

    //determine what the ray intersects first
    std::vector<double> reflectionIntersections;

    for (int reflectionIndex = 0; reflectionIndex < sceneObjects.size(); reflectionIndex++){
      reflectionIntersections.push_back(sceneObjects.at(reflectionIndex)->findIntersection(reflectionRay));
    }

    int indexOfWinningObjectWithReflection = winningObjectIndex(reflectionIntersections);

    if (indexOfWinningObjectWithReflection != -1){
      //reflection ray missed everthing else
      if (reflectionIntersections.at(indexOfWinningObjectWithReflection) > accuracy){
        // determine the position and direction at the point of intersection with the reflection ray
        //the ray only affects th color if it reflected off something
        Vector reflectionIntersectionPosition = intersectionPosition.add(reflectionDirection.mult(reflectionIntersections.at(indexOfWinningObjectWithReflection)));
        Vector reflectionIntersectionRayDirection = reflectionDirection;

        Color reflectionIntersectionColor = getColorAt(reflectionIntersectionPosition, reflectionIntersectionRayDirection, sceneObjects, indexOfWinningObjectWithReflection, lightSources, accuracy, ambientLight);
      
        finalColor = finalColor.add(reflectionIntersectionColor.scalar(winningObjectColor.getColorSpecial()));
      }   
    }

  }

  for (int lightIndex = 0; lightIndex < lightSources.size(); lightIndex ++){
    Vector lightDirection = lightSources.at(lightIndex)->getLightPosition().add(intersectionPosition.negative()).normalize();
    float cosineAngle = winningObjectNormal.dot(lightDirection);

    if (cosineAngle > 0.0){
      //test for shadows
      bool shadowed = false;

      Vector distanceToLight = lightSources.at(lightIndex)->getLightPosition().add(intersectionPosition.negative()).normalize();
      float distanceToLightMagnitude = distanceToLight.magnitude();

      Ray shadowRay(intersectionPosition, lightSources.at(lightIndex)->getLightPosition().add(intersectionPosition.negative()).normalize());
      
      std::vector<double> secundaryIntersections;

      for (int objectIndex = 0; objectIndex < sceneObjects.size() && !shadowed; objectIndex++){
        secundaryIntersections.push_back(sceneObjects.at(objectIndex)->findIntersection(shadowRay));
      }

      for (int c = 0; c < secundaryIntersections.size(); c++){
        if (secundaryIntersections.at(c) > accuracy){
          if (secundaryIntersections.at(c) <= distanceToLightMagnitude){
            shadowed = true;
            //break;
          }
       //   break;
        }
      }

      if(!shadowed){
        finalColor = finalColor.add(winningObjectColor.mult(lightSources.at(lightIndex)->getLightColor()).scalar(cosineAngle));
        if (winningObjectColor.getColorSpecial() > 0.0 && winningObjectColor.getColorSpecial() <= 1.0){
          // special value between 0 and 1 for the brightness 
          double dot1 = winningObjectNormal.dot(intersectionRayDirection.negative());
          Vector scalar1 = winningObjectNormal.mult(dot1);
          Vector add1 = scalar1.add(intersectionRayDirection);
          Vector scalar2 = add1.mult(2);
          Vector add2 = intersectionRayDirection.negative().add(scalar2);
          Vector reflectionDirection = add2.normalize();

          double specular = reflectionDirection.dot(lightDirection);
          
          if (specular > 0.0){
            specular = pow(specular, 10);
            finalColor = finalColor.add(lightSources.at(lightIndex)->getLightColor().scalar(specular*winningObjectColor.getColorSpecial()));
          }

        }
      }

    }
  }
  return finalColor.clip();
}
/*
void makeCube(Vector corner1, Vector corner2, Color color){
  //coner1
  double c1x = corner1.getVectorX();
  double c1y = corner1.getVectorY();
  double c1z = corner1.getVectorZ();

  double c2x = corner2.getVectorX();
  double c2y = corner2.getVectorY();
  double c2z = corner2.getVectorZ();

  Vector A (c2x, c1y, c1z);
  Vector B (c2x, c1y, c2z);
  Vector C (c1x, c1y, c2z);

  Vector D (c2x, c2y, c1z);
  Vector E (c1x, c2y, c1z);
  Vector F (c1x, c2y, c2z);

  //left side 
  sceneObjects.push_back(new Triangle(D, A, corner1, color));
  sceneObjects.push_back(new Triangle(corner1, E, D, color));

  //far side
  sceneObjects.push_back(new Triangle(corner2, B, A, color));  
  sceneObjects.push_back(new Triangle(A, D, corner2, color));

  //right side
  sceneObjects.push_back(new Triangle(F, C, B, color));  
  sceneObjects.push_back(new Triangle(B, corner2, F, color));

  //front side
  sceneObjects.push_back(new Triangle(E, corner1, C, color));  
  sceneObjects.push_back(new Triangle(C, F, E, color));

  //top side
  sceneObjects.push_back(new Triangle(D, E, F, color));  
  sceneObjects.push_back(new Triangle(F, corner2, D, color));

  //bottom side
  sceneObjects.push_back(new Triangle(corner1, A, B, color));  
  sceneObjects.push_back(new Triangle(B, C, corner1, color));
}
*/
int main(int argv, char *argc[]){

  cout << "rendering ..." << endl;

  //window thing
  //clock_t t1, t2;
  //t1 = clock();
  
  //linux thing
  timeval t1, t2 ;
  gettimeofday(&t1,0);  

  int dpi = 72;
  int width = 640;
  int height = 480;
  int n = width * height;

  RGBType *pixels = new RGBType[n];
  std::vector<Object*> sceneObjects;

  int aadepth = 1;//for multisampling thing
  double aaThreshold = 0.1;
 


  double aspectRatio = (double) width / (double) height;
  double ambientLight = 0.2;
  double accuracy = 0.00000001;


  Vector O (0,0,0);
  Vector X (1,0,0);
  Vector X1 (2,0,0);
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
  Color marron (0.5, 0.25, 0.25, 0.1);
  Color tileFloor (1.0, 1.0, 1.0, 2.0);
  Color orange (0.94, 0.75, 0.31, 0.3);

  Vector lightPosition (-7, 10, -10);
  Light sceneLight (lightPosition, whiteLight);
  Light sceneLight2 (Vector(7, 10, -10), whiteLight);

  std::vector<Source*> lightSources;
  lightSources.push_back(dynamic_cast<Source*>(&sceneLight));
  //lightSources.push_back(dynamic_cast<Source*>(&sceneLight2));

  //scene objects
  Sphere sphere(O, 1, prettyGreen);
  Sphere sphere2 (X1, 0.75, marron);
  Sphere sphere3 (Vector(-1, 0.5, -2), 0.8, marron);
  Plane plane(Y, -1.0, tileFloor);
  Plane plane2(Vector(1,0,0), -10.0, marron);
  Triangle triangle (Vector(3.0, 0.0, 0.0), Vector(0.0, 3.0, 0.0),Vector(0.0, 0.0, 3.0), orange);

  
  sceneObjects.push_back(dynamic_cast<Object*>(&sphere));
  sceneObjects.push_back(dynamic_cast<Object*>(&plane));
  sceneObjects.push_back(dynamic_cast<Object*>(&sphere2));
  sceneObjects.push_back(dynamic_cast<Object*>(&sphere3));
//  sceneObjects.push_back(dynamic_cast<Object*>(&triangle));

  //makeCube(Vector(1.0, 1.0, 1.0), Vector(-1.0, -1.0, -1.0), orange);
  int thisone, aa_index;
  double xamnt, yamnt; 
  double tempRed, tempGreen, tempBlue;


  for (int x = 0; x < width; x++){
    for (int y = 0; y < height; y++){
      thisone = y * width + x;



      //start the blank pixel
      double tempRed[aadepth*aadepth];
      double tempGreen[aadepth*aadepth];    
      double tempBlue[aadepth*aadepth];

          
      for (int aax = 0; aax < aadepth; aax++) {
        for (int aay = 0; aay < aadepth; aay++) {    
          
          aa_index = aay * aadepth + aax;

           srand(time(0));
          //create the ray from camera to this pixel
          if (aadepth == 1){
            // start with no anti-aliasing
            if (width > height) {
              // the image is wider than it is tall
              xamnt = ((x+0.5)/width)*aspectRatio - (((width-height)/(double)height)/2);
              yamnt = ((height - y) + 0.5)/height;
            }
            else if (height > width) {
              // the imager is taller than it is wide
              xamnt = (x + 0.5)/ width;
              yamnt = (((height - y) + 0.5)/height)/aspectRatio - (((height - width)/(double)width)/2);
            }
            else {
              // the image is square
              xamnt = (x + 0.5)/width;
              yamnt = ((height - y) + 0.5)/height;
            }

          }
          else{
            //anti-aliasing
            if (width > height) {
              // the image is wider than it is tall
              xamnt = ((x + (double)aax/((double)aadepth - 1))/width)*aspectRatio - (((width-height)/(double)height)/2);
              yamnt = ((height - y) + (double)aax/((double)aadepth - 1))/height;
            }
            else if (height > width) {
              // the imager is taller than it is wide
              xamnt = (x + (double)aax/((double)aadepth - 1))/ width;
              yamnt = (((height - y) + (double)aax/((double)aadepth - 1))/height)/aspectRatio - (((height - width)/(double)width)/2);
            }
            else {
              // the image is square
              xamnt = (x + (double)aax/((double)aadepth - 1))/width;
              yamnt = ((height - y) + (double)aax/((double)aadepth - 1))/height;
            }
          }

          Vector cameraRayOrigin = scaneCam.getCameraPosition();
          Vector cameraRayDirection = camDir.add(camRight.mult(xamnt - 0.5).add(camDown.mult(yamnt- 0.5))).normalize();

          Ray cameraRay(cameraRayOrigin, cameraRayDirection);

          std::vector<double> intersections;
          //find instersection for each object scene
              
          for (int index = 0; index < sceneObjects.size(); index++){
            intersections.push_back(sceneObjects.at(index)->findIntersection(cameraRay));
          }

          int indexOfWinningObject = winningObjectIndex(intersections);
    
          if (indexOfWinningObject < 0.0){
          //set background black

            tempRed[aa_index] = 0.0;
            tempGreen[aa_index] = 0.0;
            tempBlue[aa_index] = 0.0;
          }
          else{      
          // index corresponds to object in our scene
            if (intersections.at(indexOfWinningObject) > accuracy){
            //determine the position and direction vectors at the point of intersection
                  
              Vector intersectionPosition = cameraRayOrigin.add(cameraRayDirection.mult(intersections.at(indexOfWinningObject)));
              Vector intersectionRayDirection = cameraRayDirection;

              Color intersectionColor = getColorAt(intersectionPosition, intersectionRayDirection, sceneObjects, indexOfWinningObject, lightSources, accuracy, ambientLight);

              tempRed[aa_index] = intersectionColor.getColorRed();
              tempGreen[aa_index] = intersectionColor.getColorGreen();
              tempBlue[aa_index] = intersectionColor.getColorBlue();
            }
          }     
        }
      }
      //average the pixel color
      double totalRed = 0;
      double totalGreen = 0;
      double totalBlue = 0;

      for (int indexColor = 0; indexColor < aadepth * aadepth; indexColor++){
        totalRed = totalRed + tempRed[indexColor]; 
        totalGreen = totalGreen + tempGreen[indexColor];
        totalBlue = totalBlue + tempBlue[indexColor];
      }

      double avgRed = totalRed / (aadepth * aadepth); 
      double avgGreen = totalGreen / (aadepth * aadepth);
      double avgBlue = totalBlue / (aadepth * aadepth);

      pixels[thisone].r = avgRed;
      pixels[thisone].g = avgGreen;
      pixels[thisone].b = avgBlue;
    }
  }

  savebmp("scene.bmp", width, height, dpi, pixels);

  delete pixels, tempBlue, tempGreen, tempRed;

  //windows thing
  //t2 = clock();
  //float diff = ((float) t2 - (float) t1);
  
  //linux thing
  gettimeofday(&t2,0);
  float diff = t2.tv_sec - t1.tv_sec;
  
  cout << diff << " seconds" << endl;

  return 0;
}
