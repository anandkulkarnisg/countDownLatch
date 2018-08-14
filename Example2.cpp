#include<iostream>
#include<string>
#include<thread>

#include "CountDownLatch.h"

using namespace std;

CountDownLatch latch(1);

void sampleThreadFunc(const long& waitTime)
{
	std::cout << "I am in this thread id = " << std::this_thread::get_id() << " starting to wait for" << waitTime << " milli seconds" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
	latch.countDown();
	std::cout << "I am in this thread id ="  << std::this_thread::get_id() << " Finished waiting for the " << waitTime << " milli seconds" << std::endl;
}


int main(int argc, char* argv[])
{
	std::thread t1(&sampleThreadFunc, 12000);
	std::cout << "Main Thread : starting to wait for the Latch to finish waiting." << std::endl;
	latch.await(10000);
	std::cout << "Latch status" << latch.toString() << std::endl;
	std::cout << "Main Thread : finished waiting for the latch" << std::endl;
	t1.join();
	std::cout << "Latch status" << latch.toString() << std::endl;
	try
	{
		latch.await();
	}
	catch(const std::string& e)
	{
		std::cout << e << std::endl;
	}

	return(0);
}
