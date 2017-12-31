#include <LogReader.h>

LogStruct LogReader::get()
{
    return sm_->dequeue();
}

LogReader::LogReader(StackManager<LogStruct>* sm)
    :sm_(sm) {}
