#include<iostream>
#include<string>
#include<thread>

#include "CountDownLatch.h"

using namespace std;

CountDownLatch latch(2);

void waitFunc(const long& waitTime)
{
	std::cout << "I am in this thread id = " << std::this_thread::get_id() << "and i am going to wait for " << waitTime << "milli seconds" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
	std::cout << "I am in this thread id = " << std::this_thread::get_id() << ".Now i am counting down the latch." << std::endl;
	latch.countDown();	
}

void waitForCountDown()
{
	std::cout << "I am in this thread id = " << std::this_thread::get_id() << " starting to wait for the latch to go down to zero." << std::endl;
	latch.await();
	std::cout << "I am in this thread id = " << std::this_thread::get_id() << " Finished waiting for the latch to go down to zero." << std::endl;
}

int main(int argc, char* argv[])
{
	std::thread t1(&waitFunc, 5000);
	std::thread t2(&waitFunc, 10000);

	std::thread t3(&waitForCountDown);
	std::thread t4(&waitForCountDown);	

	std::cout << "Main thread waiting for the count down to finish" << std::endl;
	latch.await();		
	std::cout << "Main Thread , the await finished." << std::endl;

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	return(0);
}

