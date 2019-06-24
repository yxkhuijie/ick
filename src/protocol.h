#ifndef _IPROTOCOL_H_
#define _IPROTOCOL_H_

#include "src/control_object.h"
#include <map>
#include <list>
#include <vector>

enum ProtocolEncodingType
{
  ProtocolTypeHex   = 1,
  ProtocolTypeAscii = 2,
};

struct ProtocolFormat
{
  std::string name;
  std::string format;
  std::string description;
  int paramNum;
};

class ick_api IProtocol : public ControlObject
{
  DECLARE_CLASS_DYNAMIC(IProtocol)
  DECLARE_MESSAGE_LIST
public:

  IProtocol();

  virtual ~IProtocol();

private:
  


  /*
   * @ brief protocol format string
   */
  std::map<std::string, ProtocolFormat> m_strProtocolFormatMap;


  /*
   * @ brief protocol encoding type
   * @ value: default is hex, 
   * @ comment: you can call function setEncodingType to change it
   */
  ProtocolEncodingType m_encodingType;

public:
  
  virtual void onFrameReceived(const char* frame);

  void addProtocolFormat(const std::string& name, const std::string& format, const std::string& des="");

  std::string getProtocolFormat(const std::string& name);

  void setEncodingType(const std::string& type);

  /*
  * @ brief get encoding type protocol used.
  * @ return 
  *     ProtocolEncodingType::ProtocolTypeHex or
  *     ProtocolEncodingType::ProtocolTypeAscii
  */
  ProtocolEncodingType getEncodingType();

  /*
   * @ brief get encoding type protocol used.
   * @ return "HEX" or "ASCII" returned.
   */
  std::string getEncodingTypeAsString();

  /*
   * @ convert protocol data from hex string to bytes 
   * @ param strProtocol: hex string to be converted
   * @ param buffer: buffer to save results
   * @ length : size of buffer
   * @ return : actual size of result converted, may be different from length
   */
  int toBytes(std::string& strProtocol, unsigned char* buffer, int length);

  /*
   * @ brief converter frame data from hex to string format
   * @ e.g. buff[] = { 0x68, 0x01, 0x02, 0x01, 0x02, 0x68}
     *   result is: "68 01 02 01 02 68"
   */
  std::string toString(unsigned char* buff, int length);
};

#endif

