#ifndef _ICK_STORM_PUBLISH_SOCKET_H_
#define _ICK_STORM_PUBLISH_SOCKET_H_

#include "ISocket.h"
#include "Subscriber.h"

class IckStormPublishSocket : public ISocket, public Subscriber
{

public:

	IckStormPublishSocket();

	virtual ~IckStormPublishSocket();



};

#endif

