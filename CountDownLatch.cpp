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
    unique_lock<shared_mutex> lock(m_mutex);
    m_cond.wait(lock, [this]{ return(this->m_count==0); });
    lock.unlock();
    m_useStatus.store(true);
  }
  else
    throw CountDownLatchReuseException();
}

// The await function works fine without any issues. It is used primarily for purpose of timeout for main thread which waits on the latch being counted down.
// After it times out it can use the bool flag of return status to decide what to do further in case of timeout.
bool CountDownLatch::await(const long& waitTimeQuantity, const TimeUnit& timeUnit)
{
  if(!m_useStatus)
  {
    unique_lock<shared_mutex> lock(m_mutex);
    m_cond.wait_for(lock, TimeUtils::waitDuration(waitTimeQuantity, timeUnit), [this]{ return(this->m_count==0); });
    if(lock.owns_lock())
      lock.unlock();
    if(m_count  == 0)
    {
      m_useStatus.store(true);
      return true;
    }
    else
      return false;
  }
  else
    throw CountDownLatchReuseException();
}

void CountDownLatch::countDown()
{
  // Simply reduce the count of the atomic variable by 1.
  if(!m_useStatus)
  {
    m_count.fetch_sub(1, memory_order_relaxed);
    m_cond.notify_all();
    if(m_count == 0)
      m_useStatus.store(true);
  }
  else
    throw CountDownLatchReuseException();
}

long CountDownLatch::getCount()
{
  // return the count of the Latch here. Its ok to call this function on an expired Latch also. does not harm.
  shared_lock<shared_mutex> readLock(m_mutex);
  return m_count;
}

string CountDownLatch::toString()
{
  shared_lock<shared_mutex> readLock(m_mutex);
  const void * address = static_cast<const void*>(this);
  stringstream stream;
  stream << address;  
  string returnString = " ==> [ Latch Name = latch." + stream.str();
  returnString += ", Latch Count = ";
  returnString += to_string(getCount());
  returnString += ", Latch UseStatus = ";
  if(!m_useStatus)
    returnString += "InUse. ]";
  else
    returnString += "Used. ]";
  return returnString;
}
