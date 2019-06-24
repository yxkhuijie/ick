#ifndef _IFRAME_DATA_H_
#define _IFRAMED_ATA_H_

#include "IObject.h"

class ick_api IFrameData : public IObject
{
public:

  IFrameData();
  
  virtual ~IFrameData();

protected:
  /*
   * @ brief frame recevied buffer
   */
  unsigned char* m_pRecvFrame;
  
  /*
   * @ brief frame send buffer
   */
  unsigned char* m_pSendFrame;

  /*
   * @ brief frame length received
   */
  int            m_recvFramelength;
  
  /*
   * @ brief frame length to send
   */
  int            m_sendFrameLength;

  /*
   * @ brief frame error message
   */
  std::string    m_frameErrorMsg;

  /*
   * @ brief frame function description
   */
  std::string    m_frameDescription;

public:

  void setRecvFrame(const unsigned char* frame, int length);

  void setSendFrame(const unsigned char* frame, int length);

  unsigned char* getRecvFrame();

  unsigned char* getSendFrame();

  int getRecvFrameLength();

  int getSendFrameLength();

public:

  /*
   * @ brief frame received handler
   */
  virtual void onFrameReceived();

  /*
   * @ brief verify frame format
   */
  virtual bool verifyFrameFormat(const unsigned char* frame, int length);

  /*
   * @ brief analysis frame
   */
  virtual bool analysisFrame(const unsigned char* frame, int length);

  /*
   * @ brief 判断是否有后续帧
   */
  virtual bool verifyFrameNext(const unsigned char* frame, int length);

  /*
   * @ brief print frame
   */
  virtual void print(bool hex);

  std::string getErrorMsg();

  std::string getDescription();

};

#endif