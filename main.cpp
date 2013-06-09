
//linux header
#include <unistd.h>
#include <sys/time.h>
//------

#include "Util.h"
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
#include "Loader.h"

using namespace std;




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
  std::vector<Source*> lightSources;
  std::vector<Object*> sceneObjects;

  cout << "rendering ..." << endl;
  Loader *config = new Loader("models/model1.xml", sceneObjects, lightSources);
  //window thing
  //clock_t t1, t2;
  //t1 = clock();

  //linux thing
  timeval t1, t2 ;
  gettimeofday(&t1,0);

  int dpi = config->getDpi();
  int width = config->getWidth();
  int height = config->getHeight();
  int n = width * height;

  RGBType *pixels = new RGBType[n];


  int aadepth = config->getDepth();//for multisampling thing
  double aaThreshold = config->getThreshold();



  double aspectRatio = (double) width / (double) height;
  double ambientLight = 0.2;
  double accuracy = 0.00000001;

  /*
  Vector O (0,0,0);
  Vector X (1,0,0);
  Vector X1 (2,0,0);
  Vector Z (0,0,1);*/

  Vector Y (0,1,0);
  Vector camPos = config->getCameraPosition();
  Vector lookAt = config->getLookAt();

  Vector diff_btw(camPos.getVectorX() - lookAt.getVectorX(), camPos.getVectorY() - lookAt.getVectorY(),camPos.getVectorZ() - lookAt.getVectorZ());
  Vector camDir = diff_btw.negative().normalize();
  Vector camRight = Y.cross(camDir).normalize();
  Vector camDown = camRight.cross(camDir);
  Camera scaneCam (camPos, camDir, camRight, camDown);
/*
  Color whiteLight (1.0, 1.0, 1.0, 0.0);
  Color prettyGreen (0.5, 1.0, 0.5, 0.3);
  Color gray (0.5, 0.5, 0.5, 0.0);
  Color back (0.0, 0.0, 0.0, 0.0);
  Color marron (0.5, 0.25, 0.25, 0.1);
  Color tileFloor (1.0, 1.0, 1.0, 2.0);
  Color orange (0.94, 0.75, 0.31, 0.3);

 */

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

          int indexOfWinningObject = Util::winningObjectIndex(intersections);

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

              Color intersectionColor = Util::getColorAt(intersectionPosition, intersectionRayDirection, sceneObjects, indexOfWinningObject, lightSources, accuracy, ambientLight);

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

  Util::savebmp("scene.bmp", width, height, dpi, pixels);

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
