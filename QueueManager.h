#ifndef INCLUDE_QUEUEMANAGER
#define INCLUDE_QUEUEMANAGER

#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <thread>
#include <iostream>
#include <SafePrint.h>

template <class T>
class QueueManager {
    public:
	QueueManager(size_t q_num, size_t limit): mtx_(), cond_(), vq_(q_num), sleep_milli_(1), limit_(limit){}

	//push to the queue but potential a blocking-
	//call if there are too many items in the queues
	void enqueue(T logStruct, size_t idx)
	{
	    size_t sz_sum = 0;
	    {
		std::lock_guard<std::mutex> lock(mtx_);
		for(size_t i = 0; i < vq_.size(); ++i) {
		    sz_sum += vq_[i].size();
		}
	    }

	    {
		std::lock_guard<std::mutex> lock(sleep_mtx_);
		//if the queue sizes are getting too large, 
		//we need throttle it.(*2 each time it happens again) and try again.
		if(sz_sum > limit_) {
		    sleep_milli_ *= 2; 	
		} else {
		    sleep_milli_ = 1;
		}
	    }

	    if(sleep_milli_ > 1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep_milli_));
		enqueue(logStruct, idx);
	    } else {
		//if the size is OK, we can go ahead and put more stuff in it.
		sleep_milli_ = 1;
		std::lock_guard<std::mutex> lock(mtx_);
		vq_[idx].push(logStruct);
		cond_.notify_one();
	    }
	}

	//get an element from one of the queues(highest priority), or blocks if it's all empty
	T dequeue()
	{
	    std::unique_lock<std::mutex> lock(mtx_);
	    auto all_empty = [this]() {
		bool ans = true;
		for(size_t i = 0; i < vq_.size(); ++i) {
		    ans = ans && vq_[i].empty();
		}
		return ans;
	    };
	    while(all_empty()) {
		cond_.wait(lock);
	    }

	    for(size_t i = vq_.size()-1; i >= 0; i--) {
		if(!vq_[i].empty()) {
		    T val = vq_[i].front();
		    vq_[i].pop();
		    return val;
		}
	    }
	}

    private:
	mutable std::mutex mtx_, sleep_mtx_;
	std::condition_variable cond_;
	std::vector<std::queue<T>> vq_;
	size_t sleep_milli_, limit_;
};

#endif
