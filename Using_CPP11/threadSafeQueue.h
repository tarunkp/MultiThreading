#include <string>
#include <deque>
#include <mutex>
#include <condition_variable>

class MsgQueue {
public:
	MsgQueue(int capacity);
	~MsgQueue();

	bool hasSpaceForEnque();
	bool hasDataToDeque();
	void enque(std::string msg);
	std::string deque();
	void enableDebug();

private:
	void addMsg(std::string msg);
	std::string getMsg();
private:
	int mCap;
	int mCount;
	std::deque<std::string> mQue;
	std::mutex mMutexCount;
	std::condition_variable mCondVarEnque;
	std::condition_variable mCondVarDeque;

	bool mDebug;
};

