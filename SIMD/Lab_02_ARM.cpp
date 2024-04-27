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
////��ʼ����������
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
////��ӡ����
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
////��ͨ��˹��ȥNeon����
//float** neon_gauss(float** data, int n) {
//	float32x4_t t1, t2, t3, t4;
//
//	for (int k = 0; k < n; k++)
//	{
//		//��������
//		float tmp[4] = { data[k][k], data[k][k], data[k][k], data[k][k] };
//		t1 = vld1q_f32(tmp);
//		for (int j = n - 4; j >= k; j -= 4) //�Ӻ���ǰÿ��ȡ�ĸ�
//		{
//			t2 =  vld1q_f32(data[k] + j);
//			t3 = vdivq_f32(t2, t1);//����
//			vst1q_f32(data[k] + j, t3);
//		}
//
//		if (k % 4 != (n % 4)) //�����ܱ�4������Ԫ��
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
//		//��������
//		for (int i = k + 1; i < n; i++)
//		{
//			float tmp[4] = { data[i][k], data[i][k], data[i][k], data[i][k] };
//			t1 = vld1q_f32(tmp);
//			for (int j = n - 4; j > k; j -= 4)
//			{
//				t2 = vld1q_f32(data[i] + j);
//				t3 = vld1q_f32(data[k] + j);
//				t4 = vsubq_f32(t2, vmulq_f32(t1, t3)); //����
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
//	int n;//�����ģ
//	cin >> n;
//	//��ʱ��
//	
//
//	//��ʶ�Ż�����
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
//		//Linux��ȷ��ʱ
//		struct timespec sts, ets;
//		timespec_get(&sts, TIME_UTC);
//		normal_gauss(data, n);//�����㷨
//		//sse_gauss(data, n);//sse�㷨
//		neon_gauss(data, n);//neon�㷨
//		//avx_gauss(data, n);//AVX�㷨
//		timespec_get(&ets, TIME_UTC);
//		time_t dsec = ets.tv_sec - sts.tv_sec;
//		long dnsec = ets.tv_nsec - sts.tv_nsec;
//		if (dnsec < 0) {
//			dsec--;
//			dnsec += 1000000000ll;
//		}
//		printf(" % lld. % 09llds\n", dsec, dnsec);
//
//		//�ͷŶ�̬�ڴ�
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
