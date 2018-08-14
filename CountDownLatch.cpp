#include<iostream>
#include<string>
#include<thread>

#include "CountDownLatch.h"

using namespace std;

// Constructor.
CountDownLatch::CountDownLatch(const long& count) 
{
	m_count.store(abs(count));
	m_useStatus.store(false);
}

// await implementation.
void CountDownLatch::await()
{
	if(!m_useStatus)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cond.wait(lock, [this]{ return(this->m_count==0); });
		m_useStatus.store(true);
	}
	else
	{
		std::cout << "Exception : The count down latch has already been counted down!!" << std::endl;
	}
}

// await implementation with a wait for certain time. This needs improvement and fix.
bool CountDownLatch::await(const long& waitTime)
{
	if(!m_useStatus)
	{
		std::unique_lock<std::mutex> lock(m_mutex,std::defer_lock);
		m_cond.wait_for(lock, std::chrono::milliseconds(waitTime), [this]{ return(this->m_count==0); });
		if(m_count  == 0)
		{
			m_useStatus.store(true);
			return(true);
		}
		else
			return(false);
	}
	else
	{
		std::cout << "Exception : The count down latch has already been counted down!!" << std::endl;
		return(false);
	}
}

void CountDownLatch::countDown()
{
	// Simply reduce the count of the atomic variable by 1.
	m_count.fetch_sub(1, std::memory_order_relaxed);
	m_cond.notify_all();
	if(m_count == 0)
		m_useStatus.store(true);
}

long CountDownLatch::getCount()
{
	// return the count of the Latch here.
	return(m_count);
}

std::string CountDownLatch::toString()
{
	const void * address = static_cast<const void*>(this);
	std::stringstream stream;
	stream << address;  
	std::string returnString = "Latch Name = latch." + stream.str();
	returnString += ", Latch Count = ";
	returnString += std::to_string(getCount());
	returnString += ", Latch UseStatus = ";
	if(!m_useStatus)
		returnString += "InUse.";
	else
		returnString += "Used.";
	return(returnString);	
}
