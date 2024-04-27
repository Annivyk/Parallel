
#include<iostream>
#include<ctime>
#include <cstdlib>
#include<stdio.h>
#include<windows.h>
#include<time.h>
//#include<arm_neon.h>//Neon
#include<xmmintrin.h>//SSE
#include<immintrin.h>//AVX、AVX2
using namespace std;

//初始化矩阵数据
void init(float**& data, int n) {
	srand((int)time(0));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			data[i][j] = rand() % 100;
		}
	}
}
void aligned_init(float**& data, int n) {
	srand((unsigned)time(0));
	data = new float* [n];
	for (int i = 0; i < n; i++) {
		data[i] = (float*)_aligned_malloc(n * sizeof(float), 16);
		for (int j = 0; j < n; j++) {
			data[i][j] = rand() % 100;
		}
	}
}
float** aligned_sse_gauss(float** data, int n) {
	__m128 t1, t2, t3, t4;

	for (int k = 0; k < n; k++) {
		float tmp[4] = { data[k][k], data[k][k], data[k][k], data[k][k] };
		t1 = _mm_load_ps(tmp); // 使用对齐加载

		for (int j = n - 4; j >= k; j -= 4) {
			t2 = _mm_load_ps(data[k] + j);
			t3 = _mm_div_ps(t2, t1); // SSE除法
			_mm_store_ps(data[k] + j, t3); // 使用对齐存储
		}

		// 对不能整除4的剩余部分处理
		if (k % 4 != (n % 4)) //处理不能被4整除的元素
		{
			for (int j = k; j % 4 != (n % 4); j++)
			{
				data[k][j] = data[k][j] / tmp[0];
			}
		}
		for (int j = (n % 4) - 1; j >= 0; j--)
		{
			data[k][j] = data[k][j] / tmp[0];
		}
		// 更新其他行
		for (int i = k + 1; i < n; i++) {
			float coeff = data[i][k];
			float coeff_array[4] = { coeff, coeff, coeff, coeff };
			t1 = _mm_load_ps(coeff_array);

			for (int j = n - 4; j > k; j -= 4) {
				t2 = _mm_load_ps(data[i] + j);
				t3 = _mm_load_ps(data[k] + j);
				t4 = _mm_sub_ps(t2, _mm_mul_ps(t1, t3)); // SSE减法
				_mm_store_ps(data[i] + j, t4);
			}

			for (int j = k + 1; j % 4 != (n % 4); j++)
			{
				data[i][j] = data[i][j] - data[i][k] * data[k][j];
			}

			data[i][k] = 0;
		}
	}
	return data;
}


//打印矩阵
void printMatrix(float** data, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << data[i][j] << " ";
		}
		cout << endl;
	}
}

//普通高斯消去串行算法
float** normal_gauss(float** data, int n) {
	for (int k = 0; k < n; k++) {
		for (int j = k + 1; j < n; j++) {
			data[k][j] = data[k][j] / data[k][k];
		}
		data[k][k] = 1.0;
		for (int i = k + 1; i < n; i++) {
			for (int j = k + 1; j < n; j++) {
				data[i][j] = data[i][j] - data[i][k] * data[k][j];
			}
			data[i][k] = 0;
		}
	}
	return data;
}

//普通高斯消去SSE并行
float** sse_gauss(float** data, int n) {
	__m128 t1, t2, t3, t4;

	for (int k = 0; k < n; k++)
	{
		//除法并行
		float tmp[4] = { data[k][k], data[k][k], data[k][k], data[k][k] };
		t1 = _mm_loadu_ps(tmp);
		for (int j = n - 4; j >= k; j -= 4) //从后向前每次取四个
		{
			t2 = _mm_loadu_ps(data[k] + j);
			t3 = _mm_div_ps(t2, t1);//除法
			_mm_storeu_ps(data[k] + j, t3);
		}
		//对剩下的余数进行处理
		if (k % 4 != (n % 4)) //处理不能被4整除的元素
		{
			for (int j = k; j % 4 != (n % 4); j++)
			{
				data[k][j] = data[k][j] / tmp[0];
			}
		}
		for (int j = (n % 4) - 1; j >= 0; j--)
		{
			data[k][j] = data[k][j] / tmp[0];
		}
		//减法并行
		for (int i = k + 1; i < n; i++)
		{
			float tmp[4] = { data[i][k], data[i][k], data[i][k], data[i][k] };
			t1 = _mm_loadu_ps(tmp);
			for (int j = n - 4; j > k; j -= 4)
			{
				t2 = _mm_loadu_ps(data[i] + j);
				t3 = _mm_loadu_ps(data[k] + j);
				t4 = _mm_sub_ps(t2, _mm_mul_ps(t1, t3)); //减法
				_mm_storeu_ps(data[i] + j, t4);
			}

			for (int j = k + 1; j % 4 != (n % 4); j++)
			{
				data[i][j] = data[i][j] - data[i][k] * data[k][j];
			}

			data[i][k] = 0;
		}
	}
	return data;
}
//普通高斯消去Neon并行
//float** neon_gauss(float** data, int n) {
//	float32x4_t t1, t2, t3, t4;
//
//	for (int k = 0; k < n; k++)
//	{
//		//除法并行
//		float tmp[4] = { data[k][k], data[k][k], data[k][k], data[k][k] };
//		t1 = vld1q_f32(tmp);
//		for (int j = n - 4; j >= k; j -= 4) //从后向前每次取四个
//		{
//			t2 =  vld1q_f32(data[k] + j);
//			t3 = vdivq_f32(t2, t1);//除法
//			vst1q_f32(data[k] + j, t3);
//		}
//
//		if (k % 4 != (n % 4)) //处理不能被4整除的元素
//		{
//			for (int j = k; j % 4 != (n % 4); j++)
//			{
//				data[k][j] = data[k][j] / tmp[0];
//			}
//		}
//
//		for (int j = (n % 4) - 1; j >= 0; j--)
//		{
//			data[k][j] = data[k][j] / tmp[0];
//		}
//
//
//		//减法并行
//		for (int i = k + 1; i < n; i++)
//		{
//			float tmp[4] = { data[i][k], data[i][k], data[i][k], data[i][k] };
//			t1 = vld1q_f32(tmp);
//			for (int j = n - 4; j > k; j -= 4)
//			{
//				t2 = vld1q_f32(data[i] + j);
//				t3 = vld1q_f32(data[k] + j);
//				t4 = vsubq_f32(t2, vmulq_f32(t1, t3)); //减法
//				vst1q_f32(data[i] + j, t4);
//			}
//
//			for (int j = k + 1; j % 4 != (n % 4); j++)
//			{
//				data[i][j] = data[i][j] - data[i][k] * data[k][j];
//			}
//
//			data[i][k] = 0;
//		}
//	}
//	return data;
//}

