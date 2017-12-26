#ifndef INCLUDE_LOGSTRUCT
#define INCLUDE_LOGSTRUCT

#include <string>
#include <thread>
#include <ctime>

struct LogStruct {
    int priority; 
    std::string log;
    std::string class_name;
    std::time_t epoch;
    std::thread::id thread_id;
};

#endif
