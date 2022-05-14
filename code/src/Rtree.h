
#ifndef RTREE_H
#define RTREE_H

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include "Point.h"
#include "Global.h"
using namespace std;


bool comp(const Point* p1, const Point* p2);

class Rectangle {

	public:
		// The list of lower coordinates.
		int* minValues;

		// The list of higher coordinates.
		int* maxValues;

		Rectangle() ;

		Rectangle(int* minValues, int* maxValues) ;

		~Rectangle() ;
		
		Rectangle& enlarge(const Rectangle& rec) ;

		void setMinCoords(const int* minCoords) ;

		void setMaxCoords(const int* maxCoords) ;

		int stateWithSphere(Point* q, double sqr_eps) ;
	

};

class RtreeNode {
	protected:

	//  The list of child nodes.
	vector<RtreeNode*> childNodes;


	public:

		// The level of this node in the R-tree. All the leaves are at level-0.
		int level;

		//  The minimum bounding rectangle of the child nodes.
		Rectangle mbr;

		// The data object in of this node if this is a leaf.
		Point* pt;

		RtreeNode() ;

		RtreeNode(Point* pt) ;

		~RtreeNode() ;

		void releaseSpace() ;

		vector<RtreeNode*>& getChildNodes() ;

		void addChildNode(RtreeNode* child) ;
		
};

class Rtree {
	protected:

		RtreeNode* root;

	public:

		Rtree() ;

		~Rtree() ;

		void releaseSpace() ;

		void bulkLoadRtree(vector<Point*>& ptList) ;

		void rangeQuery(Point* pt, double sqr_eps, vector<Point*>& targetPlace) ;

};


#endif