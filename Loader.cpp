#include "Loader.h"

Loader::Loader(const char *filename){
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
	//std::cout << "depth " << multisampling.attribute("depth").value() << std::endl;
  //std::cout << "threshold " << multisampling.attribute("threshold").value() << std::endl;
 

  /*
    //[code_traverse_base_basic
  for (pugi::xml_node tool = tools.first_child(); tool; tool = tool.next_sibling()){
        std::cout << "Tool:";
    for (pugi::xml_attribute attr = tool.first_attribute(); attr; attr = attr.next_attribute()){
            std::cout << " " << attr.name() << "=" << attr.value();
    }

    std::cout << std::endl;
  }
    //]

    std::cout << std::endl;

  */

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
