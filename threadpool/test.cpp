#ifndef THREADPOLL_H
#define THREADPOLL_H

//三种机制实现同步与互斥  mutex  : lock  unlocker   cond  : wait signal    sem : wait  post

#include<list>
#include<cstdio>
#include<exception>
#include<pthread.h>
#include"locker.h"

template<class T>
class threadpool
{
public:
	threadpool(int thead_number = 8, int max_requests = 10000);
	~threadpool();
	bool append(T*request);    //往请求队列中添加数据

private:
	static void * worker(void* arg);   //线程工作运行的函数
	void run();

	
private:
	int m_thread_number;      //线程池中的线程数  
	int m_max_request;        //请求队列中最大请求数
	pthread_t * m_threads;      //描述线程的数组，其大小为m_thread_number
	std::list<T*> m_workqueue;//请求队列
	locker m_queuelocker;     //保护请求队列的互斥锁
	sem m_queuestat;         //是否有任务需要处理
	bool m_stop;             //是否结束函数

};

//最重要的两个参数：描述线程的数组    请求队列


template<class T>
threadpool<T>::threadpool(int thread_number, int max_requests) :
m_thread_number(thread_number), m_max_request(max_requests),
m_stop(false), m_threds(NULL)
{
	if ((thread_number <= 0) || (max_requests <= 0))
	{
		throw std::exception();
	}
	m_threads = new pthread_t[m_thread_number];
	if (!m_threads)
	{
		throw std::exception();
	}

	//创建thread_number个线程
	for (int i = 0; i < thread_number; i++)
	{
		printf("create the %d th thread\n",i);
		if (pthread_create(m_threads + i, NULL, worker, this) != 0)
		{
			delete[] m_threads;
			throw std::exception();
		}

		//线程分离
		if (phtread_detach(m_threads[i]))
		{
			delete[] m_threads;
			throw std::exception();
		}
	}

}

template<class T>
threadpool<T>::~threadpool()
{
	delete[] m_threads;
	m_stop = true;
}
template<class T>
bool threadpool<T>::append(T*request)
{
	//操作队列时，一定要上锁，因为所有的线程是共享进程空间的
	m_queuelocker.lock();
	if (m_workqueue.size() > m_max_request)
	{
		m_queuelocker.unlocker();
		return false;
	}

	m_workqueue.push_back(request);
	m_queuelocker.unlocker();
	m_queuestat.post();
	return true;
}


template<class T>
void* threadpool<T>::worker(void* arg)
{
	threadpool* pool = (threadpool*)arg;
	pool->run();
	return pool;
}


template<typename T>
void threadpool<T>::run()
{
	while (!m_stop)
	{
		m_queuestat.wait();
		m_queuelocker.locker();

		if (m_workqueue.empty())
		{
			m_queuelocker.unlocker();
			continue;
		}

		T* request = m_workqueue.front();
		m_workqueue.pop_front();
		m_queuelocker.unlock();

		if (!request)
		{
			continue;
		}
		request->process();
	}
}
#endif
