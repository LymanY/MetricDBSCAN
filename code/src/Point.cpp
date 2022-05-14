#include "Point.h"



Point::Point(int* coords) {
	this->coords = coords;
	this->fromE = false;
	this->isCorePoint = false;
	this->visited = false;
	clustringID = -1;
	neighbor = NULL;
	countn = 0;
}