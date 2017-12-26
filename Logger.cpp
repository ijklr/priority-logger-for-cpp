#include <Logger.h>

Logger::Logger(QueueManager<LogStruct> *qm, const std::string& class_name)
    :qm_(qm), class_name_(class_name) {}
    void Logger::setPriority(int p) {
	priority_ = p;
    }
int Logger::getPriority()
{
    return priority_;
}
void Logger::logAppend(const std::string &str)
{
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
    decorateInfo(logStruct_);
    qm_->enqueue(logStruct_, priority_-1);
    logStruct_.log = "";
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
    qm_->enqueue(logStruct, p-1);
}
