#include "Util.h"

int Util::winningObjectIndex(std::vector<double> object_intersections){
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


void Util::savebmp(const char *filename, int w, int h, int dpi, RGBType *data){
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


Color Util::getColorAt(Vector intersectionPosition, Vector intersectionRayDirection, std::vector<Object*> sceneObjects, int indexOfWinningObject, std::vector<Source*> lightSources, double accuracy, double ambientLight){
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

    int indexOfWinningObjectWithReflection = Util::winningObjectIndex(reflectionIntersections);

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
