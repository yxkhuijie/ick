#ifndef _XML_PARSE_HANDLER_H_
#define _XML_PARSE_HANDLER_H_


#include "src/object.h"

#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/Attributes.hpp>

XERCES_CPP_NAMESPACE_USE

class ick_api XmlParseHandler : public DefaultHandler, public IObject
{
public:
  XmlParseHandler();
  ~XmlParseHandler();

public:

  void startDocument();

  void startElement(
    const   XMLCh* const    uri,
    const   XMLCh* const    localname,
    const   XMLCh* const    qname,
    const   Attributes&     attrs
  );

  void characters(
    const   XMLCh* const    chars
    , const XMLSize_t       length
  );

  void endElement(
    const XMLCh* const uri,
    const XMLCh* const localname,
    const XMLCh* const qname
  );

  void endDocument();

  void fatalError(const SAXParseException&);
};

#endif

