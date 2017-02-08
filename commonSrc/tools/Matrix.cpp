#include "Matrix.h"

void Matrix_::ReadFile(string filename)
{
	FILE *fp;
	if ((fp = fopen(filename.c_str(), "r")) == NULL)
	{
		cerr << "No Input File!";
		exit(-1);
	}

	fscanf(fp, "%d", &rows);
	fscanf(fp, "%d", &cols);

	REAL ele;
	vector<REAL> t;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			fscanf(fp, READTYEP, &ele);
			t.push_back(ele);
		}
		data.push_back(t);
		t.clear();
	}
	fclose(fp);
}

void Matrix_::Init(string filename)
{
	FILE *fp;
	if ((fp = fopen(filename.c_str(), "r")) == NULL)
	{
		cerr << "No Input File!";
		exit(-1);
	}

	fscanf(fp, "%d", &rows);
	fscanf(fp, "%d", &cols);

	data.assign(rows, vector<REAL>(cols, 0));

	REAL ele;
	//vector<REAL> t;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			fscanf(fp, READTYEP, &ele);
			data[i][j] = ele;
		}

	}
	fclose(fp);
}

void Matrix_::WriteFile(std::string fname){
	ifstream fexist;
	ofstream fout;
	fexist.open(fname.c_str());
	if (fexist){
		cout << "Are you sure to OVERWRITE file [" << fname << "] of a new matrix?[y/N]" << endl;
		string exist;
		cin >> exist;
		while (1){
			if (exist == "N"){
				cout << "Refuse to overwrite" << endl;
				exit(0);
			}
			else if (exist == "y")
				break;
			else if (exist != "y"&&exist != "N")
				cout << "WRONG answer! Please enter again" << endl;
			else;
		}
	}
	fout.open(fname.c_str(), ios::out);
	fout.setf(ios::left);
	fout << rows << "    " << cols << endl;
	for (vector< vector<REAL> >::size_type i = 0; i != rows; i++){
		vector<REAL> rowvec = data[i];
		for (vector<REAL>::size_type j = 0; j != cols; j++){
			fout.width(14);
			fout << rowvec[j];
		}
		fout << endl;
	}
	fout.close();
}

void Matrix_::SetMatrix(int r, int c, REAL* p){
	if ((rows != r) || (cols != c))
	{
		rows = r;
		cols = c;
		data.assign(rows, vector<REAL>(cols, 0));
	}
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			data[i][j] = p[i*cols + j];
		}
	}
}

Matrix_ Matrix_::operator+(Matrix_ &ad) const{
	Matrix_ res(rows, cols);
	if (this->rows != ad.rows || this->cols != ad.cols){
		cerr << "Matrix dimensions must agree." << endl;
		exit(-1);
	}
	else{
		for (int i = 0; i<rows; i++)
		for (int j = 0; j<cols; j++)
			res.data[i][j] = this->data[i][j] + ad.data[i][j];
	}
	return res;
}

Matrix_ Matrix_::operator-(Matrix_ &su) const{
	Matrix_ res(rows, cols);
	if (this->rows != su.rows || this->cols != su.cols){
		cerr << "Matrix dimensions must agree." << endl;
		exit(-1);
	}
	else{
		for (int i = 0; i<rows; i++)
		for (int j = 0; j<cols; j++)
			res.data[i][j] = this->data[i][j] - su.data[i][j];
	}
	return res;
}

Matrix_ Matrix_::operator*(Matrix_ &mul) const{
	if (mul.rows != cols){
		cerr << "Matrix dimensions must agree." << endl;
		exit(-1);
	}
	Matrix_ res(rows, mul.Getcols());

	for (int i = 0; i < rows; i++)
	{
		for (int k = 0; k < cols; k++)
		{
			if (data[i][k] == 0) continue;
			for (int j = 0; j < mul.Getcols(); j++)
			{
				{
					res.data[i][j] += data[i][k] * mul.data[k][j];
				}
			}
		}
	}

	//for (int i = 0; i != rows; i++){
	//	for (int j = 0; j != mul.Getcols(); j++){
	//		for (int k = 0; k != cols; k++)
	//			res.data[i][j] = res.data[i][j] + data[i][k] * mul.data[k][j];
	//	}
	//}

	return res;
}

