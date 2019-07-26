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

#include "src/converter.h"
#include "src/logger.h"
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

class ick_api XmlParser : public IObject {
 private:
  std::string m_strConfigPath;

 public:
  XmlParser();
  ~XmlParser();

 private:
  XercesDOMParser* xerces_dom_parser_;
  DOMElement* LoadIO(DOMElement* ele);
  DOMElement* LoadControlObject(DOMElement* ele);
  DOMElement* loadXmlFile(std::string filePath);

 public:
  int loadConfigFile();
  // bool loadXmlFromString(const std::string& xml_data);
  void setConfigPath(const std::string& configPath);
};

class StrX {
 public:
  StrX() : xerce_form_(nullptr) {}
  StrX(const XMLCh* const toTranscode) {
    local_form_ = Converter::convertUtf16ToUtf8(toTranscode);
    xerce_form_ = nullptr;
  }

  StrX(const char* const toTranscode) {
    std::vector<uint16_t> xerce_form_vec;
    Converter::convertUtf8ToUtf16(toTranscode, &xerce_form_vec);
    xerce_form_ = new XMLCh[xerce_form_vec.size() + 1];
    XMLCh* psz_out = xerce_form_;
    for (const auto& it : xerce_form_vec) {
      *psz_out++ = it;
    }
    *psz_out = 0;
  }

  ~StrX() {
    if (xerce_form_ != nullptr) delete[] xerce_form_;
  }

  const char* toChar(const XMLCh* const src) {
    local_form_ = Converter::convertUtf16ToUtf8(src);
    return local_form_.c_str();
  }

  const XMLCh* toXMLCh(const char* const src) {
    std::vector<uint16_t> dst_vec;
    Converter::convertUtf8ToUtf16(src, &dst_vec);
    if (xerce_form_ != nullptr) {
      delete[] xerce_form_;
      xerce_form_ = nullptr;
    }
    xerce_form_ = new XMLCh[dst_vec.size() + 1];
    XMLCh* psz_out = xerce_form_;
    for (const auto& it : dst_vec) {
      *psz_out++ = it;
    }
    *psz_out = 0;
    return xerce_form_;
  }

  const char* localForm() const {
    return local_form_.c_str();
  }

  const XMLCh* xerceForm() const {
    return xerce_form_;
  }

 private:
  std::string local_form_;
  XMLCh* xerce_form_;
  char* char_form_;
};

inline XERCES_STD_QUALIFIER ostream& operator<<(XERCES_STD_QUALIFIER ostream& target, const StrX& toDump) {
  target << toDump.localForm();
  return target;
}
#endif
