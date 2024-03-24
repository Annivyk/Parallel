//#include<iostream>
//#include<windows.h>
//using namespace std;
//const int LOOP = 10;
//void setit(int n, int**& matrix, int*& a) {
//    for (int i = 0; i < n; i++) {
//        for (int j = 0; j < n; j++) {
//            matrix[i][j] = i * 5 + j * 6;
//        }
//    }
//    for (int i = 0; i < n; i++) {
//        a[i] = i * 2;
//    }
//}
//void normal(int n, int**& matrix, int*& a, int*& b) {
//    long long int begin, end, freq;
//    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
//    QueryPerformanceCounter((LARGE_INTEGER*)&begin);
//    for (int loop = 0; loop < LOOP; loop++) {
//        for (int i = 0; i < n; i++) {
//            for (int j = 0; j < n; j++) {
//                b[i] += a[i] * matrix[j][i];
//            }
//        }
//    }
//    QueryPerformanceCounter((LARGE_INTEGER*)&end);
//    cout << "normal:" << (end - begin) * 1000.0 / freq /LOOP << endl;
//}
//
//
//void uncoll(int n, int**& matrix, int*& a, int*& b) {
//    long long int begin, end, freq;
//    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
//    QueryPerformanceCounter((LARGE_INTEGER*)&begin);
//    for (int loop = 0; loop < LOOP; loop++) {
//        for (int j = 0; j < n; j += 10)
//        {
//            int tmp0 = 0, tmp1 = 0, tmp2 = 0, tmp3 = 0, tmp4 = 0, tmp5 = 0, tmp6 = 0, tmp7 = 0, tmp8 = 0, tmp9 = 0;
//            for (int i = 0; i < n; i++)
//            {
//                tmp0 += a[j + 0] * matrix[j + 0][i];
//                tmp1 += a[j + 1] * matrix[j + 1][i];
//                tmp2 += a[j + 2] * matrix[j + 2][i];
//                tmp3 += a[j + 3] * matrix[j + 3][i];
//                tmp4 += a[j + 4] * matrix[j + 4][i];
//                tmp5 += a[j + 5] * matrix[j + 5][i];
//                tmp6 += a[j + 6] * matrix[j + 6][i];
//                tmp6 += a[j + 6] * matrix[j + 6][i];
//                tmp7 += a[j + 7] * matrix[j + 7][i];
//                tmp8 += a[j + 8] * matrix[j + 8][i];
//                tmp9 += a[j + 9] * matrix[j + 9][i];
//            }
//            b[j + 0] = tmp0;
//            b[j + 1] = tmp1;
//            b[j + 2] = tmp2;
//            b[j + 3] = tmp3;
//            b[j + 4] = tmp4;
//            b[j + 5] = tmp5;
//            b[j + 6] = tmp6;
//            b[j + 7] = tmp7;
//            b[j + 8] = tmp8;
//            b[j + 9] = tmp9;
//        }
//    }
//    QueryPerformanceCounter((LARGE_INTEGER*)&end);
//    cout << "uncoll:" << (end - begin) * 1000.0 / freq/LOOP   << endl;
//}
//void cacheF(int n, int**& matrix, int*& a, int*& b) {
//    long long int begin, end, freq;
//    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
//    QueryPerformanceCounter((LARGE_INTEGER*)&begin);
//    for (int loop = 0; loop < LOOP; loop++) {
//        for (int i = 0; i < n; i++) {
//            for (int j = 0; j < n; j++) {
//                b[j] += matrix[i][j] * a[i];
//            }
//        }
//    }
//    QueryPerformanceCounter((LARGE_INTEGER*)&end);
//    cout << "cacheF:" << (end - begin) * 1000.0 / freq/LOOP  << endl;
//}
//
//int main() {
//    //Q1
//    //for (int n = 1000; n <= 10000; n += 1000) {
//    int n = 5000;
//        cout << endl;
//        cout << "n=" << n << endl;
//        int** A; A = new int* [n];
//        for (int i = 0; i < n; i++) {
//            A[i] = new int[n];
//        }
//        for (int i = 0; i < n; i++) {
//            for (int j = 0; j < n; j++) {
//                A[i][j] = 0;
//            }
//        }
//
//        int* a; a = new int[n];
//        setit(n, A, a);
//
//        int* c = new int[n];
//        normal(n, A, a, c);
//
//        int* d = new int[n];
//        cacheF(n, A, a, d);
//
//        int* e = new int[n];
//        uncoll(n, A, a, e);
//    //}
//        system("pause");
//    
//
//}
//