Matrix_ Matrix_::operator*(const REAL &mul) const{

	Matrix_ res(rows, cols);
	for (int i = 0; i != rows; i++){
		for (int j = 0; j != cols; j++){
			res.data[i][j] = this->data[i][j] * mul;
		}
	}
	return res;
}

Matrix_ Matrix_::operator/(const REAL &div)const{
	Matrix_ res(rows, cols);
	for (int i = 0; i<rows; i++)
	for (int j = 0; j<cols; j++)
		res.data[i][j] = this->data[i][j] / div;
	return res;
}

Matrix_ Matrix_::m_sqrt(){
	Matrix_ res(rows, cols);
	vector< vector<REAL> > data_t(rows, vector<REAL>(cols, 0));
	for (int i = 0; i<rows; i++)
	for (int j = 0; j<cols; j++)
		data_t[i][j] = sqrt(data[i][j]);
	res.setdata(data_t);
	return res;
}

Matrix_ Matrix_::trans(){
	Matrix_ res(cols, rows);
	vector<REAL> t;
	for (int i = 0; i != cols; i++)
	for (int j = 0; j != rows; j++)
		res.data[i][j] = this->data[j][i];
	return res;
}

vector<REAL> Matrix_::mean(int option){
	vector<REAL> res;
	switch (option){
	case 1:{
			   //Mean on rows
			   REAL s = 0;
			   for (int j = 0; j<cols; j++){
				   for (int i = 0; i<rows; i++)
					   s += this->data[i][j];
				   res.push_back(s / rows);
				   s = 0;
			   }
			   break;
	}
	case 2:{
			   //Mean on rows
			   REAL s = 0;
			   for (int j = 0; j<rows; j++){
				   for (int i = 0; i<cols; i++)
					   s += this->data[j][i];
				   res.push_back(s / cols);
				   s = 0;
			   }
			   break;
	}
	default:{
				cerr << "WRONG Input arguments" << endl;
				exit(-1);
	}
	}
	return res;
}

Matrix_ Matrix_::sort(vector<int>& id, int option){
	switch (option){
	case 1:{
			   //on rows
			   Matrix_ res(rows, cols);
			   if (id.size() != rows){
				   cerr << "Matrix dimensions must agree." << endl;
				   exit(-1);
			   }
			   for (vector<REAL>::size_type i = 0; i != id.size(); i++)
				   res.data[i] = data[id[i]];
			   return res;
	}
	case 2:{
			   //on columns
			   Matrix_ res = trans();
			   res = res.sort(id, 1);
			   res = res.trans();
			   return res;
	}
	default:{
				cerr << "WRONG Input arguments" << endl;
				exit(-1);
	}
	}

}

Matrix_ Matrix_::trunc(int num, int option){
	switch (option){
	case 1:{
			   //on rows
			   Matrix_ res(num, cols);
			   vector< vector<REAL> > d;
			   for (int i = 0; i<num; i++)
				   d.push_back(data[i]);
			   res.setdata(d);
			   return res;
	}
	case 2:{
			   //on column
			   Matrix_ res = trans();
			   res = res.trunc(num, 1);
			   res = res.trans();
			   return res;
	}
	default:{
				cerr << "WRONG Input arguments" << endl;
				exit(-1);
	}
	}

}

