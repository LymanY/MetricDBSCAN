#include "E_Operation.h"




void init_PtoE(vector<Point*>& pt, int* PtoE_index, unsigned int* PtoE_value, int* E_new, int E_length, int act_sel, int n, int d) {
	unsigned int min;
	unsigned int current;
	for (int i = 0; i < n; ++i) {
		if (pt[i]->fromE) {
			continue;
		}
		min = square_dist(pt, i, E_new[0], d);
		PtoE_index[i] = E_length;
		for (int j = 1; j < act_sel; ++j) {
			current = square_dist(pt, i, E_new[j], d);
			if (current < min) {
				min = current;
				PtoE_index[i] = E_length + j;
			}
		}
		PtoE_value[i] = min;
	}
}


void update_PtoE(vector<Point*>& pt, int* PtoE_index, unsigned int* PtoE_value,  int* E_new, int E_length, int act_sel, int n ,int d) {
	unsigned int min;
	unsigned int current;
	for (int i = 0; i < n; ++i) {
		if (pt[i]->fromE) {
			continue;
		}
		min = PtoE_value[i];
		for (int j = 0; j < act_sel; ++j) {
			current = square_dist(pt, i, E_new[j],d);
			if (current < min) {
				min = current;
				PtoE_index[i] = E_length + j;
			}
		}
		PtoE_value[i] = min;
	}
}


void AddtoE(vector<Point*>& pt, vector<int>& E, int* PtoE_index, unsigned int* PtoE_value, int* E_new, int E_length, int act_sel) {
	E.resize(E.size() + act_sel); //cout << act_sel << endl;
	for (int i = 0; i < act_sel; ++i) {
		//移除P
		pt[E_new[i]]->fromE = true;
		//加入E
		E[E_length + i] = E_new[i];
		//save EtoE
		PtoE_index[E_new[i]] = E_length + i;
		PtoE_value[E_new[i]] = 0;
	}
	//cout << act_sel << endl;//E.assign(E_new,E_new+act_sel+1);
}


int delete_nearE(vector<Point*>& pt, int* E_new, int beforDel, double r, int d) {
	int afterDel = beforDel;
	for (int i = 1; i < afterDel; ++i) {
		for (int j = 0; j < i; j++) {
			if (square_dist(pt, E_new[i], E_new[j], d) < r) {
				E_new[i] = E_new[afterDel - 1];
				afterDel--;
				i--;
				break;
			}
		}
	}
	return afterDel;
}


void generate_distE(unsigned int* dist_E, vector<Point*>& points, vector<int>& E, int E_length, int d) {
	//三角矩阵
	int index;
	for (int i = 0; i < E_length - 1; ++i) {
		for (int j = i + 1; j < E_length; ++j) {
			index = i * E_length - (3 + i) * i / 2 + j - 1;
			dist_E[index] = square_dist(points, E[i], E[j], d);
		}
	}
	//二维数组
}

//inline 
unsigned int query_distE(unsigned int* dist_E, int a, int b, int E_length) {
	if (a == b) {
		return 0;
	}
	int index;
	if (a > b) {
		//swap(a, b);
		index = b * E_length - (3 + b) * b / 2 + a - 1;
	}
	else {
		index = a * E_length - (3 + a) * a / 2 + b - 1;
	}
	return dist_E[index];
}