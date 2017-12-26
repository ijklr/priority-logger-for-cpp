#ifndef INCLUDE_LOGREADER
#define INCLUDE_LOGREADER

#include <LogStruct.h>
#include <QueueManager.h>

class LogReader
{
    public:
	LogReader(QueueManager<LogStruct>* qm);
	LogStruct get(); //blocking call
    private:
	QueueManager<LogStruct>* qm_;

};
#endif
