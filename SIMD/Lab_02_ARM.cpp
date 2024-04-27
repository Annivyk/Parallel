//
//#include<iostream>
//#include<ctime>
//#include <cstdlib>
//#include<stdio.h>
//#include<time.h>
//#include<arm_neon.h>//Neon
//
//using namespace std;
//
////初始化矩阵数据
//void init(float**& data, int n) {
//	srand((int)time(0));
//	for (int i = 0; i < n; i++) {
//		for (int j = 0; j < n; j++) {
//			data[i][j] = rand() % 100;
//		}
//	}
//}
//float** normal_gauss(float** data, int n) {
//	for (int k = 0; k < n; k++) {
//		for (int j = k + 1; j < n; j++) {
//			data[k][j] = data[k][j] / data[k][k];
//		}
//		data[k][k] = 1.0;
//		for (int i = k + 1; i < n; i++) {
//			for (int j = k + 1; j < n; j++) {
//				data[i][j] = data[i][j] - data[i][k] * data[k][j];
//			}
//			data[i][k] = 0;
//		}
//	}
//	return data;
//}
//
////打印矩阵
//void printMatrix(float** data, int n) {
//	for (int i = 0; i < n; i++) {
//		for (int j = 0; j < n; j++) {
//			cout << data[i][j] << " ";
//		}
//		cout << endl;
//	}
//}
//
//
//
////普通高斯消去Neon并行
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
//
//
//
//int main() {
//	int n;//矩阵规模
//	cin >> n;
//	//总时间
//	
//
//	//标识优化策略
//	int flag = 0;
//
//	const int N_count = 5;
//	int count = N_count * 3;
//	while (count > 0) {
//		float** data = new float* [n];
//		for (int i = 0; i < n; ++i) {
//			data[i] = new float[n];
//		}
//		init(data, n);
//
//		//Linux精确测时
//		struct timespec sts, ets;
//		timespec_get(&sts, TIME_UTC);
//		normal_gauss(data, n);//串行算法
//		//sse_gauss(data, n);//sse算法
//		neon_gauss(data, n);//neon算法
//		//avx_gauss(data, n);//AVX算法
//		timespec_get(&ets, TIME_UTC);
//		time_t dsec = ets.tv_sec - sts.tv_sec;
//		long dnsec = ets.tv_nsec - sts.tv_nsec;
//		if (dnsec < 0) {
//			dsec--;
//			dnsec += 1000000000ll;
//		}
//		printf(" % lld. % 09llds\n", dsec, dnsec);
//
//		//释放动态内存
//		for (int i = 0; i < n; i++) {
//			delete[] data[i];
//		}
//		
//
//		delete[] data;
//		flag++;
//		flag %= 3;
//		count--;
//	}
//	cout << endl;
//	/*sum /= N_count * 3;
//	cout << sum * 1000.0 << "ms" << endl;*/
//	
//
//	return 0;
//}
