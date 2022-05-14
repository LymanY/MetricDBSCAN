#include "Distance.h"



unsigned int square_dist(vector<Point*>& pt, int a, int b, int d) {
	//if (a > n || b > n) {
	//	cout << "" << endl;
	//}
	
	register unsigned int temp;
	//double temp;
	register unsigned int dist = 0;//sue long/longlong, when distances are large
	//double dist = 0;
	for (int i = 0; i < d; ++i) {
		temp = pt[a]->coords[i] - pt[b]->coords[i];
		dist += temp * temp;
	}
	return dist;
}