#ifndef CountDownLatch_H
#define CountDownLatch_H

#include<iostream>
#include<string>
#include<atomic>
#include<shared_mutex>
#include<cmath>
#include<sstream>
#include<condition_variable>

#include "CountDownLatchExceptions.h"
#include "TimeUtils.h"

class CountDownLatch
{
  private:
    std::atomic<long>  m_count;									// The counter of the latch has to be thread safe. Hence atomic variable is perfectly suited for this purpose.
    std::condition_variable_any m_cond;					// The condition variable that is used to signal and control the logic for waiting.
    std::shared_mutex m_mutex;									// A Lock required for condition variable and signalling.
    std::atomic<bool> m_useStatus;							// This indicates the use status so that after the Latch has been counted down it can not be used again.
    std::string m_latchName;                    // The latch unique name.
    std::string getName();                      // This initializes the name of the Latch object uniquely in constructor.

  public:
    CountDownLatch(const CountDownLatch&) = delete;				// disable copy constructor
    CountDownLatch& operator=(CountDownLatch const&) = delete;	// disable assignment operator
    CountDownLatch(const long&);								// The countdown count that needs to be initialized.
    void await();												// block till the count is reduced to zero.
    bool await(const long&, const TimeUnit& = TimeUnit::MilliSeconds);// block till the count is reduced to zero or till the time out period is achieved.
    void countDown();											// counts the latch by one count.
    long getCount();											// Get the count currently held by the latch.
    std::string toString();										// Get a unique name of the Latch and the current status of count as well.
};

#endif
