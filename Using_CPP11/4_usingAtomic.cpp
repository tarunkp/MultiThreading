// https://thispointer.com//c11-multithreading-part-5-using-mutex-to-fix-race-conditions/
//

#include <iostream>
#include <thread>
#include <atomic> 
//#include <mutex>
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

class WalletWithAtomic {
	std::atomic<int> mMoney; // Use std::atomic, rest is exactly same
public:
	WalletWithAtomic() : mMoney(0) {}
	int getMoney() { return mMoney; }
	void addMoney(int money) {
		for(int i = 0; i < money; ++i)
			mMoney++;
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

int testAtomicWallet() {
    WalletWithAtomic walletObject;
    std::vector<std::thread> threads;
    for(size_t i = 0; i < 5; ++i)
        threads.push_back(std::thread(&WalletWithAtomic::addMoney, &walletObject, TRANS_MONEY));
 
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
	std::cout<<"Testing Wallet with Atomic"<<std::endl;
	mismatch_count = 0;
	for(int k = 0; k < 1000; k++) {
		int val = testAtomicWallet();
		if(val != TRANS_MONEY *5) {
			std::cout << "Error at count = "<<k<<" Money in Wallet = "<<val << std::endl;
			mismatch_count++;
		}
	}
	std::cout<<"Wallet with Atomic: Mismatches found = "<<mismatch_count<<std::endl;

	return 0;
}

