//#include <pthread.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <windows.h>
//#include <iostream>
//#include <ctime>
//#include <xmmintrin.h> //SSE
//#include <emmintrin.h> //SSE2
//#include <pmmintrin.h> //SSE3
//#include <tmmintrin.h> //SSSE3
//#include <smmintrin.h> //SSE4.1
//#include <nmmintrin.h> //SSSE4.2
//#include <semaphore.h>
//using namespace std;
//
//#pragma comment(lib, "pthreadVC2.lib")
//
//
//
//int n = 3000;//数据规模
//float** m_data;//矩阵数据
//const int thread_count = 8;//线程数
//
////初始化矩阵数据
//void init() {
//	for (int i = 0; i < n; i++) {
//		m_data[i] = new float[n];
//	}
//	srand((int)time(0));
//	for (int i = 0; i < n; i++) {
//		for (int j = 0; j < n; j++) {
//			m_data[i][j] = rand() % 100;
//		}
//	}
//}
//
////打印矩阵
//void printMatrix() {
//	for (int i = 0; i < n; i++) {
//		for (int j = 0; j < n; j++) {
//			cout << m_data[i][j] << " ";
//		}
//		cout << endl;
//	}
//}
//
////普通高斯消去串行算法
//float** normal_Gauss() {
//	for (int k = 0; k < n; k++) {
//		for (int j = k + 1; j < n; j++) {
//			m_data[k][j] = m_data[k][j] / m_data[k][k];
//		}
//		m_data[k][k] = 1.0;
//		for (int i = k + 1; i < n; i++) {
//			for (int j = k + 1; j < n; j++) {
//				m_data[i][j] = m_data[i][j] - m_data[i][k] * m_data[k][j];
//			}
//			m_data[i][k] = 0;
//		}
//	}
//	return m_data;
//}
//
////动态专用
//typedef struct {
//	int k; //消去的轮次
//	int t_id; // 线程 id
//}threadParam_t;
//
////静态专用
//typedef struct {
//	int t_id;//线程id
//}threadParam_t2;
//
////动态多线程
//void* dynamic(void* param)
//{
//	threadParam_t* p = (threadParam_t*)param;
//	int k = p->k; //消去的轮次
//	int t_id = p->t_id; //线程编号
//
//	int q = (n - k - 1) / thread_count;//负责的任务量
//	int w = k + t_id * q + 1; //获取自己的首个计算任务
//	for (int i = w; i < w + q; i++)
//	{
//		for (int j = k + 1; j < n; ++j)
//		{
//			m_data[i][j] = m_data[i][j] - m_data[i][k] * m_data[k][j];
//		}
//		m_data[i][k] = 0;
//	}
//	pthread_exit(NULL);
//	return NULL;
//}
//
////test：动态多线程（列行分）
//void dynamic_test()
//{
//	for (int k = 0; k < n; k++)
//	{	//主线程做除法操作
//		for (int j = k + 1; j < n; j++)
//		{
//			m_data[k][j] = m_data[k][j] / m_data[k][k];
//		}
//		m_data[k][k] = 1.0;
//
//		//创建工作线程，进行消去操作
//		pthread_t* handles = new pthread_t[thread_count];// 创建对应的 Handle
//		threadParam_t* param = new threadParam_t[thread_count];// 创建对应的线程数据结构
//		//分配任务
//		for (int t_id = 0; t_id < thread_count; t_id++)
//		{
//			param[t_id].k = k;
//			param[t_id].t_id = t_id;
//		}
//		//创建线程
//		for (int t_id = 0; t_id < thread_count; t_id++)
//		{
//			pthread_create(&handles[t_id], NULL, dynamic, (void*)&param[t_id]);
//		}
//		for (int i = k + (n - k - 1) / (thread_count + 1) * thread_count + 1; i < n; i++)
//		{
//			for (int j = k + 1; j < n; ++j)
//			{
//				m_data[i][j] = m_data[i][j] - m_data[i][k] * m_data[k][j];
//			}
//			m_data[i][k] = 0;
//		}
//		//主线程挂起等待所有的工作线程完成此轮消去工作
//		for (int t_id = 0; t_id < thread_count; t_id++)
//		{
//			pthread_join(handles[t_id], NULL);
//		}
//	}
//}
//
////动态多线程
//void* dynamic_(void* param)
//{
//	threadParam_t* p = (threadParam_t*)param;
//	int k = p->k; //消去的轮次
//	int t_id = p->t_id; //线程编号
//
//	int q = (n - k - 1) / thread_count;//负责的任务量/一共多少行（列）
//	int w = k + t_id * q + 1; //获取自己的首个计算任务/第一行（列）
//	for (int i = k + 1; i < n; i++) {
//		for (int j = w; j < w + q; j++) {
//			m_data[i][j] = m_data[i][j] - m_data[i][k] * m_data[k][j];
//		}
//	}
//	pthread_exit(NULL);
//	return NULL;
//}
//
////test：动态多线程（列行分）
//void dynamic__test()
//{
//	for (int k = 0; k < n; k++)
//	{	//主线程做除法操作
//		for (int j = k + 1; j < n; j++)
//		{
//			m_data[k][j] = m_data[k][j] / m_data[k][k];
//		}
//		m_data[k][k] = 1.0;
//
//		//创建工作线程，进行消去操作
//		pthread_t* handles = new pthread_t[thread_count];// 创建对应的 Handle
//		threadParam_t* param = new threadParam_t[thread_count];// 创建对应的线程数据结构
//		//分配任务
//		for (int t_id = 0; t_id < thread_count; t_id++)
//		{
//			param[t_id].k = k;
//			param[t_id].t_id = t_id;
//		}
//		//创建线程
//		for (int t_id = 0; t_id < thread_count; t_id++)
//		{
//			pthread_create(&handles[t_id], NULL, dynamic, (void*)&param[t_id]);
//		}
//		for (int i = k + (n - k - 1) / (thread_count + 1) * thread_count + 1; i < n; i++)
//		{
//			for (int j = k + 1; j < n; ++j)
//			{
//				m_data[i][j] = m_data[i][j] - m_data[i][k] * m_data[k][j];
//			}
//			m_data[i][k] = 0;
//		}
//		//主线程挂起等待所有的工作线程完成此轮消去工作
//		for (int t_id = 0; t_id < thread_count; t_id++)
//		{
//			pthread_join(handles[t_id], NULL);
//		}
//	}
//}
//
////动态+SSE
//void* dynamic_sse(void* param) {
//	threadParam_t* p = (threadParam_t*)param;
//	int k = p->k; //消去的轮次
//	int t_id = p->t_id; //线程编号
//	int q = (n - k - 1) / thread_count;//负责的任务量
//	int w = k + t_id * q + 1; //获取自己的首个计算任务
//	__m128 t2, t3, t4, vx;
//	for (int i = w; i < w + q; i++)
//	{
//		t2 = _mm_set_ps(m_data[i][k], m_data[i][k], m_data[i][k], m_data[i][k]);
//		for (int j = k + 1; j + 4 < n; j = j + 4)
//		{
//			t3 = _mm_loadu_ps(m_data[k] + j);
//			t4 = _mm_loadu_ps(m_data[i] + j);
//			vx = _mm_mul_ps(t2, t3);
//			t4 = _mm_sub_ps(t4, vx);
//			_mm_store_ps(m_data[i] + j, t4);
//		}
//		for (int j = (n / 4) * 4; j < n; j++)
//		{
//			m_data[i][j] = m_data[i][j] - m_data[i][k] * m_data[k][j];
//		}
//		m_data[i][k] = 0;
//	}
//	pthread_exit(NULL);
//	return NULL;
//}
//
////test: 动态+SSE
//void dynamic_sse_test() {
//	for (int k = 0; k < n; k++)
//	{	//主线程做除法操作
//		__m128 t1, t0;
//		t0 = _mm_set_ps(m_data[k][k], m_data[k][k], m_data[k][k], m_data[k][k]);
//		for (int j = k + 1; j + 4 < n; j = j + 4)
//		{
//			t1 = _mm_loadu_ps(m_data[k] + j);
//			t1 = _mm_div_ps(t1, t0);
//			_mm_store_ps(m_data[k] + j, t1);
//		}
//		for (int j = (n / 4) * 4; j < n; j++)
//		{
//			m_data[k][j] = m_data[k][j] / m_data[k][k];
//		}
//		m_data[k][k] = float(1.0);
//		//创建工作线程，进行消去操作
//		pthread_t* handles = new pthread_t[thread_count];// 创建对应的 Handle
//		threadParam_t* param = new threadParam_t[thread_count];// 创建对应的线程数据结构
//		//分配任务
//		for (int t_id = 0; t_id < thread_count; t_id++)
//		{
//			param[t_id].k = k;
//			param[t_id].t_id = t_id;
//		}
//		//创建线程
//		for (int t_id = 0; t_id < thread_count; t_id++)
//		{
//			pthread_create(&handles[t_id], NULL, dynamic_sse, (void*)&param[t_id]);
//		}
//		__m128 t2, t3, t4, vx;
//		for (int i = k + (n - k - 1) / (thread_count + 1) * thread_count + 1; i < n; i++)
//		{
//			t2 = _mm_set_ps(m_data[i][k], m_data[i][k], m_data[i][k], m_data[i][k]);
//			for (int j = k + 1; j + 4 < n; j = j + 4)
//			{
//				t3 = _mm_loadu_ps(m_data[k] + j);
//				t4 = _mm_loadu_ps(m_data[i] + j);
//				vx = _mm_mul_ps(t2, t3);
//				t4 = _mm_sub_ps(t4, vx);
//				_mm_store_ps(m_data[i] + j, t4);
//			}
//			for (int j = (n / 4) * 4; j < n; j++)
//			{
//				m_data[i][j] = m_data[i][j] - m_data[i][k] * m_data[k][j];
//			}
//			m_data[i][k] = 0;
//		}
//		//主线程挂起等待所有的工作线程完成此轮消去工作
//		for (int t_id = 0; t_id < thread_count; t_id++)
//		{
//			pthread_join(handles[t_id], NULL);
//		}
//	}
//}
//
//
////静态+信号量同步	任务循环划分	垂直划分
////信号量定义
//sem_t sem_main_loop;
//sem_t sem_workerstart_loop[thread_count];//每个线程有自己专属的信号量
//sem_t sem_workerend_loop[thread_count];
////线程函数定义
//void* static_loop(void* param) {
//	threadParam_t2* p = (threadParam_t2*)param;
//	int t_id = p->t_id;
//	for (int k = 0; k < n; ++k) {
//		sem_wait(&sem_workerstart_loop[t_id]);//阻塞，等待主线完成除法操作（操作自己专属的信号量）
//		//循环任务划分
//		for (int i = k + 1 + t_id; i < n; i += thread_count) {
//			//消去
//			for (int j = k + 1; j < n; j++) {
//				m_data[i][j] = m_data[i][j] - m_data[i][k] * m_data[k][j];
//			}
//			m_data[i][k] = 0.0;
//		}
//		sem_post(&sem_main_loop);//唤醒主线程
//		sem_wait(&sem_workerend_loop[t_id]);//阻塞，等待主线程唤醒进入下一轮
//	}
//	pthread_exit(NULL);
//	return NULL;
//}
//
////test: 静态+信号量同步	任务循环划分	垂直划分
//void static_loop_test() {
//	//初始化信号量
//	sem_init(&sem_main_loop, 0, 0);
//	for (int i = 0; i < thread_count; ++i) {
//		sem_init(&sem_workerstart_loop[i], 0, 0);
//		sem_init(&sem_workerend_loop[i], 0, 0);
//	}
//	//创建线程
//	pthread_t handles[thread_count];
//	threadParam_t2 param[thread_count];
//	for (int t_id = 0; t_id < thread_count; t_id++) {
//		param[t_id].t_id = t_id;
//		pthread_create(&handles[t_id], NULL, static_loop, (void*)&param[t_id]);
//	}
//
//	for (int k = 0; k < n; k++) {
//		//主线程做除法操作
//		for (int j = k + 1; j < n; j++) {
//			m_data[k][j] = m_data[k][j] / m_data[k][k];
//		}
//		m_data[k][k] = 1.0;
//
//		//开始唤醒工作线程
//		for (int t_id = 0; t_id < thread_count; t_id++) {
//			sem_post(&sem_workerstart_loop[t_id]);
//		}
//
//		//主线程睡眠（等待所有的工作线程完成此轮消去任务）
//		for (int t_id = 0; t_id < thread_count; t_id++) {
//			sem_wait(&sem_main_loop);
//		}
//
//		//主线程再次唤醒工作线程进入下一轮的消去任务
//		for (int t_id = 0; t_id < thread_count; t_id++) {
//			sem_post(&sem_workerend_loop[t_id]);
//		}
//	}
//	for (int t_id = 0; t_id < thread_count; t_id++) {
//		pthread_join(handles[t_id], NULL);
//	}
//	//销毁所有信号量
//	sem_destroy(&sem_main_loop);
//	sem_destroy(&sem_workerstart_loop[thread_count]);
//	sem_destroy(&sem_workerend_loop[thread_count]);
//}
//
////静态+信号量同步	任务按块划分	水平划分
////信号量定义
//sem_t sem_main_block;
//sem_t sem_workerstart_block[thread_count];//每个线程有自己专属的信号量
//sem_t sem_workerend_block[thread_count];
////线程函数定义
//void* static_block(void* param) {
//	threadParam_t2* p = (threadParam_t2*)param;
//	int t_id = p->t_id;
//	for (int k = 0; k < n; ++k) {
//		sem_wait(&sem_workerstart_block[t_id]);//阻塞，等待主线完成除法操作（操作自己专属的信号量）
//		
//		//任务按块划分
//		int q = (n - k - 1) / (thread_count + 1);
//		int w = k + t_id * q + 1; //获取自己的计算任务
//		for (int i = w; i < w + q; i++)
//		{
//			for (int j = k + 1; j < n; ++j)
//			{
//				m_data[i][j] = m_data[i][j] - m_data[i][k] * m_data[k][j];
//			}
//			m_data[i][k] = 0;
//		}
//		sem_post(&sem_main_block);//唤醒主线程
//		sem_wait(&sem_workerend_block[t_id]);//阻塞，等待主线程唤醒进入下一轮
//	}
//	pthread_exit(NULL);
//	return NULL;
//}
//
////test: 静态+信号量同步	任务按块划分	水平划分
//void static_block_test() {
//	//初始化信号量
//	sem_init(&sem_main_block, 0, 0);
//	for (int i = 0; i < thread_count; ++i) {
//		sem_init(&sem_workerstart_block[i], 0, 0);
//		sem_init(&sem_workerend_block[i], 0, 0);
//	}
//	//创建线程
//	pthread_t handles[thread_count];
//	threadParam_t2 param[thread_count];
//	for (int t_id = 0; t_id < thread_count; t_id++) {
//		param[t_id].t_id = t_id;
//		pthread_create(&handles[t_id], NULL, static_block, (void*)&param[t_id]);
//	}
//
//	for (int k = 0; k < n; k++) {
//		//主线程做除法操作
//		for (int j = k + 1; j < n; j++) {
//			m_data[k][j] = m_data[k][j] / m_data[k][k];
//		}
//		m_data[k][k] = 1.0;
//
//		//开始唤醒工作线程
//		for (int t_id = 0; t_id < thread_count; t_id++) {
//			sem_post(&sem_workerstart_block[t_id]);
//		}
//		for (int i = k + (n - k - 1) / (thread_count + 1) * thread_count + 1; i < n; i++)
//		{
//			for (int j = k + 1; j < n; ++j)
//			{
//				m_data[i][j] = m_data[i][j] - m_data[i][k] * m_data[k][j];
//			}
//			m_data[i][k] = 0.0;
//		}
//
//		//主线程睡眠（等待所有的工作线程完成此轮消去任务）
//		for (int t_id = 0; t_id < thread_count; t_id++) {
//			sem_wait(&sem_main_block);
//		}
//
//		//主线程再次唤醒工作线程进入下一轮的消去任务
//		for (int t_id = 0; t_id < thread_count; t_id++) {
//			sem_post(&sem_workerend_block[t_id]);
//		}
//	}
//	for (int t_id = 0; t_id < thread_count; t_id++) {
//		pthread_join(handles[t_id], NULL);
//	}
//	//销毁所有信号量
//	sem_destroy(&sem_main_block);
//	sem_destroy(&sem_workerstart_block[thread_count]);
//	sem_destroy(&sem_workerend_block[thread_count]);
//}
//
////静态+信号量同步	任务按块划分	垂直划分
////信号量定义
//sem_t sem_main_block3;
//sem_t sem_workerstart_block3[thread_count];//每个线程有自己专属的信号量
//sem_t sem_workerend_block3[thread_count];
////线程函数定义
//void* static_v_block(void* param) {
//	threadParam_t2* p = (threadParam_t2*)param;
//	int t_id = p->t_id;
//	for (int k = 0; k < n; ++k) {
//		sem_wait(&sem_workerstart_block3[t_id]);//阻塞，等待主线完成除法操作（操作自己专属的信号量）
//
//		//任务按块划分
//		int q = (n - k - 1) / (thread_count + 1);
//		int w = k + t_id * q + 1; //获取自己的计算任务
//		//for (int i = w; i < w + q; i++)
//		//{
//		//	for (int j = k + 1; j < n; ++j)
//		//	{
//		//		m_data[i][j] = m_data[i][j] - m_data[i][k] * m_data[k][j];
//		//	}
//		//	m_data[i][k] = 0;
//		//}
//
//		for (int i = k + 1; i < n; i++) {
//			//消去
//			for (int j = w; j < w + q;j++) {
//				m_data[i][j] = m_data[i][j] - m_data[i][k] * m_data[k][j];
//			}
//			m_data[i][k] = 0.0;
//		}
//
//		sem_post(&sem_main_block3);//唤醒主线程
//		sem_wait(&sem_workerend_block3[t_id]);//阻塞，等待主线程唤醒进入下一轮
//	}
//	pthread_exit(NULL);
//	return NULL;
//}
//
////test: 静态+信号量同步	任务按块划分	垂直划分
//void static_block_v_test() {
//	//初始化信号量
//	sem_init(&sem_main_block, 0, 0);
//	for (int i = 0; i < thread_count; ++i) {
//		sem_init(&sem_workerstart_block3[i], 0, 0);
//		sem_init(&sem_workerend_block3[i], 0, 0);
//	}
//	//创建线程
//	pthread_t handles[thread_count];
//	threadParam_t2 param[thread_count];
//	for (int t_id = 0; t_id < thread_count; t_id++) {
//		param[t_id].t_id = t_id;
//		pthread_create(&handles[t_id], NULL, static_v_block, (void*)&param[t_id]);
//	}
//
//	for (int k = 0; k < n; k++) {
//		//主线程做除法操作
//		for (int j = k + 1; j < n; j++) {
//			m_data[k][j] = m_data[k][j] / m_data[k][k];
//		}
//		m_data[k][k] = 1.0;
//
//		//开始唤醒工作线程
//		for (int t_id = 0; t_id < thread_count; t_id++) {
//			sem_post(&sem_workerstart_block3[t_id]);
//		}
//		for (int i = k+1; i < n; i++)
//		{
//			//int i = k + (n - k - 1) / (thread_count + 1) * thread_count + 1; i < n; i++
//			for (int j = k + (n - k - 1) / (thread_count + 1) * thread_count + 1; j < n; ++j)
//			{
//				m_data[i][j] = m_data[i][j] - m_data[i][k] * m_data[k][j];
//			}
//			m_data[i][k] = 0.0;
//		}
//
//		//主线程睡眠（等待所有的工作线程完成此轮消去任务）
//		for (int t_id = 0; t_id < thread_count; t_id++) {
//			sem_wait(&sem_main_block3);
//		}
//
//		//主线程再次唤醒工作线程进入下一轮的消去任务
//		for (int t_id = 0; t_id < thread_count; t_id++) {
//			sem_post(&sem_workerend_block3[t_id]);
//		}
//	}
//	for (int t_id = 0; t_id < thread_count; t_id++) {
//		pthread_join(handles[t_id], NULL);
//	}
//	//销毁所有信号量
//	sem_destroy(&sem_main_block3);
//	sem_destroy(&sem_workerstart_block3[thread_count]);
//	sem_destroy(&sem_workerend_block3[thread_count]);
//}
//
////静态+信号量同步	任务循环划分	水平划分
////信号量定义
//sem_t sem_main_loop2;
//sem_t sem_workerstart_loop2[thread_count];//每个线程有自己专属的信号量
//sem_t sem_workerend_loop2[thread_count];
////线程函数定义
//void* static_loop2(void* param) {
//	threadParam_t2* p = (threadParam_t2*)param;
//	int t_id = p->t_id;
//	for (int k = 0; k < n; ++k) {
//		sem_wait(&sem_workerstart_loop2[t_id]);//阻塞，等待主线完成除法操作（操作自己专属的信号量）
//
//		//循环任务划分	水平划分
//		for (int i = k + 1; i < n; i++) {
//			//消去
//			for (int j = k + 1 + t_id; j < n; j += thread_count) {
//				m_data[i][j] = m_data[i][j] - m_data[i][k] * m_data[k][j];
//			}
//			m_data[i][k] = 0.0;
//		}
//		sem_post(&sem_main_loop2);//唤醒主线程
//		sem_wait(&sem_workerend_loop2[t_id]);//阻塞，等待主线程唤醒进入下一轮
//	}
//	pthread_exit(NULL);
//	return NULL;
//}
//
////test: 静态+信号量同步	任务循环划分	水平划分
//void static_loop_test2() {
//	//初始化信号量
//	sem_init(&sem_main_loop2, 0, 0);
//	for (int i = 0; i < thread_count; ++i) {
//		sem_init(&sem_workerstart_loop2[i], 0, 0);
//		sem_init(&sem_workerend_loop2[i], 0, 0);
//	}
//	//创建线程
//	pthread_t handles[thread_count];
//	threadParam_t2 param[thread_count];
//	for (int t_id = 0; t_id < thread_count; t_id++) {
//		param[t_id].t_id = t_id;
//		pthread_create(&handles[t_id], NULL, static_loop2, (void*)&param[t_id]);
//	}
//
//	for (int k = 0; k < n; k++) {
//		//主线程做除法操作
//		for (int j = k + 1; j < n; j++) {
//			m_data[k][j] = m_data[k][j] / m_data[k][k];
//		}
//		m_data[k][k] = 1.0;
//
//		//开始唤醒工作线程
//		for (int t_id = 0; t_id < thread_count; t_id++) {
//			sem_post(&sem_workerstart_loop2[t_id]);
//		}
//
//		//主线程睡眠（等待所有的工作线程完成此轮消去任务）
//		for (int t_id = 0; t_id < thread_count; t_id++) {
//			sem_wait(&sem_main_loop2);
//		}
//
//		//主线程再次唤醒工作线程进入下一轮的消去任务
//		for (int t_id = 0; t_id < thread_count; t_id++) {
//			sem_post(&sem_workerend_loop2[t_id]);
//		}
//	}
//	for (int t_id = 0; t_id < thread_count; t_id++) {
//		pthread_join(handles[t_id], NULL);
//	}
//	//销毁所有信号量
//	sem_destroy(&sem_main_loop2);
//	sem_destroy(&sem_workerstart_loop2[thread_count]);
//	sem_destroy(&sem_workerend_loop2[thread_count]);
//}
//
//
////静态+信号量同步+SSE	任务按块划分	水平划分
////信号量定义
//sem_t sem_main_block2;
//sem_t sem_workerstart_block2[thread_count];//每个线程有自己专属的信号量
//sem_t sem_workerend_block2[thread_count];
////线程函数定义
//void* static_block2(void* param) {
//	threadParam_t2* p = (threadParam_t2*)param;
//	int t_id = p->t_id;
//	for (int k = 0; k < n; ++k) {
//		sem_wait(&sem_workerstart_block2[t_id]);//阻塞，等待主线完成除法操作（操作自己专属的信号量）
//
//		//任务按块划分
//		int q = (n - k - 1) / (thread_count + 1);
//		int w = k + t_id * q + 1; //获取自己的计算任务
//		__m128 t2, t3, t4, vx;
//		for (int i = w; i < w + q; i++)
//		{
//			t2 = _mm_set_ps(m_data[i][k], m_data[i][k], m_data[i][k], m_data[i][k]);
//			for (int j = k + 1; j + 4 < n; j = j + 4)
//			{
//				t3 = _mm_loadu_ps(m_data[k] + j);
//				t4 = _mm_loadu_ps(m_data[i] + j);
//				vx = _mm_mul_ps(t2, t3);
//				t4 = _mm_sub_ps(t4, vx);
//				_mm_store_ps(m_data[i] + j, t4);
//			}
//			for (int j = (n / 4) * 4; j < n; j++)
//			{
//				m_data[i][j] = m_data[i][j] - m_data[i][k] * m_data[k][j];
//			}
//			m_data[i][k] = 0;
//		}
//		sem_post(&sem_main_block2);//唤醒主线程
//		sem_wait(&sem_workerend_block2[t_id]);//阻塞，等待主线程唤醒进入下一轮
//	}
//	pthread_exit(NULL);
//	return NULL;
//}
//
////test: 静态+信号量同步+SSE 	任务按块划分	水平划分
//void static_block_sse_test() {
//	//初始化信号量
//	sem_init(&sem_main_block2, 0, 0);
//	for (int i = 0; i < thread_count; ++i) {
//		sem_init(&sem_workerstart_block2[i], 0, 0);
//		sem_init(&sem_workerend_block2[i], 0, 0);
//	}
//	//创建线程
//	pthread_t handles[thread_count];
//	threadParam_t2 param[thread_count];
//	for (int t_id = 0; t_id < thread_count; t_id++) {
//		param[t_id].t_id = t_id;
//		pthread_create(&handles[t_id], NULL, static_block2, (void*)&param[t_id]);
//	}
//
//	for (int k = 0; k < n; k++) {
//		//主线程做除法操作
//		for (int j = k + 1; j < n; j++) {
//			m_data[k][j] = m_data[k][j] / m_data[k][k];
//		}
//		m_data[k][k] = 1.0;
//
//		//开始唤醒工作线程
//		for (int t_id = 0; t_id < thread_count; t_id++) {
//			sem_post(&sem_workerstart_block2[t_id]);
//		}
//		for (int i = k + (n - k - 1) / (thread_count + 1) * thread_count + 1; i < n; i++)
//		{
//			for (int j = k + 1; j < n; ++j)
//			{
//				m_data[i][j] = m_data[i][j] - m_data[i][k] * m_data[k][j];
//			}
//			m_data[i][k] = 0.0;
//		}
//
//		//主线程睡眠（等待所有的工作线程完成此轮消去任务）
//		for (int t_id = 0; t_id < thread_count; t_id++) {
//			sem_wait(&sem_main_block2);
//		}
//
//		//主线程再次唤醒工作线程进入下一轮的消去任务
//		for (int t_id = 0; t_id < thread_count; t_id++) {
//			sem_post(&sem_workerend_block2[t_id]);
//		}
//	}
//	for (int t_id = 0; t_id < thread_count; t_id++) {
//		pthread_join(handles[t_id], NULL);
//	}
//	//销毁所有信号量
//	sem_destroy(&sem_main_block2);
//	sem_destroy(&sem_workerstart_block2[thread_count]);
//	sem_destroy(&sem_workerend_block2[thread_count]);
//}
//
////静态+信号量同步+三重循环全部纳入线程函数	循环划分(改为按块划分)	水平划分
////信号量定义
//sem_t sem_leader;
//sem_t sem_Division[thread_count - 1];
//sem_t sem_Elimiination[thread_count - 1];
////线程函数定义
//void* static_loop3(void* param) {
//	threadParam_t2* p = (threadParam_t2*)param;
//	int t_id = p->t_id;
//	for (int k = 0; k < n; k++) {
//		//t_id为0的线程除法操作，其他线程先等待
//		if (t_id == 0) {
//			for (int j = k + 1; j < n; j++) {
//				m_data[k][j] = m_data[k][j] / m_data[k][k];
//			}
//			m_data[k][k] = 1.0;
//		}
//		else
//			sem_wait(&sem_Division[t_id - 1]);//阻塞，等待完成除法操作
//		//t_id 为0的线程唤醒其他工作线程，进行消去操作
//		if (t_id == 0) {
//			for (int i = 0; i < thread_count - 1; i++) {
//				sem_post(&sem_Division[i]);
//			}
//		}
//		int q = (n - k - 1) / (thread_count + 1);
//		int w = k + t_id * q + 1; //获取自己的计算任务
//		//循环任务划分
//		//for (int i = k + 1 + t_id; i < n; i += thread_count) {
//		//	//消去
//		//	for (int j = k + 1; j < n; j++) {
//		//		m_data[i][j] = m_data[i][j] - m_data[i][k] * m_data[k][j];
//		//	}
//		//	m_data[i][k] = 0.0;
//		//}
//		for (int i = w; i < w + q; i++) {
//			for (int j = k + 1; j < n; j++) {
//				m_data[i][j] = m_data[i][j] - m_data[i][k] * m_data[k][j];
//			}
//			m_data[i][k] = 0.0;
//		}
//
//		if (t_id == 0) {
//			for (int i = 0; i < thread_count - 1; ++i) {
//				sem_wait(&sem_leader);//等待其他worker完成消去
//			}
//
//			for (int i = 0; i < thread_count - 1; i++) {
//				sem_post(&sem_Elimiination[i]);//通知其他worker进入下一轮
//			}
//		}
//		else {
//			sem_post(&sem_leader);
//			sem_wait(&sem_Elimiination[t_id - 1]);
//		}
//	}
//	pthread_exit(NULL);
//	return NULL;
//}
//
////test: 静态+信号量同步+三重循环全部纳入线程函数  循环划分  水平划分
//void static_loop_test3() {
//	sem_init(&sem_leader, 0, 0);
//	for (int i = 0; i < thread_count - 1; i++) {
//		sem_init(&sem_Division[i], 0, 0);
//		sem_init(&sem_Elimiination[i], 0, 0);
//	}
//	//创建线程
//	pthread_t handles[thread_count];
//	threadParam_t2 param[thread_count];
//	for (int t_id = 0; t_id < thread_count; t_id++) {
//		param[t_id].t_id = t_id;
//		pthread_create(&handles[t_id], NULL, static_loop3, (void*)&param[t_id]);
//	}
//
//	for (int t_id = 0; t_id < thread_count; t_id++) {
//		pthread_join(handles[t_id], NULL);
//	}
//	//销毁信号量
//	sem_destroy(&sem_leader);
//	sem_destroy(&sem_Division[thread_count - 1]);
//	sem_destroy(&sem_Elimiination[thread_count - 1]);
//}
//
////静态+barrier同步	循环划分（块）  垂直划分（水平）
////barrier定义
//pthread_barrier_t barrier_Divsion;
//pthread_barrier_t barrier_Elimination;
////线程函数定义
//void* static_barrier_loop(void* param) {
//	threadParam_t2* p = (threadParam_t2*)param;
//	int t_id = p->t_id;
//	for (int k = 0; k < n; k++) {
//		//t_id为0的线程除法操作，其他线程先等待
//		if (t_id == 0) {
//			for (int j = k + 1; j < n; j++) {
//				m_data[k][j] = m_data[k][j] / m_data[k][k];
//			}
//			m_data[k][k] = 1.0;
//		}
//
//		//第一个同步点
//		pthread_barrier_wait(&barrier_Divsion);
//
//		//任务按块划分
//		int q = (n - k - 1) / (thread_count + 1);
//		int w = k + t_id * q + 1; //获取自己的计算任务
//		__m128 t2, t3, t4, vx;
//		for (int i = w; i < w + q; i++)
//		{
//			t2 = _mm_set_ps(m_data[i][k], m_data[i][k], m_data[i][k], m_data[i][k]);
//			for (int j = k + 1; j + 4 < n; j = j + 4)
//			{
//				t3 = _mm_loadu_ps(m_data[k] + j);
//				t4 = _mm_loadu_ps(m_data[i] + j);
//				vx = _mm_mul_ps(t2, t3);
//				t4 = _mm_sub_ps(t4, vx);
//				_mm_store_ps(m_data[i] + j, t4);
//			}
//			for (int j = (n / 4) * 4; j < n; j++)
//			{
//				m_data[i][j] = m_data[i][j] - m_data[i][k] * m_data[k][j];
//			}
//			m_data[i][k] = 0;
//		}
//		//循环任务划分
//		//for (int i = k + 1 + t_id; i < n; i += thread_count) {
//		//	//消去
//		//	for (int j = k + 1; j < n; j++) {
//		//		m_data[i][j] = m_data[i][j] - m_data[i][k] * m_data[k][j];
//		//	}
//		//	m_data[i][k] = 0.0;
//		//}
//
//		//第二个同步点
//		pthread_barrier_wait(&barrier_Elimination);
//	}
//	pthread_exit(NULL);
//	return NULL;
//}
//
////test: 静态+barrier同步	循环划分（块）  垂直划分（水平）
//void static_barrier_loop_test() {
//	pthread_barrier_init(&barrier_Divsion, NULL, thread_count);
//	pthread_barrier_init(&barrier_Elimination, NULL, thread_count);
//	//创建线程
//	pthread_t handles[thread_count];
//	threadParam_t2 param[thread_count];
//	for (int t_id = 0; t_id < thread_count; t_id++) {
//		param[t_id].t_id = t_id;
//		pthread_create(&handles[t_id], NULL, static_barrier_loop, (void*)&param[t_id]);
//	}
//
//	for (int t_id = 0; t_id < thread_count; t_id++) {
//		pthread_join(handles[t_id], NULL);
//	}
//
//	//销毁barrier
//	pthread_barrier_destroy(&barrier_Divsion);
//	pthread_barrier_destroy(&barrier_Elimination);
//}
//
//int main() {
//
//	int m[9] = { 100,200,300,400,500,1000,2000,3000,4000 };
//
//
//
//	for (int i = 0; i < 1; i++) {
//		n = 1000;
//
//		m_data = new float* [n];//矩阵数据
//		init();
//		//printMatrix();
//		long long head, tail, freq;
//		QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
//		QueryPerformanceCounter((LARGE_INTEGER*)&head);
//
//		//normal_Gauss();//串行算法
//		// 
//		//switch (i) {
//		//case 0:
//		//	dynamic_test();//动态线程（水平划分）
//		//	break;
//		//case 1:
//		//	dynamic_sse_test();//动态+SSE
//		//	break;
//		//case 2:
//		//	static_block_test();//静态+信号量同步	任务块划分	水平划分
//		//	break;
//		//case 3:
//		//	static_block_v_test(); //静态 + 信号量同步	任务块划分	垂直划分
//		//	break;
//		//case 4:
//		//	static_block_sse_test();//静态+信号量同步+SSE 	任务按块划分	水平划分
//		//	break;
//		//case 5:
//		//	static_loop_test2();//静态+信号量同步	任务循环划分	水平划分
//		//	break;
//		//case 6:
//		//	static_loop_test();//静态+信号量同步	任务循环划分	垂直划分
//		//	break;
//		//case 7:
//		//	static_loop_test3();//静态+信号量同步+三重循环全部纳入线程函数  循环划分（改为按块划分）  水平划分
//		//	break;
//		//case 8:
//		//	static_barrier_loop_test();//静态+barrier同步	循环划分（块）  垂直划分（水平）
//		//	break;
//		//default:
//		//	break;
//		//}
//		// 
//		//dynamic_test();//动态线程（垂直划分）
//		//dynamic_sse_test();//动态+SSE
//		//static_block_v_test();
//		//static_block_test();//静态+信号量同步	任务块划分	水平划分
//		//static_block_sse_test();//静态+信号量同步+SSE 	任务按块划分	水平划分
//
//		//static_loop_test2();//静态+信号量同步	任务循环划分	水平划分
//		//static_loop_test();//静态+信号量同步	任务循环划分	垂直划分
//		//static_loop_test3();//静态+信号量同步+三重循环全部纳入线程函数  循环划分（改为按块划分）  水平划分
//		//static_barrier_loop_test();//静态+barrier同步	循环划分（块）  垂直划分（水平）
//
//		QueryPerformanceCounter((LARGE_INTEGER*)&tail);
//		cout << (tail - head) * 1.0 / freq << endl;
//
//		//释放动态内存
//		for (int i = 0; i < n; i++) {
//			delete[] m_data[i];
//		}
//		delete[] m_data;
//	}
//
//	return 0;
//}