Matrix_ Matrix_::invert(){
	if (rows != cols){
		cerr << "Matrix dimensions must agree." << endl;
		exit(-1);
	}
	Matrix_ res(rows, rows);
	int actualsize = rows;
	int maxsize = rows;
	vector< vector<REAL> > data_t = data;
	for (int i = 1; i<actualsize; i++)
		data_t[0][i] /= data_t[0][0];
	for (int i = 1; i < actualsize; i++){
		for (int j = i; j < actualsize; j++){
			REAL sum = 0.0;
			for (int k = 0; k < i; k++)
				sum += data_t[j][k] * data_t[k][i];
			data_t[j][i] -= sum;
		}
		if (i == actualsize - 1)
			continue;
		for (int j = i + 1; j < actualsize; j++){
			REAL sum = 0.0;
			for (int k = 0; k < i; k++)
				sum += data_t[i][k] * data_t[k][j];
			data_t[i][j] = (data_t[i][j] - sum) / data_t[i][i];
		}
	}
	for (int i = 0; i < actualsize; i++)
	for (int j = i; j < actualsize; j++){
		REAL x = 1.0;
		if (i != j) {
			x = 0.0;
			for (int k = i; k < j; k++)
				x -= data_t[j][k] * data_t[k][i];
		}
		data_t[j][i] = x / data_t[j][j];
	}
	for (int i = 0; i < actualsize; i++)
	for (int j = i; j < actualsize; j++)  {
		if (i == j) continue;
		REAL sum = 0.0;
		for (int k = i; k < j; k++)
			sum += data_t[k][j] * ((i == k) ? 1.0 : data_t[i][k]);
		data_t[i][j] = -sum;
	}
	for (int i = 0; i < actualsize; i++)
	for (int j = 0; j < actualsize; j++)  {
		REAL sum = 0.0;
		for (int k = ((i>j) ? i : j); k < actualsize; k++)
			sum += ((j == k) ? 1.0 : data_t[j][k])*data_t[k][i];
		data_t[j][i] = sum;
	}
	res.setdata(data_t);
	return res;
}

void Matrix_::PrintMat()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			cout << data[i][j] << "	";
		}
		cout << endl;
	}
}

//struct ARG {
//	Matrix *A;
//	Matrix *B;
//	Matrix *C;
//	int cx, cy; //第一个元素的坐标
//};

//DWORD WINAPI Mul_Fun(LPVOID arg)
//{
//	struct ARG *p = (struct ARG *)arg;
//
//	Matrix *A = p->A;
//	Matrix *B = p->B;
//	Matrix *C = p->C;
//
//	for (int i = 0; i < A->rows; i++)
//	{
//		for (int j = p->cx; j < p->cy; j++)
//		{
//			C->data[i][j] = 0;
//		}
//	}
//
//	for (int i = 0; i < A->rows; i++)
//	{
//		for (int k = 0; k < B->rows; k++)
//		{
//			if (A->data[i][k] == 0) continue;
//			for (int j = p->cx; j < p->cy; j++)
//			{
//				{
//					C->data[i][j] += A->data[i][k] * B->data[k][j];
//				}
//			}
//		}-
//	}
//	return 0;
//}

void MatDiff(Matrix_ *A, Matrix_ *B, Matrix_ *C)
{
	if (A->rows != B->rows || A->cols != B->cols || A->rows != C->rows || A->cols != C->cols)
	{
		cerr << "Matrix dimensions must agree." << endl;
		exit(-1);
	}
	for (int i = 0; i < C->rows; i++)
	{
		for (int j = 0; j < C->cols; j++)
		{
			C->data[i][j] = A->data[i][j] - B->data[i][j];
		}
	}
	return;
}

void MatMul(Matrix_ *A, Matrix_ *B, Matrix_ *C)
{
	if ((C->rows != A->rows) || (C->cols != B->cols) || (A->cols != B->rows))
	{
		cerr << "Matrix dimensions must agree." << endl;
		exit(-1);
	}

	for (int i = 0; i < A->rows; i++)
	{
		for (int j = 0; j < C->cols; j++)
		{
			C->data[i][j] = 0;
		}
	}

	for (int i = 0; i < A->rows; i++)
	{
		for (int k = 0; k < B->rows; k++)
		{
			if (A->data[i][k] == 0) continue;
			for (int j = 0; j < C->cols; j++)
			{
				{
					C->data[i][j] += A->data[i][k] * B->data[k][j];
				}
			}
		}
	}
	return;
}

