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
		lock.unlock();
		m_useStatus.store(true);
	}
	else
	{
		throw CountDownLatchReuseException();
	}
}

// await implementation with a wait for certain time. This needs improvement and fix.
bool CountDownLatch::await(const long& waitTime)
{
	if(!m_useStatus)
	{
		std::unique_lock<std::mutex> lock(m_mutex,std::defer_lock);
		m_cond.wait_for(lock, std::chrono::milliseconds(waitTime), [this]{ return(this->m_count==0); });
		lock.unlock();
		if(m_count  == 0)
		{
			m_useStatus.store(true);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		throw CountDownLatchReuseException();
	}
}

void CountDownLatch::countDown()
{
	// Simply reduce the count of the atomic variable by 1.
	if(!m_useStatus)
	{
		m_count.fetch_sub(1, std::memory_order_relaxed);
		m_cond.notify_all();
		if(m_count == 0)
		{
			m_useStatus.store(true);
		}
	}
	else
	{
		throw CountDownLatchReuseException();
	}
}

long CountDownLatch::getCount()
{
	// return the count of the Latch here. Its ok to call this function on an expired Latch also. does not harm.
	return m_count;
}

std::string CountDownLatch::toString()
{
	const void * address = static_cast<const void*>(this);
	std::stringstream stream;
	stream << address;  
	std::string returnString = " ==> [ Latch Name = latch." + stream.str();
	returnString += ", Latch Count = ";
	returnString += std::to_string(getCount());
	returnString += ", Latch UseStatus = ";
	if(!m_useStatus)
		returnString += "InUse. ]";
	else
		returnString += "Used. ]";
	return returnString;
}
