#include "DS_SynchronousBlockingQueue.h"


template<typename T>
SynchronousBlockingQueue<T>::SynchronousBlockingQueue(size_t maxSize)
{
	this->mMaxSize = maxSize;
}

template<typename T>
void SynchronousBlockingQueue<T>::push(const T& newElement)
{
	std::unique_lock<std::mutex> lock(mQueueMutex);
	while (mData.size() == mMaxSize)
	{
		mConditionVariable.wait(lock);
	}

	mData.push(newElement);
	std::cout << "Pushed value " << newElement << std::endl;

	mConditionVariable.notify_all();
}

template<typename T>
T SynchronousBlockingQueue<T>::pop()
{
	//TODO
	std::unique_lock<std::mutex> lock(mQueueMutex);
	while (mData.empty())
	{
		mConditionVariable.wait(lock);
	}

	T element = mData.front();
	mData.pop();
	std::cout << "Popped value " << element << std::endl;

	mConditionVariable.notify_all();
	return element;
}

template<typename T>
bool SynchronousBlockingQueue<T>::empty()
{
	return mData.empty();
}

template<typename T>
bool SynchronousBlockingQueue<T>::full()
{
	return (mData.size() == mMaxSize);
}
