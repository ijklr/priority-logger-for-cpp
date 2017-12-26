#include <QueueManager.h>
#include <Logger.h>
#include <LogReader.h>
#include <LogStruct.h> 
#include <SafePrint.h>

#include <iostream>
#include <SafePrint.h>

using namespace std;

using QM = QueueManager<LogStruct>;

//threads A1, A2, B1 to log things
void threadA1(QM *qm) {
    auto logger = Logger(qm, "class name A");
    logger.log(3, "first");
    logger.log(1, "abc");
    logger.log(2, "def");
    logger.log(1, "ghi");
    logger.log(3, "xyz");
}
void threadA2(QM *qm) {
    auto logger = Logger(qm, "class name B");
    logger.log(1, "A2abc");
    logger.log(2, "A2def");
    logger.log(1, "A2ghi");
    logger.log(3, "A2xyz");
    logger.log(1, "A2klr");
}
void threadB1(QM *qm) {
    auto logger = Logger(qm, "class name B");
    logger.setPriority(1);
    logger.log("B1xyz");
    logger.setPriority(3);
    logger.logAppend("B1abc");
    logger.logAppend("B1def");
    logger.logPublish();
    logger.setPriority(1);
    logger.logAppend("B1last");
    logger.logPublish();
}

//threads A3, B2, B3 to read the logs
void threadA3(QM *qm) {
    auto logReader = LogReader(qm);
    for(;;) {
	//Note that get() is a blocking call
	auto logStruct = logReader.get();
	SafePrint::pretty_print(logStruct);
    }
}
void threadB2(QM *qm) {
    auto logReader = LogReader(qm);
    for(;;) {
	auto logStruct = logReader.get();
	SafePrint::pretty_print(logStruct);
    }
}
void threadB3(QM *qm) {
    auto logReader = LogReader(qm);
    //It prints the first one
    LogStruct log = logReader.get();
    SafePrint::pretty_print(log);
}

int main()
{
    cout << "Testing"<<endl;
    //create a QueueManager instance with 3 queues
    //limit it to 1 so it will hit our limit for throttling.
    auto qm = new QueueManager<LogStruct>(3, 1);

    //spawn threads A1, A2, A3, B1, B2, B3
    thread A1(threadA1, qm);
    thread A2(threadA2, qm);
    thread A3(threadA3, qm);
    thread B1(threadB1, qm);
    thread B2(threadB2, qm);
    thread B3(threadB3, qm);
    
    A1.join();
    A2.join();
    B1.join();

    std::this_thread::sleep_for(3s);
    A3.detach();
    B2.detach();
    B3.detach();
}
