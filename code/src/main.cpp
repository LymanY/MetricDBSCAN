//g++ -O3 -w -std=c++11 ./src/*.cpp -o DBSCAN
//g++ -O3 -w -std=c++11 ./src/*.cpp -o DBSCAN -DWIN
//./dbscan "testfile2.txt" "result.txt" 11837 498 10 80 75 100 15 2/1
//./dbscan -algo 1 -n 11837 -d 2 -r 10 -k 80 -ds "testfile.txt" -rf "result.txt"


#include <iostream>
#include <ctime>
#include <vector>
#include <memory.h>
#include "Point.h"
#include "Process.h"
#include "File_Op.h"
#include "Global.h"

using namespace std;


int main(int argc, char *argv[]) {
	//定义
	int n;
	int d;
	vector<Point*> points;
	vector<int> E;
	int E_length = E.size();
	double r;
	int init;//初次
	int k;//循环第一步
	int ran_sel;//循环第二步
	double Eps;
	int MinPts;
	int op;//是否加入过滤操作
    ///////////////////////////////////////////////////////////////////////////
	/*char data_file[50] = "testfile2.txt";
	char result_file[50] = "result.txt";
	n = 11837;
	d = 498;
	Eps = 10;
	MinPts = 80;
	init = 75;
	k = 100;
	ran_sel = init;
	r = 15;*/


	char *data_file = argv[1];
	char *result_file = argv[2];
	n = atoi(argv[3]);
	d = atoi(argv[4]);
	Eps = atof(argv[5]);
	MinPts = atoi(argv[6]);
	init = atoi(argv[7]);
	k = atoi(argv[8]);
	ran_sel = init;
	r = atof(argv[9]);
	op = atoi(argv[10]);
    //////////////////////////////////////////////////////////////////////////
	DIM = d;
	int* PtoE_index = new int[n];
	unsigned int* PtoE_value = new unsigned int[n];
	//vector<int> Pz;


	//执行程序
	//读入
	readDatasetFromFile(data_file, points, n, d);

	clock_t start = clock();
    //////////////////////////////////////////////////////////////////////////
	//Part-one
	if(op==1 || op==2){
		E_length = CalE(points, E, PtoE_index, PtoE_value, r, init, k, ran_sel, E_length, n, d, op);
	}
	else{

	}

	//释放内存 
	//delete[] PtoE_value;
	//PtoE_value = NULL;

	clock_t tt = clock();
	cout << "E length " << E_length << endl;

	//Part-two-a
	CorePoint(points, PtoE_index, PtoE_value, E, E_length, MinPts, Eps, r, n, d);
	//释放内存
	PtoE_index = NULL;
	PtoE_value = NULL;
	vector<int>().swap(E);
	//vector<int>().swap(Pz);


	//Part-two-b
	DBSCAN(points, n);
    ///////////////////////////////////////////////////////////////////////////
	clock_t end = clock();	



	//statistics
	int tocount = 0;
	int* count = new int[cata];
	memset(count, 0, sizeof(int)*cata);
	for(int i = 0; i < points.size() ; i++){
		if(points[i]->clustringID!=-1){
			count[points[i]->clustringID-1]++;
			tocount++;

		}
	}
	/*for(int i = 0 ; i < cata ; i++){
		cout << i+1 << "\t" << count[i] << endl;
	}*/
	cout << "Cluster Points\t" << tocount << "/" << n << endl;

	//Print time-consuming
	cout << "part1 time\t" << (double)(tt - start) / (double)CLOCKS_PER_SEC << endl;
	cout << "part2 time\t" << (double)(end - tt) / (double)CLOCKS_PER_SEC << endl;	
	cout << "total time\t" << (double)(end - start) / (double)CLOCKS_PER_SEC << endl;


	//输出
	writeClusterToFile(points, result_file, n);
	//system("pause");

	return 0;

}