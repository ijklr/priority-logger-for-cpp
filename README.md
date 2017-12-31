# priority-logger-for-cpp
This is a simple C++ logging library to log according to specified priorities. 

![architecture:](https://docs.google.com/drawings/d/e/2PACX-1vTudng8oSbWFIXEwJU3h7_K0VRZW3ZTzazvwqZVia2a84zgi533TKd9R43Z03Ob2mkLQik-Hq5AsFUo/pub?w=1440&h=1080)

Logger: This write the logs with a priority number to the StackManager, it blocks if the total number of logs in memory are too large(> limit). One can append to the same log using multiple lines in the code.(see example)
StackManager: This is the data storage class with in-memory stacks to store the logs. The logs in the stacks are meant to be drained by the LogReaders to avoid high memory consumption. The logger throttles if the total number of logs reaches certain threshold. The number of priorities can be specified. 
LogReader: This reads logs from the StackManager according the specified priorities. It blocks if there's nothing to read, as soon as there's something to read, it wakes up.
LogStruct: A struct that encapsulates the actual log string, priority number, class name, time(epoch), thread id.

Examples:
```
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
```
