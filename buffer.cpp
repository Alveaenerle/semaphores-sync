#include <iostream>
#include <vector>
#include <string>
#include "monitor.h"
#include <thread>
#include <chrono>

int const threadsCounts = 4;  // number of threads

int const bufferSize = 9;
class Buffer
{
private:
	Semaphore mutex;
    Semaphore semA; 
    Semaphore empty; // 0 - empty
    Semaphore semB; 
	bool waitA;
	bool waitB;
	

	std::vector<char> values;

	void print(std::string name)
	{
		std::cout << "\n ###" << name << " " << values.size() << "[";
		for (auto v : values)
			std::cout << v << ",";
		std::cout << "] ###\n";
	}

	void debug(std::string info)
	{
		std::cout<<info<<std::endl;
	}

public:
	Buffer() : mutex(1), empty(0), semB(0), semA(0), waitA(false), waitB(false)
	{
	}

	inline bool canA()
	{
		return values.size() > 5 && values.size() < bufferSize;
	}

	inline bool canB()
	{
		return values.size() < bufferSize;
	}

	void putA(char value)
	{

        mutex.p();
		if (!canA())
		{
			waitA = true;
			mutex.v();
			semA.p();
			waitA = false;
		}

		values.push_back(value);
		print("PA");
		empty.v();

		if (canB() && waitB)
			semB.v();
		else 
			mutex.v();
		
	}

    void putB(char value)
	{
       
        mutex.p();
		if (!canB())
		{
			waitB = true;
			mutex.v();
			semB.p();
			waitB = false;
		}

		values.push_back(value);
		print("PB");
		if (values.size() > 3)
			empty.v();

		if (canA() && waitA)
			semA.v();
		else
			mutex.v();
		
	}

	char get()
	{

		empty.p();
        mutex.p();
		auto value = values.front();
		values.erase(values.begin());
		print("C");

		if (canA() && waitA)
			semA.v();
		else if (canB() && waitB)
			semB.v();
		else
			mutex.v();

		return value;
	}

	
};

Buffer buffer;

void* threadProdA(void* arg)
{
	for (int i = 0; i < 20; ++i)
	{
		buffer.putA('A');
		//std::cout<<i<<std::endl;
	}
	std::cout<<"skonczylem prod A"<<std::endl;
	return NULL;
}

void* threadProdB(void* arg)
{
	for (int i = 0; i < 20; ++i)
	{
		buffer.putB('B');
		//std::cout<<i<<std::endl;
	}
	std::cout<<"skonczylem prod B"<<std::endl;
	return NULL;
}

void* threadCons(void* arg)
{
	for (int i = 0; i < 20; ++i)
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(10));
		auto value = buffer.get();
		//std::cout<<i<<std::endl;
	}
	std::cout<<"skonczylem cons A"<<std::endl;
	return NULL;
}



int main()
{
#ifdef _WIN32
	HANDLE tid[threadsCounts];
	DWORD id;

	// create threads
	tid[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProd, 0, 0, &id);
	tid[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProd, 0, 0, &id);
	tid[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProd, 0, 0, &id);
	tid[3] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadConsA, 0, 0, &id);
	tid[4] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadConsB, 0, 0, &id);

	// wait for threads to finish
	for (int i = 0; i <= threadsCounts; i++)
		WaitForSingleObject(tid[i], INFINITE);
#else
	pthread_t tid[threadsCounts];

	// create threads
	pthread_create(&(tid[0]), NULL, threadProdA, NULL);
	pthread_create(&(tid[1]), NULL, threadProdB, NULL);
	pthread_create(&(tid[2]), NULL, threadCons, NULL);
	pthread_create(&(tid[3]), NULL, threadCons, NULL);

	// wait for threads to finish
	for (int i = 0; i < threadsCounts; i++)
		pthread_join(tid[i], (void**)NULL);
#endif
	return 0;
}

