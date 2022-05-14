#include "Process.h"



int CalE(vector<Point*>& points, vector<int>& E, int* PtoE_index, unsigned int* PtoE_value, double r, int init, int k, int ran_sel, int E_length, int n, int d, int op) {


	double sqr_r = r * r;
	double del_r = (r/2.0)*(r/2.0);

	//初次选点
	int* E_first = new int[init];//数组值为在Points中的下标
	select_random_1(E_first, init, n);
	int act_sel;
	if(op == 2)
		act_sel = delete_nearE(points, E_first, init, del_r, d);
	if(op == 1)
		act_sel = init;
	
	//加入E，移除P
	AddtoE(points, E, PtoE_index, PtoE_value, E_first, E_length, act_sel);
	//计算PE距离
	init_PtoE(points, PtoE_index, PtoE_value, E_first, E_length, act_sel, n, d);
	E_length += act_sel;

	//释放E_first
	delete[] E_first;
	E_first = NULL;


	//循环
	unsigned int dis_QtoE;
	int* Q = new int[k];//值为在points中的下标
	int* E_new = new int[ran_sel];//值为在points中的下标
	int* index_Q = new int[ran_sel];//值为在Q中的下标
	dis_QtoE = furthest_KinP(Q, points, PtoE_value, k, n); //cout << dis_QtoE << "\t" << E_length << endl;
	while (dis_QtoE > sqr_r) {
		select_random_2(index_Q, ran_sel, k);
		for (int i = 0; i < ran_sel; ++i) {
			E_new[i] = Q[index_Q[i]];
		}
		if(op == 1)
			act_sel = ran_sel;
		if(op == 2)
			act_sel = delete_nearE(points, E_new, ran_sel, del_r, d);
		AddtoE(points, E, PtoE_index, PtoE_value, E_new, E_length, act_sel);
		update_PtoE(points, PtoE_index, PtoE_value, E_new, E_length, act_sel, n, d);
		E_length += act_sel;
		dis_QtoE = furthest_KinP(Q, points, PtoE_value, k, n);
		//cout << dis_QtoE <<"\t"<< E_length << endl;
	}
	/*for (int i = 0; i < k; ++i) {
		if(PtoE_value[Q[i]] > sqr_r){
			Pz.emplace_back(Q[i]);
		}
	}*/
	delete[] Q;
	delete[] index_Q;
	delete[] E_new;
	Q = NULL;
	index_Q = NULL;
	E_new = NULL;

	return E_length;
}

