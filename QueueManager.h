#include <LogStruct.h>

class QueueManager {
    public:
	static QueueManager* getInstance() {
	    if(singleton) {
		return singleton;
	    } else {
		singleton = new QueueManager();
	    }
	}

	void publish(const LogStruct logStruct);

	LogStruct get();
	unique_ptr<LogStruct> getNonBlock();

    private:
	QueueManager();
	static QueueManager* singleton;
	queue<LogStruct> QP1
	

	
	    

};
