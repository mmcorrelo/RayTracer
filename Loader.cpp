#include "Loader.h"

Loader::Loader(const char *filename, vector<Object*> &sceneObjects, vector<Source*> &lightSources){
	pugi::xml_document doc;	

  if (!doc.load_file(filename)){
  	std::cout << "Error loading xml file" << std::endl; 
  	exit(0);
  } 
  pugi::xml_node scene = doc.child("scene");
  pugi::xml_attribute sceneName = scene.first_attribute();
  this->modelName = sceneName.value();

  pugi::xml_node author = scene.child("author");
  this->author = author.child_value();

	pugi::xml_node date = scene.child("date");
	this->date = date.child_value();

	pugi::xml_node resolution = scene.child("resolution");
	this->width = resolution.attribute("width").as_int();
  this->height = resolution.attribute("height").as_int();
  this->dpi = resolution.attribute("dpi").as_int();
  
  pugi::xml_node multisampling = scene.child("multisampling");
  this->depth = multisampling.attribute("depth").as_int();
  this->threshold = multisampling.attribute("threshold").as_double();
	
  pugi::xml_node camera = scene.child("camera");
  pugi::xml_node position = camera.child("position");

  double x,y,z;
  x = position.attribute("x").as_double();
  y = position.attribute("y").as_double();
  z = position.attribute("z").as_double();
  this->camPos = Vector(x,y,z);

  pugi::xml_node lookat = camera.child("lookat");
  x = lookat.attribute("x").as_double();
  y = lookat.attribute("y").as_double();
  z = lookat.attribute("z").as_double();
  this->lookAt = Vector(x,y,z);

  pugi::xml_node lights = scene.child("lights");

  double r,g,b,special;
  for (pugi::xml_node light = lights.first_child(); light; light = light.next_sibling()){
  	pugi::xml_node lightPosition = light.child("position");
  	x = lightPosition.attribute("x").as_double();
  	y = lightPosition.attribute("y").as_double();
  	z = lightPosition.attribute("z").as_double();
  	Vector vLightPosition = Vector(x, y, z);

  	pugi::xml_node lightColor = light.child("color");
  	r = lightColor.attribute("r").as_double();
  	g = lightColor.attribute("g").as_double();
  	b = lightColor.attribute("b").as_double();

  	Color cLightColor = Color(r, g, b, 0.0);

  	lightSources.push_back(dynamic_cast<Source*>(new Light(vLightPosition,cLightColor)));
  }

  pugi::xml_node objects = scene.child("objects");
  pugi::xml_node planes = objects.child("planes");
  pugi::xml_node spheres = objects.child("spheres");
  pugi::xml_node triangles = objects.child("triangles");

  for (pugi::xml_node_iterator it = planes.begin(); it != planes.end(); ++it){
  	double distance = it->first_attribute().as_double();
  
    pugi::xml_node planePosition = it->child("normal");
	 	x = planePosition.attribute("x").as_double();
	 	y = planePosition.attribute("y").as_double();
	 	z = planePosition.attribute("z").as_double();

	 	Vector vPlanePosition(x, y, z);
	  pugi::xml_node planeColor = it->child("color");
	 	r = planeColor.attribute("r").as_double();
		g = planeColor.attribute("g").as_double();
	 	b = planeColor.attribute("b").as_double();
		special = planeColor.attribute("special").as_double();
	
		Color cPlaneColor(r, g, b, special);
		sceneObjects.push_back(dynamic_cast<Object*>(new Plane (vPlanePosition, distance, cPlaneColor)));    
 }
 
 for (pugi::xml_node_iterator it = spheres.begin(); it != spheres.end(); ++it){
  	double radius = it->first_attribute().as_double();
  
    pugi::xml_node sphereCenter = it->child("center");
	 	x = sphereCenter.attribute("x").as_double();
	 	y = sphereCenter.attribute("y").as_double();
	 	z = sphereCenter.attribute("z").as_double();

	 	Vector vSphereCenter(x, y, z);
	  pugi::xml_node sphereColor = it->child("color");
	 	r = sphereColor.attribute("r").as_double();
		g = sphereColor.attribute("g").as_double();
	 	b = sphereColor.attribute("b").as_double();
		special = sphereColor.attribute("special").as_double();

		Color cSphereColor(r, g, b, special);
		sceneObjects.push_back(dynamic_cast<Object*>(new Sphere (vSphereCenter, radius, cSphereColor)));    
 }

 for (pugi::xml_node_iterator it = triangles.begin(); it != triangles.end(); ++it){
  	
 
    pugi::xml_node pointA = it->child("point");
	 	x = pointA.attribute("x").as_double();
	 	y = pointA.attribute("y").as_double();
	 	z = pointA.attribute("z").as_double();
	 	Vector A(x, y, z);
	 	cout << x << " " <<  y << " " << z << endl;

	 	pugi::xml_node pointB = pointA.next_sibling();
	 	x = pointB.attribute("x").as_double();
	 	y = pointB.attribute("y").as_double();
	 	z = pointB.attribute("z").as_double();
	 	Vector B(x, y, z);
	 	cout << x << " " <<  y << " " << z << endl;


	 	pugi::xml_node pointC = pointB.next_sibling();
	 	x = pointC.attribute("x").as_double();
	 	y = pointC.attribute("y").as_double();
	 	z = pointC.attribute("z").as_double();
	 	Vector C(x, y, z);
	 	cout << x << " " <<  y << " " << z << endl;


	  pugi::xml_node colorTriangle = it->child("color");
	 	r = colorTriangle.attribute("r").as_double();
		g = colorTriangle.attribute("g").as_double();
	 	b = colorTriangle.attribute("b").as_double();
		special = colorTriangle.attribute("special").as_double();

		Color cColorTriangle(r, g, b, special);
		sceneObjects.push_back(dynamic_cast<Object*>(new Triangle(A, B, C, cColorTriangle)));    
 }



}

int Loader::getWidth(){
	return this->width;
}
int Loader::getHeight(){
	return this->height;
}
int Loader::getDpi(){
	return this->dpi;
}
int Loader::getDepth(){
	return this->depth;
}
double Loader::getThreshold(){
	return this->threshold;
}

string Loader::getModelName(){
	return this->modelName;
}

string Loader::getAuthor(){
	return this->author;
}

string Loader::getDate(){
	this->date;
}

Vector Loader::getCameraPosition(){
	return this->camPos;
}

Vector Loader::getLookAt(){
	return this->lookAt;
}
/*
void Loader::getLightSources(const vector<Source*> lSources){
	for(int i = 0; i < this->lightSources.size(); i++){
		Light light = this->lightSources.at(i)->clone();
		lSources.push_back(light);
	}
}*/