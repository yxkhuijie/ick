#include "XmlParser.h"
#include "xercesc/parsers/SAXParser.hpp"

#include "ReadWriteInt.h"
#include "ReadWriteDouble.h"
#include "ReadWriteString.h"
#include "ReadOnlyInt.h"
#include "ReadOnlyDouble.h"
#include "ReadOnlyString.h"
#include "ObjectManager.h"
#include "ControlObject.h"



XmlParser::XmlParser()
{
  m_parser = NULL;
  m_strConfigPath = "";

  try 
  {
    XMLPlatformUtils::Initialize();
  }
  catch (xercesc::XMLException & toCatch)
  {
    XERCES_STD_QUALIFIER cerr << "Error during Xerces-c Initialization.\n"
      << "  Exception message:"
      << StrX(toCatch.getMessage()) << XERCES_STD_QUALIFIER endl;
    return;
    /*
     char* msg = XMLString::transcode(excp.getMessage());
     printf("XML toolkit initialization error: %s/n", msg);
     XMLString::release(&msg);
     */
  }

  //���� XercesDOMParser �������ڽ����ĵ�
  m_parser = new XercesDOMParser;
}


XmlParser::~XmlParser()
{
  try
  {
    delete m_parser;
    XMLPlatformUtils::Terminate();
  }
  catch (XMLException& excp)
  {
    char* msg = XMLString::transcode(excp.getMessage());
    printf("XML toolkit terminate error: %s\n", msg);
    XMLString::release(&msg);
  }
}

void XmlParser::setConfigPath(const std::string& configPath)
{
  this->m_strConfigPath = configPath;
}

DOMElement* XmlParser::loadXmlFile(std::string fileName)
{
  if (this->m_strConfigPath.compare("") == 0) return 0;
  std::string filePath = "";
  try
  {
    if (this->m_strConfigPath.at(m_strConfigPath.length() - 1) == '\\')
    {
      filePath = this->m_strConfigPath + fileName;
      m_parser->parse((this->m_strConfigPath + fileName).c_str());
    }
    else
    {
      filePath = this->m_strConfigPath + std::string("\\") + fileName;
      m_parser->parse((this->m_strConfigPath + std::string("\\") + fileName).c_str());
    }
  }
  catch (SAXException& e)
  {
    printf("msg:%s\n", XMLString::transcode(e.getMessage()));
    return NULL;
  }
  catch (XMLException& e)
  {
    printf("code:%d, msg:%s\n", e.getCode(), XMLString::transcode(e.getMessage()));
    return NULL;
  }
  catch (DOMException& e)
  {
    printf("code:%d, msg:%s\n", e.code, e.msg);
    return NULL;
  }

  DOMDOC* pDoc = NULL;

  try
  {
    pDoc = m_parser->getDocument();
  }
  catch (DOMLSException& e)
  {
    printf("code:%d, msg:%s\n", e.code, e.msg);
    return NULL;
  }
  catch (DOMException& e)
  {
    printf("code:%d, msg:%s\n", e.code, e.msg);
    return NULL;
  }

  if (pDoc == NULL)
  {
    std::string message = std::string("Load config file ") + filePath + std::string(" failure");
    Logger::getInstance()->Error(message);
    throw message;
  }

  // get root element
  DOMElement* root = pDoc->getDocumentElement();
  if (root == NULL)
  {
    return NULL;
  }
  return root;
}

int XmlParser::loadConfigFile()
{
  DOMElement* root = this->loadXmlFile("IO_config.xml");
  this->LoadIO(root);

  root = this->loadXmlFile("Control_config.xml");
  this->LoadControlObject(root);

  return 0;
}

