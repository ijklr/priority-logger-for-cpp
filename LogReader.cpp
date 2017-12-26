#include <LogReader.h>

LogStruct LogReader::get()
{
    return qm_->dequeue();
}

LogReader::LogReader(QueueManager<LogStruct>* qm)
    :qm_(qm) {}
