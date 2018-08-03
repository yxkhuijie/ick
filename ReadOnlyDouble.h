#ifndef _READ_ONLY_DOUBLE_H_
#define _READ_ONLY_DOUBLE_H_

#include "UntypedData.h"

class ick_api ReadOnlyDouble : public UntypedData
{

public:

	ReadOnlyDouble();

	ReadOnlyDouble(double value);

	virtual ~ReadOnlyDouble();

private:

	double m_value;

	double m_oldValue;

	double m_accuracy;

	
	friend class IckStormPublisher;
	friend class IckStormSubscriberManager;
	friend class IckStormSubscriberSocket;

public:

	double getValue();

	void setSimulatedValue(double value);

	/*
	 * @ brief get value as string
	 * @ return string: string format of value
	 */
	virtual std::string getValueAsString() const;

	/*
	 * @ brief get value info
	 * @ return ValueInfo: value info of data
	 */
	virtual ValueInfo getValueInfo() const;

	/*
	 * @ brief get type info
	 * @ return TypeInfo: type info of data
	 */
	virtual TypeInfo getTypeInfo() const;

	
};

#endif

