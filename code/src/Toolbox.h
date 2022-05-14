
#ifndef TOOLBOX_H
#define TOOLBOX_H


#include <iostream>
#include <vector>
#include <ctime>
#include "Point.h"
using namespace std;


void AdjustDown(int* top, unsigned int* topvalue, int root, int size) ;

//计算最远k个点
unsigned int furthest_KinP(int* Q, vector<Point*>& pt, unsigned int* PtoE_value, int k, int n) ;

//随机选择0~sup-1中的num个不同数字
void select_random_1(int* pick, int num, int sup) ;

void select_random_2(int* pick, int num, int sup) ;



#endif