#include <SafePrint.h>
#include <ctime>
#include <chrono>
#include <iostream>
#include <string>

std::mutex SafePrint::mtx;

void SafePrint::pretty_print(const LogStruct& log)
{
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "[" << log.epoch << "|" << log.thread_id << "|" 
	<< log.class_name << "|"  <<log.priority<<"] " << log.log << std::endl;
}

void SafePrint::print(const std::string& str)
{
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << str;
}
