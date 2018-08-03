#ifndef _READ_ONLY_INT_H_
#define _READ_ONLY_INT_H_

#include "UntypedData.h"
#include "IntValueInfo.h"

class ick_api ReadOnlyInt : public UntypedData
{

public:

	ReadOnlyInt(int value = 0);

	ReadOnlyInt(IntValueInfo& valueInfo);

	virtual ~ReadOnlyInt();

private:

	int m_value;

	int m_oldValue;

	IntValueInfo m_valueInfo;

	bool m_valid;

	friend class IckStormPublisher;
	friend class IckStormSubscriberManager;
	friend class IckStormSubscriberSocket;

public:

	int getValue();

	int getOldValue();

	void setSimulatedValue(int value);

	virtual bool isValidated();

	std::string getValueAsString() const;

	IntValueInfo getValueInfo() const;

	TypeInfo getTypeInfo() const;

};

#endif

