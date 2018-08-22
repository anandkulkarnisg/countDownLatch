#include<iostream>
#include<string>
#include<exception>

#ifndef CountDownLatchExceptions_H
#define CountDownLatchExceptions_H

const char* CountDownLatchReuseException_msg = "Exception : Attempt was made to use a CountDownLatch that has already expried.";

struct CountDownLatchReuseException : public std::exception
{
	const char * what () const throw ()
	{
		return(CountDownLatchReuseException_msg);
	}
};

#endif
