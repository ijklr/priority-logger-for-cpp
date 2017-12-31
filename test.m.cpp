#include <StackManager.h>
#include <Logger.h>
#include <LogReader.h>
#include <LogStruct.h> 
#include <SafePrint.h>

#include <iostream>
#include <SafePrint.h>

using namespace std;

using SM = StackManager<LogStruct>;

//threads A1, A2, B1 to log things
void threadA1(SM *sm) {
    Logger logger(sm, "class name A");
    logger.log(3, "first");
    logger.log(1, "abc");
    logger.log(2, "def");
    logger.log(1, "ghi");
    logger.log(3, "xyz");
}
void threadA2(SM *sm) {
    Logger logger(sm, "class name A");
    logger.log(1, "A2abc");
    logger.log(2, "A2def");
    logger.log(1, "A2ghi");
    logger.log(3, "A2xyz");
    logger.log(1, "A2klr");
}
void threadB1(SM *sm) {
    Logger logger(sm, "class name B");
    logger.setPriority(1);
    logger.log("B1xyz");
    logger.setPriority(3);
    logger.logAppend("B1abc");
    logger.logAppend("B1def");
    logger.logPublish();
    logger.setPriority(1);
    //this will automatically publish as well
    logger.logAppend("B1last");
}

//threads A3, B2, B3 to read the logs
void threadA3(SM *sm) {
    LogReader logReader(sm);
    for(;;) {
	//Note that get() is a blocking call
	auto logStruct = logReader.get();
	SafePrint::pretty_print(logStruct);
    }
}
void threadB2(SM *sm) {
    LogReader logReader(sm);
    for(;;) {
	auto logStruct = logReader.get();
	SafePrint::pretty_print(logStruct);
    }
}
void threadB3(SM *sm) {
    LogReader logReader(sm);
    //It prints the first one
    LogStruct log = logReader.get();
    SafePrint::pretty_print(log);
}


int main()
{
    cout << "Testing"<<endl;
    //create a QueueManager instance with 3 queues
    size_t q_num = 3;
    size_t limit = 100;
    auto sm = new StackManager<LogStruct>(q_num, limit);

    //spawn threads A1, A2, A3, B1, B2, B3
    thread A1(threadA1, sm);
    thread A2(threadA2, sm);
    thread B1(threadB1, sm);
    //sleep so all the producers will be done.
    std::this_thread::sleep_for(1s);

    thread A3(threadA3, sm);
    thread B2(threadB2, sm);
    thread B3(threadB3, sm);
    //sleep so all the consumers will be done.
    std::this_thread::sleep_for(1s);

    A1.join();
    A2.join();
    B1.join();
    A3.detach();
    B2.detach();
    B3.detach();
}
