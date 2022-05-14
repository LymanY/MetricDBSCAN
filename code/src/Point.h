
#ifndef POINT_H
#define POINT_H

#include <iostream>
using namespace std;


class Point {
	public:

		int* coords;//坐标
		int clustringID;
		Point** neighbor;
		int countn;

		bool fromE;//属于P，不属于E，则不是core，返回false
		bool isCorePoint;
		bool visited;

		Point(int* coords);

};


#endif