#ifndef _ICK_SERVICE_IMPL_H_
#define _ICK_SERVICE_IMPL_H_


#include "IObject.h"
//#include "IckService.h"

class ick_api IckServiceImpl/* : public IckCore::IckService*/
{
public:
	IckServiceImpl();
	~IckServiceImpl();

public:
	virtual void runService(const std::string& objectName, const std::string& serviceName, const std::string& param/*, const ::Ice::Current& = ::Ice::emptyCurrent*/);
	
};

#endif

