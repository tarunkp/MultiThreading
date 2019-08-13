
class MsgQueue {
public:
	MsgQueue(int capacity);
	~MsgQueue();

	void enque(const char *msg);
	const char * deque();
	void enableDebug();

private:
	void addMsg(const char* msg);
	const char* getMsg();
private:
	int mCap;
	int mCount;
	int mHead, mTail;
	char **mQue;
	pthread_mutex_t mMutexCount = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t  mCondVarEnque  = PTHREAD_COND_INITIALIZER;
	pthread_cond_t  mCondVarDeque  = PTHREAD_COND_INITIALIZER;

	bool mDebug;
};

