#ifndef _XML_PARSER_H_
#define _XML_PARSER_H_

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>

#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp> 
#include <xercesc/util/XMLUni.hpp>

#include <xercesc/dom/DOM.hpp>  
#include <xercesc/dom/DOMDocument.hpp>  
#include <xercesc/dom/DOMDocumentType.hpp>  
#include <xercesc/dom/DOMElement.hpp>  
#include <xercesc/dom/DOMImplementation.hpp>  
#include <xercesc/dom/DOMImplementationLS.hpp>  
#include <xercesc/dom/DOMNodeIterator.hpp>  
#include <xercesc/dom/DOMNodeList.hpp>  
#include <xercesc/dom/DOMText.hpp>  
#include <xercesc/dom/DOMAttr.hpp>  
#include <xercesc/parsers/XercesDOMParser.hpp> 
#include <xercesc/util/OutOfMemoryException.hpp>

#include "src/object.h"
#include "src/xml_parse_handler.h"
//
//#ifdef _DEBUG
//#pragma comment( lib, "xerces-c_3D.lib" )
//#else
//#pragma comment( lib, "xerces-c_3.lib" )
//#endif

XERCES_CPP_NAMESPACE_USE


typedef XERCES_CPP_NAMESPACE::DOMDocument DOMDOC;

#ifndef __MSXML_LIBRARY_DEFINED__
#define __MSXML_LIBRARY_DEFINED__
#endif

class ick_api XmlParser : public IObject
{
private:
  std::string m_strConfigPath;

public:
  XmlParser();
  ~XmlParser();

private:
  XercesDOMParser* m_parser;
  DOMElement* LoadIO(DOMElement* ele);
  DOMElement* LoadControlObject(DOMElement* ele);
  DOMElement* loadXmlFile(std::string filePath);
public:
  int loadConfigFile();
  void setConfigPath(const std::string& configPath);
};



// ---------------------------------------------------------------------------
//  This is a simple class that lets us do easy (though not terribly efficient)
//  trancoding of XMLCh data to local code page for display.
// ---------------------------------------------------------------------------
class StrX
{
public:
  // -----------------------------------------------------------------------
  //  Constructors and Destructor
  // -----------------------------------------------------------------------
  StrX(const XMLCh* const toTranscode)
  {
    // Call the private transcoding method
    fLocalForm = XMLString::transcode(toTranscode);
    fXerceForm = NULL;
  }

  StrX(const char* const toTranscode)
  {
    fLocalForm = NULL;
    fXerceForm = XMLString::transcode(toTranscode);
  }

  ~StrX()
  {
    if(fLocalForm != NULL) XMLString::release(&fLocalForm);
    if (fXerceForm != NULL) XMLString::release(&fXerceForm);
  }


  // -----------------------------------------------------------------------
  //  Getter methods
  // -----------------------------------------------------------------------
  const char* localForm() const
  {
    return fLocalForm;
  }

  const XMLCh* xerceForm() const
  {
    return fXerceForm;
  }

private:
  // -----------------------------------------------------------------------
  //  Private data members
  //
  //  fLocalForm
  //      This is the local code page form of the string.
  // -----------------------------------------------------------------------
  char*   fLocalForm;
  XMLCh*  fXerceForm;
};

inline XERCES_STD_QUALIFIER ostream& operator<<(XERCES_STD_QUALIFIER ostream& target, const StrX& toDump)
{
  target << toDump.localForm();
  return target;
}

#endif