//普通高斯消去AVX并行
float** avx_gauss(float** data, int n) {
	__m256 t1, t2, t3, t4;

	for (int k = 0; k < n; k++)
	{
		//除法并行
		float tmp[8] = { data[k][k], data[k][k], data[k][k], data[k][k],data[k][k], data[k][k], data[k][k], data[k][k] };
		t1 = _mm256_loadu_ps(tmp);
		for (int j = n - 8; j >= k; j -= 8) //从后向前每次取八个
		{
			t2 = _mm256_loadu_ps(data[k] + j);
			t3 = _mm256_div_ps(t2, t1);//除法
			_mm256_storeu_ps(data[k] + j, t3);
		}

		if (k % 8 != (n % 8)) //处理不能被4整除的元素
		{
			for (int j = k; j % 8 != (n % 8); j++)
			{
				data[k][j] = data[k][j] / tmp[0];
			}
		}

		for (int j = (n % 8) - 1; j >= 0; j--)
		{
			data[k][j] = data[k][j] / tmp[0];
		}


		//减法并行
		for (int i = k + 1; i < n; i++)
		{
			float tmp[8] = { data[i][k], data[i][k], data[i][k], data[i][k], data[i][k], data[i][k], data[i][k], data[i][k] };
			t1 = _mm256_loadu_ps(tmp);
			for (int j = n - 8; j > k; j -= 8)
			{
				t2 = _mm256_loadu_ps(data[i] + j);
				t3 = _mm256_loadu_ps(data[k] + j);
				t4 = _mm256_sub_ps(t2, _mm256_mul_ps(t1, t3)); //减法
				_mm256_storeu_ps(data[i] + j, t4);
			}

			for (int j = k + 1; j % 8 != (n % 8); j++)
			{
				data[i][j] = data[i][j] - data[i][k] * data[k][j];
			}

			data[i][k] = 0;
		}
	}
	return data;
}

int main() {
	int n;//矩阵规模
	cin >> n;
	//总时间
	float sum_time[3] = { 0 };
	float sum = 0;
	string name[3] = {
		"normal",
		"sse",
		"avx"
	};

	//标识优化策略
	int flag = 0;

	const int N_count = 5;
	int count = N_count*3;
	while (count > 0) {
		float** data = new float* [n];
		for (int i = 0; i < n; ++i) {
			data[i] = new float[n];
		}
		init(data, n);
		/*float** data;
		aligned_init(data, n);*/

		//windows精确测时
		long  float head, tail, freq;//timers
		QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
		QueryPerformanceCounter((LARGE_INTEGER*)&head);
		//if(flag==0){
		//	normal_gauss(data, n);//串行算法
		//}
		 if (flag == 1) {
			sse_gauss(data, n);//sse算法
		}
		//neon_gauss(data, n);//neon算法
		//else if (flag == 2) {
		//	avx_gauss(data, n);//AVX算法
		//}
		//aligned_sse_gauss(data, n);

		QueryPerformanceCounter((LARGE_INTEGER*)&tail);
		cout << (tail - head) / freq << endl;
		//sum += (tail - head) / freq;
		sum_time[flag] += (tail - head)  / freq ;

		//释放动态内存
		for (int i = 0; i < n; i++) {
			delete[] data[i];
		}
		/*for (int i = 0; i < n; i++) {
			_aligned_free(data[i]);
		}*/
		
		delete[] data;
		flag++;
		flag %= 3;
		count--;
	}
	cout << endl;
	/*sum /= N_count * 3;
	cout << sum * 1000.0 << "ms" << endl;*/
	for (int i = 0; i < 3; i++) {
		sum_time[i] /= N_count;
		cout << name[i] << "time = " << sum_time[i] * 1000.0 << "ms" <<endl;
	}

	//return 0;
}
