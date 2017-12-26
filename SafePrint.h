#ifndef INCLUDE_SAFEPRINT
#define INCLUDE_SAFEPRINT

#include <string>
#include <LogStruct.h>
#include <mutex>

class SafePrint {
    public:
	static void pretty_print(const LogStruct& logStruct);
	static void print(const std::string& str);
    private:
	static std::mutex mtx;
};
#endif
