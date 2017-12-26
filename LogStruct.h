
#include <string>
#include <thread>
#include <time.h>

struct LogStruct {
    int priority; 
    std::string log;
    std::string class_name;
    time_t time;
    std::thread::id thread_id;
};
