#ifndef CountDownLatchExceptions_H
#define CountDownLatchExceptions_H

#include<iostream>
#include<string>
#include<exception>

struct CountDownLatchReuseException : public std::exception
{
	const char* CountDownLatchReuseException_msg = "Exception : Attempt was made to use a CountDownLatch that has already expried.";

	const char * what () const throw ()
	{
		return(CountDownLatchReuseException_msg);
	}
};

#endif