void CorePoint(vector<Point*>& points, int* PtoE_index, unsigned int* PtoE_value, vector<int>& E, int E_length, int MinPts, double Eps, double r, int n, int d) {

	//clock_t t1 = clock();
	bool outliers = false;

	//calculate dist_E
	unsigned int* dist_E = new unsigned int[(E_length - 1) * E_length / 2];
	generate_distE(dist_E, points, E, E_length, d);


	double sqr_r = r * r;
	//calculate EtoP
	//vector<int>* EtoP = new vector<int>[E_length];//1+	
	vector<int>* E2P = new vector<int>[E_length + 1];
	for (int i = 0; i < n; i++) {
		//EtoP[PtoE_index[i]].emplace_back(i);
		if (PtoE_value[i] <= sqr_r) {
			E2P[PtoE_index[i]].emplace_back(i);
		}
		else {
			E2P[E_length].emplace_back(i);
		}
	}
	if (E2P[E_length].size() > 0) {
		outliers = true;
	}
	//cout<< E2P[E_length].size() << endl;

	//delete[] PtoE_index;
	delete[] PtoE_value;
	//PtoE_value = NULL;

	//clock_t t2 = clock();
	//cout << "t2-t1:\t" << (t2-t1) / (double)CLOCKS_PER_SEC << endl;

	//construct Rtree for each Core
	int npsize = 0;
	vector<Point* > near_point;
	Rtree* rtree = new Rtree[E_length + 1];//1+
	for (int i = 0; i < E_length + 1; ++i) {//+1
		npsize = E2P[i].size();
		near_point.resize(npsize);
		for (int j = 0; j < npsize; ++j) {
			near_point[j] = points[E2P[i][j]];
		}
		rtree[i].bulkLoadRtree(near_point);

		vector<Point*>().swap(near_point);
		near_point.clear();
	}

	//clock_t t3 = clock();
	//cout << "t3-t2:\t" << (t3-t2) / (double)CLOCKS_PER_SEC << endl;

	//double rq = 0;


	//label Core points
	//int factor = 2;
	//double sqr_bound = (factor * r + Eps) * (factor * r + Eps);
	double sqr_bound = (2 * r + Eps) * (2 * r + Eps);
	double sqr_eps = Eps * Eps;

	Point* cur;
	int ccount = 0;
	int epsize = 0;
	vector<Point*> targetPlace;
	vector<Point*> SumField;
	vector<int> E_neighbor;

	//zrq1 = 0;
	//zrq2 = 0;
	//ls = 0;

	//clock_t r1 = clock();
	//clock_t r2 = clock();
	//double tr = 0;
	//int tc = 0;
	//start loop
	for (int i = 0; i < E_length; ++i) {
		//store Near Core index
		for (int j = 0; j < E_length; ++j) {
			if (query_distE(dist_E, i, j, E_length) <= sqr_bound) {
				E_neighbor.emplace_back(j);
			}
		}
		if (outliers) {
			E_neighbor.emplace_back(E_length);
		}
		//E_neighbor.emplace_back(E_length);

		//Foreach in this Core
		epsize = E2P[i].size();
		for (int t = 0; t < epsize; ++t) {
			ccount = 0;
			cur = points[E2P[i][t]];


			//t3 = clock();

			for (vector<int>::iterator iter = E_neighbor.begin(); iter != E_neighbor.end(); ++iter) {
				//r1 = clock();
				rtree[*iter].rangeQuery(cur, sqr_eps, targetPlace);
				//r2 = clock();
				//tc += 1;
				//tr += (r2-r1);
				ccount += targetPlace.size();
				SumField.reserve(ccount);
				SumField.insert(SumField.end(), targetPlace.begin(), targetPlace.end());
				vector<Point*>().swap(targetPlace);
				targetPlace.clear();
			}




			//when it is corepoint
			if (ccount >= MinPts) {
				cur->isCorePoint = true;
				cur->countn = ccount;
				//points[i]->neighbor = &this_neighbor[0]; 
				cur->neighbor = new Point * [ccount];
				for (int k = 0; k < ccount; k++) {
					//points[index]->neighbor[t] = this_neighbor[t];
					cur->neighbor[k] = SumField[k];
				}
			}

			vector<Point*>().swap(SumField);
			SumField.clear();
		}


		vector<int>().swap(E_neighbor);
		E_neighbor.clear();
	}
	//cout << "tc	" << tc << endl;
	//cout << "tr	" << tr / (double)CLOCKS_PER_SEC << endl;
	//cout << "zrq1	" << zrq1 / (double)CLOCKS_PER_SEC << endl;
	//cout << "zrq2	" << zrq2 / (double)CLOCKS_PER_SEC << endl;
	//cout << "ls" << ls << endl;

	//cout << E_length << endl;
	//clock_t t4 = clock();
	//cout << "t4-t3:\t" << (t4-t3) / (double)CLOCKS_PER_SEC << endl;

	if (outliers) {
		double sqr_b = (r + Eps) * (r + Eps);
		double sqr_bd = 4 * (r + Eps) * (r + Eps);
		int epsize = E2P[E_length].size();//cout << "epsize " << epsize << endl;
		int curind;
		int eind;
		for (int i = 0; i < epsize; i++) {
			ccount = 0;
			curind = E2P[E_length][i];
			cur = points[curind];
			eind = PtoE_index[curind];
			for (int j = 0; j < E_length; ++j) {
				if(query_distE(dist_E, eind, j, E_length) <= sqr_bd){
					if (square_dist(points, curind, E[j], d) <= sqr_b) {
						E_neighbor.emplace_back(j);
					}
				}				
			}
			E_neighbor.emplace_back(E_length);

			for (vector<int>::iterator iter = E_neighbor.begin(); iter != E_neighbor.end(); ++iter) {
				rtree[*iter].rangeQuery(cur, sqr_eps, targetPlace);
				ccount += targetPlace.size();
				SumField.reserve(ccount);
				SumField.insert(SumField.end(), targetPlace.begin(), targetPlace.end());
				vector<Point*>().swap(targetPlace);
				targetPlace.clear();
			}




			//when it is corepoint
			if (ccount >= MinPts) {
				cur->isCorePoint = true;
				cur->countn = ccount;
				//points[i]->neighbor = &this_neighbor[0]; 
				cur->neighbor = new Point * [ccount];
				for (int k = 0; k < ccount; k++) {
					//points[index]->neighbor[t] = this_neighbor[t];
					cur->neighbor[k] = SumField[k];
				}
			}

			vector<Point*>().swap(SumField);
			SumField.clear();
			vector<int>().swap(E_neighbor);
			E_neighbor.clear();
		}
	}

	//clock_t t5 = clock();
	//cout << "t5-t4:\t" << (t5-t4) / (double)CLOCKS_PER_SEC << endl;

	for (int i = 0; i < E_length + 1; ++i) {
		rtree[i].releaseSpace();
	}


	//cout <<"rangequery time\t"<< rq / (double)CLOCKS_PER_SEC << endl;	



	//release memory
	delete[] dist_E;
	dist_E = NULL;
	for (int i = 0; i < E_length; i++) {
		//EtoP[i].clear();
		//vector<int>().swap(EtoP[i]);
		vector<int>().swap(E2P[i]);
	}
	vector<int>().swap(E2P[E_length]);
	//delete[] EtoP;
	delete[] E2P;
	//EtoP = NULL;
	E2P = NULL;
	delete[] PtoE_index;
}

