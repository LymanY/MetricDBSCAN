
#ifndef PROCESS_H
#define PROCESS_H


#include <iostream>
#include <vector>
#include <ctime>
#include "Point.h"
#include "Toolbox.h"
#include "E_Operation.h"
#include "Rtree.h"
#include "Distance.h"
#include "Global.h"
using namespace std;



int CalE(vector<Point*>& points, vector<int>& E, int* PtoE_index, unsigned int* PtoE_value, double r, int init, int k, int ran_sel, int E_length, int n, int d, int op) ;

void CorePoint(vector<Point*>& points, int* PtoE_index, unsigned int* PtoE_value, vector<int>& E, int E_length, int MinPts, double Eps, double r, int n, int d) ;

void DBSCAN(vector<Point*>& points, int n) ;



#endif