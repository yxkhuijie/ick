#ifndef _ICK_STORM_DATA_H_
#define _ICK_STORM_DATA_H
#include <string>

namespace IckCore
{
  enum RemoteDataType
  {
    UNKNOWN,
    INTDATA,
    STRINGDATA,
    DOUBLEDATA
  };

  enum RemoteAccessMode
  {
    DEFAULT,
    READONLY,
    READWRITE
  };

  struct RemoteIntTypeInfo
  {
    int min;
    int max;
    ::std::string unit;
  };

  struct RemoteStringTypeInfo
  {
    long long int maxLength;
  };

  struct RemoteDoubleTypeInfo
  {
    double min;
    double max;
    ::std::string unit;
    double accuracy;
  };

  struct RemoteValueInfo
  {
    ::IckCore::RemoteDataType dataType;
    ::IckCore::RemoteAccessMode accessMode;
    long long int timeStamp;
    int iValue;
    double dValue;
    ::std::string sValue;
    ::std::string descriptor;
  };

  struct RemoteIntValueInfo
  {
    int value;
    ::std::string descriptor;
    ::IckCore::RemoteDataType dataType;
    ::IckCore::RemoteAccessMode accessMode;
    long long int timeStamp;
  };

  struct RemoteDoubleValueInfo
  {
    double value;
    ::IckCore::RemoteDataType dataType;
    ::IckCore::RemoteAccessMode accessMode;
    long long int timeStamp;
  };

  struct RemoteStringValueInfo
  {
    ::std::string value;
    ::IckCore::RemoteDataType dataType;
    ::IckCore::RemoteAccessMode accessMode;
    long long int timeStamp;
  };

  struct RemoteDataInfo
  {
    int nodeNumber;
    int channelNumber;
    ::IckCore::RemoteDataType dataType;
    ::IckCore::RemoteAccessMode accessMode;
    bool isSimulated;
  };

  struct RemoteIntDataInfo
  {
    ::IckCore::RemoteIntTypeInfo typeInfo;
  };

  struct RemoteDoubleDataInfo
  {
    ::IckCore::RemoteDoubleTypeInfo typeInfo;
  };

  struct RemoteStringDataInfo
  {
    ::IckCore::RemoteStringTypeInfo typeInfo;
  };

  struct RemoteData
  {
    ::std::string channelName;
    ::std::string value;
  };

  struct DataInfo
  {
    int nodeNumber;
    int channelNumber;
  };

  struct SubscriberInfo
  {
    std::string objectId;
    std::string endPoint;
  };
};
#endif
