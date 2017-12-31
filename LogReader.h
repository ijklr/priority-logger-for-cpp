#ifndef INCLUDE_LOGREADER
#define INCLUDE_LOGREADER

#include <LogStruct.h>
#include <StackManager.h>

class LogReader
{
    public:
	LogReader(StackManager<LogStruct>* sm);
	LogStruct get(); //blocking call
    private:
	StackManager<LogStruct>* sm_;

};
#endif
