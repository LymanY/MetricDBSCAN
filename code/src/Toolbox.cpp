#include "Toolbox.h"



void AdjustDown(int* top, unsigned int* topvalue, int root, int size) {
	int child = 2 * root + 1;//下标从0开始,左孩子
	while (root < size / 2) {
		if (child + 1 < size && topvalue[child] > topvalue[child + 1]) {
			child++;
		}
		if (child<size && topvalue[root]>topvalue[child]) {
			swap(topvalue[root], topvalue[child]);
			swap(top[root], top[child]);
			root = child;
			child = 2 * root + 1;
		}
		else {
			break;
		}
	}
}

//计算最远k个点
unsigned int furthest_KinP(int* Q, vector<Point*>& pt, unsigned int* PtoE_value, int k, int n) {

	//堆排序法
	unsigned int* Qvalue = new unsigned int[k];
	int j = 0;
	for (int i = 0; i < k; ++i) {
		while (j < n && pt[j]->fromE) {
			j++;
		}
		if (j >= n) {



			cout << "error furthest_KinP\n";
			k = i;
			exit(0);
			break;
		}
		Qvalue[i] = PtoE_value[j];
		Q[i] = j;
		j++;
	}
	for (int i = k / 2 - 1; i >= 0; i--) {
		AdjustDown(Q, Qvalue, i, k);
	}
	for (; j < n; j++) {
		if (PtoE_value[j] > Qvalue[0]) {
			Qvalue[0] = PtoE_value[j];
			Q[0] = j;
			AdjustDown(Q, Qvalue, 0, k);
		}
	}
	unsigned int dis_QtoE = Qvalue[0];
	delete[] Qvalue;
	Qvalue = NULL;
	return dis_QtoE;
}

//select num different numbers in [0,sup-1] randomly
void select_random_1(int* pick, int num, int sup) {
	if (sup < num) {
		cout << "error select_random" << endl;
		exit(0);
		return;
	}
	int temp;
	int flag;
	srand((unsigned)(time(0)));
	//总数很大适合选用
	for (int i = 0; i < num; ++i) {
		while (true) {
			flag = 0;
			temp = (rand() % sup);
			for (int j = 0; j < i; ++j) {
				if (pick[j] == temp) {
					flag = 1;
					break;
				}
			}
			if (flag == 0) {
				pick[i] = temp;
				break;
			}
		}
	}


}

void select_random_2(int* pick, int num, int sup) {
	if (sup < num) {
		cout << "error select_random" << endl;
		return;
	}
	int temp;
	srand((unsigned)(time(0)));

	int* line = new int[sup];
	for (int i = 0; i < sup; ++i) {
		line[i] = i;
	}
	for (int i = 0; i < num; ++i) {
		temp = (rand() % (sup - i)) + i;
		pick[i] = line[temp];
		line[temp] = line[i];
	}
	delete[] line;
	line = NULL;

}