#ifndef INCLUDE_LOGGER
#define INCLUDE_LOGGER

#include <StackManager.h>
#include <LogStruct.h>

class Logger
{
    public:
	Logger(StackManager<LogStruct> *sm, const std::string& class_name);
	~Logger();
	void setPriority(int p);
	int getPriority();
	void logAppend(const std::string &str);
	void logPublish();
	void log(const std::string &str);
	void log(int p, const std::string &str);
    private:
	StackManager<LogStruct> *sm_; //publish to this queue manager
	void decorateInfo(LogStruct &logStruct);
	int priority_ = 1; //defaults to lowest 
	const std::string class_name_; //one class name for one instance of Logger
	LogStruct logStruct_; //used for partial logging
	std::mutex log_mtx_; //mutex for partial logging
};

#endif