// void CorePoint(vector<Point*>& points, int* PtoE_index, unsigned int* PtoE_value, vector<int>& E, int E_length, int MinPts, double Eps, double r, int n, int d) {

// 	clock_t t1 = clock();
// 	bool outliers = false;

// 	//calculate dist_E
// 	unsigned int* dist_E = new unsigned int[(E_length - 1) * E_length / 2];
// 	generate_distE(dist_E, points, E, E_length, d);


// 	double sqr_r = r * r;
// 	//calculate EtoP
// 	//vector<int>* EtoP = new vector<int>[E_length];//1+	
// 	vector<int>* E2P = new vector<int>[E_length + 1];
// 	for (int i = 0; i < n; i++) {
// 		//EtoP[PtoE_index[i]].emplace_back(i);
// 		if (PtoE_value[i] <= sqr_r) {
// 			E2P[PtoE_index[i]].emplace_back(i);
// 		}
// 		else {
// 			E2P[E_length].emplace_back(i);
// 		}
// 	}
// 	if (E2P[E_length].size() > 0) {
// 		outliers = true;
// 	}
// 	//cout<< E2P[E_length].size() << endl;

// 	//delete[] PtoE_index;
// 	delete[] PtoE_value;
// 	//PtoE_value = NULL;

// 	clock_t t2 = clock();
// 	cout << "t2-t1:\t" << (t2-t1) / (double)CLOCKS_PER_SEC << endl;

// 	//construct Rtree for each Core
// 	int npsize = 0;
// 	vector<Point* > near_point;
// 	Rtree* rtree = new Rtree[E_length + 1];//1+
// 	for (int i = 0; i < E_length + 1; ++i) {//+1
// 		npsize = E2P[i].size();
// 		near_point.resize(npsize);
// 		for (int j = 0; j < npsize; ++j) {
// 			near_point[j] = points[E2P[i][j]];
// 		}
// 		rtree[i].bulkLoadRtree(near_point);

// 		vector<Point*>().swap(near_point);
// 		near_point.clear();
// 	}


// 	//double rq = 0;


// 	//label Core points
// 	//int factor = 2;
// 	//double sqr_bound = (factor * r + Eps) * (factor * r + Eps);
// 	double sqr_bound = (2 * r + Eps) * (2 * r + Eps);
// 	double sqr_eps = Eps * Eps;

// 	Point* cur;
// 	int ccount = 0;
// 	int epsize = 0;
// 	vector<Point*> targetPlace;
// 	vector<Point*> SumField;
// 	vector<int> E_neighbor;
// 	//start loop
// 	for (int i = 0; i < E_length; ++i) {
// 		//store Near Core index
// 		for (int j = 0; j < E_length; ++j) {
// 			if (query_distE(dist_E, i, j, E_length) <= sqr_bound) {
// 				E_neighbor.emplace_back(j);
// 			}
// 		}
// 		if (outliers) {
// 			E_neighbor.emplace_back(E_length);
// 		}
// 		//E_neighbor.emplace_back(E_length);

// 		//Foreach in this Core
// 		epsize = E2P[i].size();
// 		for (int t = 0; t < epsize; ++t) {
// 			ccount = 0;
// 			cur = points[E2P[i][t]];


// 			//t3 = clock();

// 			for (vector<int>::iterator iter = E_neighbor.begin(); iter != E_neighbor.end(); ++iter) {
// 				rtree[*iter].rangeQuery(cur, sqr_eps, targetPlace);
// 				ccount += targetPlace.size();
// 				SumField.reserve(ccount);
// 				SumField.insert(SumField.end(), targetPlace.begin(), targetPlace.end());
// 				vector<Point*>().swap(targetPlace);
// 				targetPlace.clear();
// 			}