DOMElement* XmlParser::LoadIO(DOMElement* ele)
{
  if (ele == NULL) return NULL;

  char* tagName = XMLString::transcode(ele->getTagName());
  char* namespaceURI = XMLString::transcode(ele->getNamespaceURI());
  char* nodeName = XMLString::transcode(ele->getNodeName());

  // �жϵ�ǰ�ڵ��Ƿ���type���ԡ�Bd��Ch�ӽڵ㣬����У����ʼ��IOͨ��
  if(*ele->getAttribute(StrX("dataType").xerceForm()) != *StrX("").xerceForm() && 
      *ele->getAttribute(StrX("accessMode").xerceForm()) != *StrX("").xerceForm())
  {
    std::string dataType = StrX(ele->getAttribute(StrX("dataType").xerceForm())).localForm();
    std::string accessMode = StrX(ele->getAttribute(StrX("accessMode").xerceForm())).localForm();
    std::string path = std::string("/") + StrX(ele->getNodeName()).localForm();
    DOMNode* parentNode = ele->getParentNode();
    
    while (parentNode != NULL && std::string(StrX(parentNode->getNodeName()).localForm()).compare("#document") != 0)
    {
      path = std::string("/") + StrX(parentNode->getNodeName()).localForm() + path;
      parentNode = parentNode->getParentNode();
    }

    UntypedData* untypedData = NULL;

    if (dataType.compare("I") == 0 && accessMode.compare("RW") == 0)
    {
      untypedData = new ReadWriteInt();
    }
    else if (dataType.compare("D") == 0 && accessMode.compare("RW") == 0)
    {
      untypedData = new ReadWriteDouble();
    }
    else if (dataType.compare("S") == 0 && accessMode.compare("RW") == 0)
    {
      untypedData = new ReadWriteString();
    }
    else if (dataType.compare("I") == 0 && accessMode.compare("R") == 0)
    {
      untypedData = new ReadOnlyInt();
    }
    else if (dataType.compare("D") == 0 && accessMode.compare("R") == 0)
    {
      untypedData = new ReadOnlyDouble();
    }
    else if (dataType.compare("S") == 0 && accessMode.compare("R") == 0)
    {
      untypedData = new ReadOnlyString();
    }
    else
    {

    }


    // 
    if(*ele->getAttribute(StrX("Bd").xerceForm()) != *StrX("").xerceForm())
    {
      std::string attrValue = StrX(ele->getAttribute(StrX("Bd").xerceForm())).localForm();
      if(untypedData != NULL) untypedData->setNodeNumber(::atoi(attrValue.c_str()));
    }

    if(*ele->getAttribute(StrX("Ch").xerceForm()) != *StrX("").xerceForm())
    {
      std::string attrValue = StrX(ele->getAttribute(StrX("Ch").xerceForm())).localForm();
      if(untypedData != NULL) untypedData->setChannelNumber(::atoi(attrValue.c_str()));
    }

    if(*ele->getAttribute(StrX("lparam").xerceForm()) != *StrX("").xerceForm())
    {
      std::string attrValue = StrX(ele->getAttribute(StrX("lparam").xerceForm())).localForm();
      if(untypedData != NULL) untypedData->setFirstParamValue(attrValue);
    }

    if(*ele->getAttribute(StrX("wparam").xerceForm()) != *StrX("").xerceForm())
    {
      std::string attrValue = StrX(ele->getAttribute(StrX("wparam").xerceForm())).localForm();
      if(untypedData != NULL) untypedData->setSecondParamValue(attrValue);
    }

    if(*ele->getAttribute(StrX("description").xerceForm()) != *StrX("").xerceForm())
    {
      std::string attrValue = StrX(ele->getAttribute(StrX("description").xerceForm())).localForm();
      if(untypedData != NULL) untypedData->setDescription(attrValue);
    }

    int nodeNumber    = 0;
    int channelNumber = 0;
    DOMElement* childElement = ele->getFirstElementChild();
    while (childElement != NULL)
    {
      if (*childElement->getNodeName() == *StrX("Bd").xerceForm())
      {
        nodeNumber = ::atoi(StrX(childElement->getTextContent()).localForm());
      }
      else if (*childElement->getNodeName() == *StrX("Ch").xerceForm())
      {
        channelNumber = ::atoi(StrX(childElement->getTextContent()).localForm());
      }
      else if (*childElement->getAttribute(StrX("type").xerceForm()) == *StrX("method").xerceForm())
      {
        untypedData->dispatchMsg(StrX(childElement->getNodeName()).localForm(), StrX(childElement->getTextContent()).localForm());
      }

      childElement = childElement->getNextElementSibling();
    }

    if ((nodeNumber == 0 && channelNumber == 0) && (untypedData->getNodeNumber() == 0 && untypedData->getChannelNumber() == 0))
    {
      std::string message = "object \"" + path + "\" register failed because node number must not be set to 0!";
      Logger::getInstance()->Error(message);
      throw message;
    }

    if (!(nodeNumber == 0 && channelNumber == 0))
    {
      untypedData->setNodeNumber(nodeNumber);
      untypedData->setChannelNumber(channelNumber);
    }

    if (untypedData != NULL)
    {
      ObjectManager::getInstance()->registerObject(untypedData, path);
      
    }
  }

  XMLString::release(&tagName);
  XMLString::release(&namespaceURI);
  XMLString::release(&nodeName);

  DOMElement* childNode = ele->getFirstElementChild();
  while (childNode != NULL)
  {
    int childElementCount = childNode->getChildElementCount();
    int childAttributeCount = childNode->getAttributes()->getLength();
    if (childElementCount > 0 || childAttributeCount > 0)
    {
      LoadIO(childNode);
    }
    else
    {
      tagName = XMLString::transcode(childNode->getTagName());
      namespaceURI = XMLString::transcode(childNode->getNamespaceURI());
      nodeName = XMLString::transcode(childNode->getNodeName());

      XMLString::release(&tagName);
      XMLString::release(&namespaceURI);
      XMLString::release(&nodeName);
    }
    childNode = childNode->getNextElementSibling();
  }
  return NULL;
}


DOMElement* XmlParser::LoadControlObject(DOMElement* ele)
{
  if (ele == NULL) return NULL;

  if (*ele->getAttribute(StrX("type").xerceForm()) != *StrX("").xerceForm() &&
    *ele->getAttribute(StrX("type").xerceForm()) == *StrX("instance").xerceForm())
  {
    std::string path = std::string("/") + StrX(ele->getNodeName()).localForm();
    DOMNode* parentNode = ele->getParentNode();

    while (parentNode != NULL && std::string(StrX(parentNode->getNodeName()).localForm()).compare("#document") != 0)
    {
      path = std::string("/") + StrX(parentNode->getNodeName()).localForm() + path;
      parentNode = parentNode->getParentNode();
    }

    std::string className = StrX(ele->getAttribute(StrX("class").xerceForm())).localForm();
    ObjectManager::getInstance()->registerObject(className, path);

    ControlObject* object = dynamic_cast<ControlObject*>(ObjectManager::getInstance()->getObject(path));

    DOMElement* childElement = ele->getFirstElementChild();
    while (childElement != NULL)
    {
      if (*childElement->getAttribute(StrX("type").xerceForm()) == *StrX("method").xerceForm())
      {
        object->dispatchMsg(StrX(childElement->getNodeName()).localForm(), StrX(childElement->getTextContent()).localForm());
      }

      childElement = childElement->getNextElementSibling();
    }
  }


  DOMElement* childNode = ele->getFirstElementChild();
  while (childNode != NULL)
  {
    int childElementCount = childNode->getChildElementCount();
    LoadControlObject(childNode);
    childNode = childNode->getNextElementSibling();
  }
  return NULL;
}

