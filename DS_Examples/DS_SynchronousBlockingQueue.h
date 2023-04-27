#pragma once

template <typename T> class SynchronousBlockingQueue {
	//internal representation of the queue
	std::queue<T> mData;
	
	/// maximum allowed size for the queue
	size_t mMaxSize;
	
	/// Mutex used for the queue in order to 
	/// protect the shared data from being simultaneously accessed by multiple threads.
	std::mutex mQueueMutex;
	std::condition_variable mConditionVariable;
public:

	/**
	* TODO
	*/
	SynchronousBlockingQueue(size_t maxSize = -1);
	void push(const T& newElement);
	T pop();
	bool empty();
	bool full();
};