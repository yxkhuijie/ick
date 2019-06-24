#ifndef _OBJECT_MANAGER_H_
#define _OBJECT_MANAGER_H_

#include <map>
#include <string>

#include "src/untyped_data.h"
#include "src/control_object.h"
#include "src/thread.h"

struct IckStormNodeInfo
{
  char subNodeName[4096];
  int  subNodeSize;
  char subNodeType[20];
};

typedef void(*NodeInfoCallBack)(const char* nodeName, IckStormNodeInfo* nodeInfo);
typedef void(*ObjectCallBack)(std::string nodeName);
typedef void(*ObjectBeginMakeCallBack)(int channelIndex, int channelCount, std::string channelName);
typedef void(*ObjectAfterMakeCallBack)(int channelIndex, int channelCount, std::string channelName);
typedef void(*ObjectBeginInitCallBack)(int channelIndex, int channelCount, std::string channelName);
typedef void(*ObjectAfterInitCallBack)(int channelIndex, int channelCount, std::string channelName);
typedef void(*ObjectBeginVerifyCallBack)(int channelIndex, int channelCount, std::string channelName);
typedef void(*ObjectAfterVerifyCallBack)(int channelIndex, int channelCount, std::string channelName);
typedef void(*ObjectBeginStartupCallBack)(int channelIndex, int channelCount, std::string channelName);
typedef void(*ObjectAfterStartupCallBack)(int channelIndex, int channelCount, std::string channelName);

class ick_api ObjectManager : public IThread
{
private:

  class NodeName
  {
  private:

    std::string m_strNodeName;

    std::map<std::string, std::string> m_nodeList;

  public:
    void addNodeName(std::string nodeName)
    {
      if (m_nodeList.find(nodeName) == m_nodeList.end())
      {
        m_nodeList.insert(std::pair<std::string, std::string>(nodeName, nodeName));
      }
    }

    std::list<std::string> getNodeList()
    {
      std::list<std::string> nodeList;
      std::map<std::string, std::string>::iterator it;
      for (it = m_nodeList.begin(); it != m_nodeList.end(); ++it)
      {
        nodeList.push_back(it->first);
      }
      return nodeList;
    }

    std::string toString(std::string seperate=",")
    {
      std::string str = "";
      std::map<std::string, std::string>::iterator it;
      int index = 0;
      for (it = m_nodeList.begin(); it != m_nodeList.end(); ++it)
      {
        str += (it->first) + ((index == m_nodeList.size() - 1)?"":seperate);
        index++;
      }
      return str;
    }
  };

private:

  static ObjectManager* m_instance;

private:
  
  ObjectManager(void);

  ~ObjectManager(void);

  std::map<std::string, ControlObject*> m_entites;

  std::map<std::string, UntypedData*> m_datas;

  std::map<std::string, NodeName*> m_dataNodeNames;

  std::map<std::string, NodeName*> m_serviceNodeNames;

  //  data update interval, default value is 1000(1s)
  int m_interval;

  std::list<NodeInfoCallBack> m_nodeInfoCallBacks;

  std::list<ObjectBeginMakeCallBack> m_objectBeginMakeCallBacks;
  std::list<ObjectAfterMakeCallBack> m_objectAfterMakeCallBacks;
  std::list<ObjectBeginInitCallBack> m_objectBeginInitCallBacks;
  std::list<ObjectAfterInitCallBack> m_objectAfterInitCallBacks;
  std::list<ObjectBeginVerifyCallBack> m_objectBeginVerifyCallBacks;
  std::list<ObjectAfterVerifyCallBack> m_objectAfterVerifyCallBacks;
  std::list<ObjectBeginStartupCallBack> m_objectBeginStartupCallBacks;
  std::list<ObjectAfterStartupCallBack> m_objectAfterStartupCallBacks;

public:
  
  
  static ObjectManager* getInstance();

  // 
  void registerObject(ControlObject* entity, std::string fullName);

  void registerObject(UntypedData* entity, std::string fullName);

  void registerObject(std::string className, std::string fullName);

  bool isObjectRegisted(std::string fullName);

  void createAlias(ControlObject* object, std::string alias);

  void createAlias(UntypedData* object, std::string alias);

  void createAlias(std::string fullName, std::string alias);

    ManagedObject* getObject(std::string fullName);

  std::map<std::string, UntypedData*>* getDatas();

  std::list<std::string> getChannelNodeList(std::string node);

  std::list<std::string> getServiceNodeList(std::string node);

  void addNodeInfoCallBack(NodeInfoCallBack callback);

  std::list<NodeInfoCallBack> getNodeInfoCallBack();

  void addBeginMakeCallBack(ObjectBeginMakeCallBack callback);

  void addAfterMakeCallBack(ObjectAfterMakeCallBack callback);

  void addBeginInitCallBack(ObjectBeginInitCallBack callback);

  void addAfterInitCallBack(ObjectAfterInitCallBack callback);

  void addBeginVerifyCallBack(ObjectBeginVerifyCallBack callback);

  void addAfterVerifyCallBack(ObjectAfterVerifyCallBack callback);

  void addBeginStartupCallBack(ObjectBeginStartupCallBack callback);

  void addAfterStartupCallBack(ObjectAfterStartupCallBack callback);

  /*
   * @ brief get special node number data objects 
   * @ param nodeNumber: data's node number you want to get 
   */
  std::map<std::string, UntypedData*> getDatas(int nodeNumber);

  void startNamespace();
    
  void shutdownNamespace();

  virtual void execute();

  void setInterval(int interval);
};  

#endif

