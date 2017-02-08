#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "windowsdef.h"
#include <iostream>

using namespace std;

#define USINGFLOAT
//#define USINGDOUBLE


#ifdef USINGFLOAT 
typedef float REAL;
#define MATDATATYPE CV_32F
#define READTYEP "%f"
#endif

#ifdef USINGDOUBLE
typedef double REAL;
#define MATDATATYPE CV_64F
#define READTYPE "%lf"
#endif

#include <vector>
#include <string>
#include <fstream>


using namespace std;

class Matrix_{
private:

public:
	int rows;
	int cols;
	vector< vector<REAL> > data;
	Matrix_(){
		rows = 0;
		cols = 0;
	}
	Matrix_(int r, int c){
		rows = r;
		cols = c;
		data.assign(rows, vector<REAL>(cols, 0));
	}
	Matrix_(int r, int c, REAL* p){
		rows = r;
		cols = c;
		data.assign(rows, vector<REAL>(cols, 0));
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				data[i][j] = p[i*cols + j];
			}
		}
	}
	Matrix_(int r, int c, unsigned short* p) {
		rows = r;
		cols = c;
		data.assign(rows, vector<REAL>(cols, 0));
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				data[i][j] = (REAL)p[i*cols + j];
			}
		}
	}
	Matrix_(int r, int c, unsigned char* p) {
		rows = r;
		cols = c;
		data.assign(rows, vector<REAL>(cols, 0));
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				data[i][j] = (REAL)p[i*cols + j];
			}
		}
	}
	//Matrix(int r, int c, REAL arr){
	//	rows = r;
	//	cols = c;
	//	data.assign(rows, vector<REAL>(cols, 0));
	//	for (int i = 0; i < rows; i++)
	//	{
	//		for (int j = 0; j < cols; j++)
	//		{
	//			data[i][j] = arr[i*cols + j];
	//		}
	//	}
	//}
	void ReadFile(string fname);
	void Init(string filename);
	void WriteFile(string fname);
	void PrintMat();
	void SetMatrix(int r, int c, REAL* p);
	Matrix_ operator*(Matrix_ &mul) const;
	Matrix_ operator*(const REAL &mul) const;
	Matrix_ operator+(Matrix_ &ad) const;
	Matrix_ operator-(Matrix_ &su) const;
	Matrix_ operator/(const REAL &div) const;
	Matrix_ trans();
	Matrix_ m_sqrt();
	vector<REAL> mean(int option);
	Matrix_ sort(vector<int>& id, int option);
	Matrix_ trunc(int num, int option);
	Matrix_ invert();
	Matrix_ copy(){
		Matrix_ res(rows, cols);
		res.setdata(data);
		return res;
	}
	Matrix_ ROI(int x, int y, int w, int h)
	{
		Matrix_ ROI(h, w);
		for (int i = y; i < y + h; i++)
		{
			for (int j = x; j < x + w; j++)
			{
				ROI.data[i - y][j - x] = data[i][j];
			}
		}
		return(ROI);

	}
	REAL &operator()(int x, int y){
		return data[x][y];
	}
	const REAL &operator()(int x, int y) const{
		return data[x][y];
	}
	int Getrows() const{
		return this->rows;
	}
	int Getcols() const{
		return this->cols;
	}
	void setdata(vector< vector<REAL> > d){
		if (data.size() != d.size()){
			cout << "Matrix dimensions must agree." << endl;
			exit(-1);
		}
		data.assign(d.begin(), d.end());
	}
};

inline Matrix_ zeros(int r, int c){
	Matrix_ res(r, c);
	return res;
}
inline Matrix_ ones(int r, int c){
	Matrix_ res(r, c);
	vector< vector<REAL> > data_t(r, vector<REAL>(c, 1));
	res.setdata(data_t);
	return res;
}
inline Matrix_ diag(vector<REAL> d){
	int n = d.size();
	Matrix_ res(n, n);
	vector< vector<REAL> > data_t(n, vector<REAL>(n, 0));
	for (int i = 0; i<n; i++)
		data_t[i][i] = d[i];
	res.setdata(data_t);
	return res;
}

void MatMul(Matrix_ *A, Matrix_ *B, Matrix_ *C);
//void MatMul(Matrix *A, Matrix *B, Matrix *C, int thread_num);
void MatDiff(Matrix_ *A, Matrix_ *B, Matrix_ *C);
void MatDiv(Matrix_ *A, float B, Matrix_ *C);
void MatNumMul(Matrix_ *A, float B, Matrix_ *C);
unsigned short  Convolution(Matrix_ *A, Matrix_ *B, Matrix_ *C, Matrix_ *D);
void ROI55(Matrix_ *A, Matrix_ *B, int x, int y, int w, int h);