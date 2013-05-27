#include "Loader.h"

Loader::Loader(const char *filename, vector<Source*> &sources){
	this->lightSources = sources;
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

  double r,g,b;
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

  	sources.push_back(dynamic_cast<Source*>(new Light(vLightPosition,cLightColor)));
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