//void MatMul(Matrix *A, Matrix *B, Matrix *C, int thread_num)
//{
//	if (B->cols%thread_num != 0)
//	{
//		cerr << "ERROR!  thread_num must can be divisible by MatrixB.cols!" << endl;
//		exit(-1);
//	}
//	if ((C->rows != A->rows) || (C->cols != B->cols) || (A->cols != B->rows))
//	{
//		cerr << "Matrix dimensions must agree." << endl;
//		exit(-1);
//	}
//	struct ARG *args = new struct ARG[thread_num];
//	HANDLE *h = new HANDLE[thread_num];
//
//	for (int i = 0; i < thread_num; i++)
//	{
//		args[i].A = A;
//		args[i].B = B;
//		args[i].C = C;
//		args[i].cx = B->cols / thread_num*i;
//		args[i].cy = B->cols / thread_num*(i + 1);
//	}
//
//	for (int i = 0; i<thread_num; i++)
//	{
//		h[i] = CreateThread(NULL, 0, Mul_Fun, (LPVOID)(&args[i]), 0, 0);
//	}
//	WaitForMultipleObjects(thread_num, h, TRUE, INFINITE);
//	for (int i = 0; i < thread_num; i++)
//	{
//		CloseHandle(h[i]);
//	}
//	delete[] args;
//	delete[] h;
//
//	return;
//}

void MatDiv(Matrix_ *A, float B, Matrix_ *C)
{
	if (A->rows != C->rows || A->cols != C->cols)
	{
		cerr << "Matrix dimensions must agree." << endl;
		exit(-1);
	}
	for (int i = 0; i < C->rows; i++)
	{
		for (int j = 0; j < C->cols; j++)
		{
			C->data[i][j] = A->data[i][j] / B;
		}
	}
	return;
}

void MatNumMul(Matrix_ *A, float B, Matrix_ *C)
{
	if (A->rows != C->rows || A->cols != C->cols)
	{
		cerr << "Matrix dimensions must agree." << endl;
		exit(-1);
	}
	for (int i = 0; i < C->rows; i++)
	{
		for (int j = 0; j < C->cols; j++)
		{
			C->data[i][j] = A->data[i][j] * B;
		}
	}
	return;
}

unsigned short  Convolution(Matrix_ *A, Matrix_ *B, Matrix_ *C, Matrix_ *D)
{
	float SumF = 0,SumD=0,tmp;
	for (int i = 0; i < A->rows; i++)
	{
		for (int j = 0; j < A->cols; j++)
		{
			tmp = A->data[i][j] * B->data[i][j] * C->data[i][j];
			SumF += tmp;
			SumD += tmp*D->data[i][j];
		}
	}
	SumD /= SumF;
	//if (SumD > 0)
	//{
	//	cout << SumD << endl;
	//}
	return ((unsigned short)(SumD + 0.5));
}

void ROI55(Matrix_ *A, Matrix_ *B, int x, int y, int w, int h)
{
	if (B->rows != h || B->cols != w)
	{
		cerr << "Matrix dimensions must agree." << endl;
		exit(-1);
	}
	for (int i = y; i < h + y; i++)
	{
		for (int j = x; j < w + x; j++)
		{
			B->data[i - y][j - x] = A->data[i][j];
		}
	}
}

//
//int main()
//{
//	Matrix A(2, 4);
//	Matrix B(4, 4);
//	Matrix C(2, 4);
//
//	A.InitMat("A.txt");
//	B.InitMat("B.txt");
//
//
//	//for (int i = 0; i < A.rows; i++)
//	//{
//	//	for (int j = 0; j < A.cols; j++)
//	//	{
//	//		cout << A.data[i*A.cols + j] << "	";
//	//	}
//	//	cout << endl;
//	//}
//	//for (int i = 0; i < B.rows; i++)
//	//{
//	//	for (int j = 0; j < B.cols; j++)
//	//	{
//	//		cout << B.data[i*B.cols + j] << "	";
//	//	}
//	//	cout << endl;
//	//}
//	//for (int i = 0; i < C.rows; i++)
//	//{
//	//	for (int j = 0; j < C.cols; j++)
//	//	{
//	//		cout << C.data[i*C.cols + j] << "	";
//	//	}
//	//	cout << endl;
//	//}
//
//	MatMul(&A, &B, &C, 4);
//
//	for (int i = 0; i < C.rows; i++)
//	{
//		for (int j = 0; j < C.cols; j++)
//		{
//			cout << C.data[i*C.cols + j] << "	";
//		}
//		cout << endl;
//	}
//
//	cin.get();
//	return 0;
//}