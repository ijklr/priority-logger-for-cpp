#ifndef INCLUDE_LOGGER
#define INCLUDE_LOGGER

#include <QueueManager.h>
#include <LogStruct.h>

class Logger
{
    public:
	Logger(QueueManager<LogStruct> *qm, const std::string& class_name);
	void setPriority(int p);
	int getPriority();
	void logAppend(const std::string &str);
	void logPublish();
	void log(const std::string &str);
	void log(int p, const std::string &str);
    private:
	QueueManager<LogStruct> *qm_; //publish to this queue manager
	void decorateInfo(LogStruct &logStruct);
	int priority_ = 1; //defaults to lowest 
	const std::string class_name_; //one class name for one instance of Logger
	LogStruct logStruct_; //used for partial logging
};

#endif
