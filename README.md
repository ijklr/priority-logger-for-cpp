# priority-logger-for-cpp
This is a simple C++ logging library to log according to specified priorities. 

![architecture:](https://docs.google.com/drawings/d/e/2PACX-1vTudng8oSbWFIXEwJU3h7_K0VRZW3ZTzazvwqZVia2a84zgi533TKd9R43Z03Ob2mkLQik-Hq5AsFUo/pub?w=1440&h=1080)



Logger: This write the logs for a given topic, each with a priority number to the QueueManager.
QueueManager: This is singleton class with in-memory queues to store the logs. The logs in the queues are meant to be drained by the LogReaders to avoid high memory consumption. The logger blocks(for that topic) if the number of logs in the queues reaches certain threshold.
LogReader: This reads logs from the QueueManager according the specified priorities.
LogStruct: A struct that encapsulates the actual log string, priority number, topic name, time, thread id.

Examples:
```
//create a QueueManager singleton
auto qm = getQueueManager();

//threads A1, B1, B2 to log things
void threadA1(QueueManager *qm) {
    auto logger = new Logger("Topic A");
    logger.log(1, "abc");
    logger.log(2, "def");
    logger.log(1, "ghi");
    logger.log(3, "xyz");
}
void threadB1(QueueManager *qm) {
    auto logger = new Logger(qm, "Topic A");
    logger.log(1, "B1abc");
    logger.log(2, "B1def");
    logger.log(1, "B1ghi");
    logger.log(3, "B1xyz");
}
void threadB2(QueueManager *qm) {
    auto logger = new Logger(qm, "Topic A");
    logger.log(1, "B2abc");
    logger.log(2, "B2def");
    logger.log(1, "B2ghi");
    logger.log(3, "B2xyz");
}

//threads A3, B2, B3 to read the logs
void threadA3(QueueManager *qm) {
    auto logReader = new LogReader(qm, "Topic A");
    for(;;) {
	//Note that get() is a blocking call
	auto logStruct = logReader.get();
	Printer::pretty_print(logStruct);
    }
}
void threadB2(QueueManager *qm) {
    auto logReader = new LogReader(qm, "Topic B");
    for(;;) {
	auto logStruct = logReader.get();
	Printer::pretty_print(logStruct);
    }
}
void threadB3(QueueManager *qm) {
    auto logReader = new LogReader(qm, "Topic B");
    //Using the non-blocking get call.
    //It prints the first one if it exists.
    unique_ptr<LogStruct> log = logReader.getNonBlock();
    if(log) {
	Printer::pretty_print(logStruct);
    } else {
	Printer::print("threadB3 did not print anything from logReader");
    }
}

//spawn threads A1, A2, A3, B1, B2, B3
thread A1(threadA1, qm);
thread A2(threadA2, qm);
thread A3(threadA3, qm);
thread B1(threadB1, qm);
thread B2(threadB2, qm);
thread B3(threadB3, qm);

A1.join();
A2.join();
A3.join();
B1.join();
B2.join();
B3.join();
```
