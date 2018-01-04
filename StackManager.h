#ifndef INCLUDE_STACKMANAGER
#define INCLUDE_STACKMANAGER

#include <SafePrint.h>
#include <stack>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>

template <class T>
class StackManager {
    public:
	StackManager(size_t q_num, size_t limit): 
	    cond1_(), cond2_(), vs_(q_num), count_(0), limit_(limit) {}

	//Push item to the queue. Blocks if there are too many items.
	void enqueue(T logStruct, size_t idx)
	{
	    std::unique_lock<std::mutex> lock(count_mtx_);
	    //if the count getting too large, keep waiting until it's under the limit.
	    while(count_ > limit_) {
		cond2_.wait(lock);
	    }
	    vs_[idx].push(logStruct);
	    ++count_;
	    cond1_.notify_one();
	}

	//Get an element with the highest priority, or blocks if all queues are empty.
	T dequeue()
	{
	    std::unique_lock<std::mutex> lock(count_mtx_);
	    //if there nothing in to read, keep waiting until there's an enqueue event.
	    while(0 == count_) {
		cond1_.wait(lock);
	    }
	    T val;
	    //We are guaranteed to find an element because count_ > 0;
	    for(int i = vs_.size()-1; i >= 0; i--) {
		if(!vs_[i].empty()) {
		    val = vs_[i].top();
		    vs_[i].pop();
		    break;
		}
	    }
	    --count_;
	    cond2_.notify_one();
	    return val;
	}
private:
std::condition_variable cond1_, cond2_;
std::vector<std::stack<T>> vs_;
std::mutex count_mtx_;
size_t count_;
const size_t limit_;
};

#endif
