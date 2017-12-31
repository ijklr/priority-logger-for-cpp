#include <Logger.h>

Logger::Logger(StackManager<LogStruct> *sm, const std::string& class_name)
    :sm_(sm), class_name_(class_name) {}
    void Logger::setPriority(int p) {
	priority_ = p;
    }
Logger::~Logger()
{
    //publish any partial log that the caller did not publish.
    if(!logStruct_.log.empty()) {
	logPublish();
    }
}
int Logger::getPriority()
{
    return priority_;
}
void Logger::logAppend(const std::string &str)
{
    std::lock_guard<std::mutex> lock(log_mtx_);
    logStruct_.log += str;
}

void Logger::decorateInfo(LogStruct &logStruct)
{
    logStruct.priority = priority_;
    logStruct.epoch = std::time(0);
    logStruct.thread_id = std::this_thread::get_id();
    logStruct.class_name = class_name_;
}

void Logger::logPublish()
{
    std::lock_guard<std::mutex> lock(log_mtx_);
    decorateInfo(logStruct_);
    sm_->enqueue(logStruct_, priority_-1);
    logStruct_.log.clear();
}
void Logger::log(const std::string &str)
{
    decorateInfo(logStruct_);
    log(priority_, str);
}

void Logger::log(int p, const std::string &str)
{
    LogStruct logStruct;
    decorateInfo(logStruct);
    logStruct.priority = p;
    logStruct.log = str;
    sm_->enqueue(logStruct, p-1);
}
