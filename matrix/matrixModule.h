double* multiply(double* m1, double* m2, int dim, double* result);
double* multiply_parallel(double* m1, double* m2, int dim, double* result, int threads);
//double** transpose(double** m, double** result);
//int* getMatrixSize(double** m, int* result);
double dot(double* v1, double* v2, int len);
double* transpose(double* m, int dim, double* result);
