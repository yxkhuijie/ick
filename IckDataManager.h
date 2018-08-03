#ifndef _ICK_DATA_MANAGER_H_
#define _ICK_DATA_MANAGER_H_

//#include <IceUtil\IceUtil.h>

#include "ObjectManager.h"

class IckDataManager
{
private:
	IckDataManager();
	~IckDataManager();

private:

	static IckDataManager* m_instance;

public:

	static IckDataManager* getInstance();

};
#endif

