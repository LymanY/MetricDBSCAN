
#ifndef E_OPERATION_H
#define E_OPERATION_H


#include <iostream>
#include <vector>
#include "Point.h"
#include "Distance.h"
using namespace std;


// initial PtoE
void init_PtoE(vector<Point*>& pt, int* PtoE_index, unsigned int* PtoE_value, int* E_new, int E_length, int act_sel, int n, int d) ;

// update PtoE
void update_PtoE(vector<Point*>& pt, int* PtoE_index, unsigned int* PtoE_value,  int* E_new, int E_length, int act_sel, int n ,int d) ;

//
void AddtoE(vector<Point*>& pt, vector<int>& E, int* PtoE_index, unsigned int* PtoE_value, int* E_new, int E_length, int act_sel) ;

//删除E_new相近的点
int delete_nearE(vector<Point*>& pt, int* E_new, int beforDel, double r, int d) ;

//生成E间距离
void generate_distE(unsigned int* dist_E, vector<Point*>& points, vector<int>& E, int E_length, int d) ;

//查询E间距离
//inline 
unsigned int query_distE(unsigned int* dist_E, int a, int b, int E_length) ;



#endif