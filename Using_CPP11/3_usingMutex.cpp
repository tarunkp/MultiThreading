// https://thispointer.com//c11-multithreading-part-5-using-mutex-to-fix-race-conditions/
// In case of possible Race conditions, mutex can be used for atomicity and for exclusive access to resource.

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

#define TRANS_MONEY 100000

class WalletWithRace {
	int mMoney;
public:
	WalletWithRace() : mMoney(0) {}
	int getMoney() { return mMoney; }
	void addMoney(int money) {
		for(int i = 0; i < money; ++i)
			mMoney++;
	}
};

class WalletWithMutex {
	int mMoney;
	std::mutex mMutex;
public:
	WalletWithMutex() : mMoney(0) {}
	int getMoney() { return mMoney; }
	void addMoney(int money) {
		std::lock_guard<std::mutex> lockGuard(mMutex);
		//mMutex.lock(); // lockGuard's constructor carried out this
		for(int i = 0; i < money; ++i)
			mMoney++;
		//mMutex.unlock(); // lockGuard's destructor will do this
	}
};

int testRaceWallet() {
	WalletWithRace raceWallet;
	std::vector<std::thread> threads;
	for(size_t i = 0; i < 5; ++i)
		threads.push_back(std::thread(&WalletWithRace::addMoney, &raceWallet, TRANS_MONEY));

	for(size_t i = 0; i < threads.size() ; i++)
		threads.at(i).join();

	return raceWallet.getMoney();
}

int testMutexWallet() {
    WalletWithMutex walletObject;
    std::vector<std::thread> threads;
    for(size_t i = 0; i < 5; ++i)
        threads.push_back(std::thread(&WalletWithMutex::addMoney, &walletObject, TRANS_MONEY));
 
    for(size_t i = 0; i < threads.size() ; i++)
        threads.at(i).join();
    return walletObject.getMoney();
}
 
int main(int argc, char **argv) {
	std::cout<<"Testing Wallet with race conditions"<<std::endl;
	int mismatch_count = 0;
	for(int k = 0; k < 1000; k++) {
		int val = testRaceWallet();
		if(val != TRANS_MONEY *5) {
			std::cout << "Error at count = "<<k<<" Money in Wallet = "<<val << std::endl;
			mismatch_count++;
		}
	}
	std::cout<<"Wallet with race conditions: Mismatches found = "<<mismatch_count<<"\n\n";
	// Test Wallet with mutex
	std::cout<<"Testing Wallet with Mutex"<<std::endl;
	mismatch_count = 0;
	for(int k = 0; k < 1000; k++) {
		int val = testMutexWallet();
		if(val != TRANS_MONEY *5) {
			std::cout << "Error at count = "<<k<<" Money in Wallet = "<<val << std::endl;
			mismatch_count++;
		}
	}
	std::cout<<"Wallet with Mutex: Mismatches found = "<<mismatch_count<<std::endl;

	return 0;
}

