#ifndef _IO_EXCEPTION_H_
#define _IO_EXCEPTION_H_

#include <exception>
#include "ReadOnlyInt.h"

class IOException : public std::exception
{

public:
	
	IOException();
	
	IOException(ReadOnlyInt* data);

	~IOException() throw();



};

#endif

