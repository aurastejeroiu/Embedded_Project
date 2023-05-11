#include <queue>
#include <cstdint>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include "DS_SynchronousBlockingQueue.h"


template<typename T>
inline SynchronousBlockingQueue<T>::SynchronousBlockingQueue(size_t maxSize)
{
	//TODO
}

template<typename T>
void SynchronousBlockingQueue<T>::push(const T& newElement)
{
	std::unique_lock<std::mutex> lock(mQueueMutex);
	while(mData.size() == mMaxSize) 
	{
		mConditionVariable.wait(lock);
	}
 
	mData.push(newElement);
	std::cout << "Pushed value " << newElement << std::endl;
	
	mConditionVariable.notify_all()
}

template<typename T>
T SynchronousBlockingQueue<T>::pop()
{
	//TODO
}

template<typename T>
bool SynchronousBlockingQueue<T>::empty()
{
	//TODO
}

template<typename T>
bool SynchronousBlockingQueue<T>::full()
{
	//TODO
}
