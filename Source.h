#ifndef __SOURCE_H_
#define __SOURCE_H_

#include "Vector.h"
#include "Color.h"

class Source{
public:

	Source();

	virtual Vector getLightPosition();
	virtual Color getColor();

};
#endif
