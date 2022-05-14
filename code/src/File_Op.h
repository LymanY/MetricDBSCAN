
#ifndef FILE_OP_H
#define FILE_OP_H


#include <iostream>
#include <fstream>
#include <vector>
#include "Point.h"
using namespace std;


Point* readPointFromFile(FILE* fileHandle, int d) ;

//n is the number of points, d is dimensionality
void readDatasetFromFile(char* filePath, vector<Point*>& pt, int n, int d) ;


void writeClusterToFile(vector<Point*>& points, char* filePath, int n) ;




#endif