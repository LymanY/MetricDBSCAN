#include "File_Op.h"



Point* readPointFromFile(FILE* fileHandle, int d) {

	int* coords = new int[d];
	for (int i = 0; i < d; ++i) {
		fscanf(fileHandle, "%d", &(coords[i]));
	}
	Point* pt = new Point(coords);
	return pt;
}


void readDatasetFromFile(char* filePath, vector<Point*>& pt, int n, int d) {

	FILE* file = fopen(filePath, "rt");
	pt.clear();
	//pt.reserve(n);
	pt.resize(n);
	for (int i = 0; i < n; ++i) {
		pt[i] = readPointFromFile(file, d);
	}
	fclose(file);
}


void writeClusterToFile(vector<Point*>& points, char* filePath, int n) {
	ofstream myout(filePath);
	for (int i = 0; i < n; ++i)
	{
		myout << points[i]->clustringID << endl;
	}
	myout.close();
}