// 			//when it is corepoint
// 			if (ccount >= MinPts) {
// 				cur->isCorePoint = true;
// 				cur->countn = ccount;
// 				//points[i]->neighbor = &this_neighbor[0]; 
// 				cur->neighbor = new Point * [ccount];
// 				for (int k = 0; k < ccount; k++) {
// 					//points[index]->neighbor[t] = this_neighbor[t];
// 					cur->neighbor[k] = SumField[k];
// 				}
// 			}

// 			vector<Point*>().swap(SumField);
// 			SumField.clear();
// 		}


// 		vector<int>().swap(E_neighbor);
// 		E_neighbor.clear();
// 	}


// 	if (outliers) {
// 		double sqr_b = (r + Eps) * (r + Eps);
// 		double sqr_bd = 4 * (r + Eps) * (r + Eps);
// 		int epsize = E2P[E_length].size();
// 		int curind;
// 		int eind;
// 		for (int i = 0; i < epsize; i++) {
// 			ccount = 0;
// 			curind = E2P[E_length][i];
// 			cur = points[curind];
// 			eind = PtoE_index[curind];
// 			for (int j = 0; j < E_length; ++j) {
// 				if(query_distE(dist_E, eind, j, E_length) <= sqr_bd){
// 					if (square_dist(points, curind, E[j], d) <= sqr_b) {
// 						E_neighbor.emplace_back(j);
// 					}
// 				}				
// 			}
// 			E_neighbor.emplace_back(E_length);

// 			for (vector<int>::iterator iter = E_neighbor.begin(); iter != E_neighbor.end(); ++iter) {
// 				rtree[*iter].rangeQuery(cur, sqr_eps, targetPlace);
// 				ccount += targetPlace.size();
// 				SumField.reserve(ccount);
// 				SumField.insert(SumField.end(), targetPlace.begin(), targetPlace.end());
// 				vector<Point*>().swap(targetPlace);
// 				targetPlace.clear();
// 			}




// 			//when it is corepoint
// 			if (ccount >= MinPts) {
// 				cur->isCorePoint = true;
// 				cur->countn = ccount;
// 				//points[i]->neighbor = &this_neighbor[0]; 
// 				cur->neighbor = new Point * [ccount];
// 				for (int k = 0; k < ccount; k++) {
// 					//points[index]->neighbor[t] = this_neighbor[t];
// 					cur->neighbor[k] = SumField[k];
// 				}
// 			}

// 			vector<Point*>().swap(SumField);
// 			SumField.clear();
// 			vector<int>().swap(E_neighbor);
// 			E_neighbor.clear();
// 		}
// 	}


// 	for (int i = 0; i < E_length + 1; ++i) {
// 		rtree[i].releaseSpace();
// 	}


// 	//cout <<"rangequery time\t"<< rq / (double)CLOCKS_PER_SEC << endl;	



// 	//release memory
// 	delete[] dist_E;
// 	dist_E = NULL;
// 	for (int i = 0; i < E_length; i++) {
// 		//EtoP[i].clear();
// 		//vector<int>().swap(EtoP[i]);
// 		vector<int>().swap(E2P[i]);
// 	}
// 	vector<int>().swap(E2P[E_length]);
// 	//delete[] EtoP;
// 	delete[] E2P;
// 	//EtoP = NULL;
// 	E2P = NULL;
// 	delete[] PtoE_index;
// }

//聚类
void DBSCAN(vector<Point*>& points, int n) {
	//bool* visited = new bool[n];
	vector<Point*> local;
	int newlabel = 1;
	//int res;
	//int it;
	//memset(visited, false, n);

	//bool* added = new bool[n];
	//memset(added, false, n);

	for (int i = 0; i < n; ++i) {
		if (points[i]->visited) {
			continue;
		}
		if (points[i]->isCorePoint) {
			points[i]->clustringID = newlabel;
			points[i]->visited = true;
			local.emplace_back(points[i]);
			//added[i] = true;

			for (int j = 0; j < local.size(); ++j) {
				//res = local[j];

				for (int t = 0; t < local[j]->countn; ++t) {
					//local[j]->neighbor[t];

					if (!local[j]->neighbor[t]->visited) {
						local[j]->neighbor[t]->visited = true;
						local[j]->neighbor[t]->clustringID = newlabel;

						//if (points[it]->isCorePoint && !added[it]) {
						if (local[j]->neighbor[t]->isCorePoint) {
							local.emplace_back(local[j]->neighbor[t]);
							//added[it] = true;
						}
					}
				}
			}
			newlabel++;
			vector<Point* >().swap(local);
			local.clear();
		}
	}cata = newlabel-1;
	//delete[] visited;
	//visited = NULL;

	//delete[] added;
	//added = NULL;
}