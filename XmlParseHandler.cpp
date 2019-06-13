#include "XmlParseHandler.h"



XmlParseHandler::XmlParseHandler()
{

}


XmlParseHandler::~XmlParseHandler()
{

}

void XmlParseHandler::startDocument()
{

}

void XmlParseHandler::endDocument()
{

}

void XmlParseHandler::startElement(
  const   XMLCh* const    uri,
  const   XMLCh* const    localname,
  const   XMLCh* const    qname,
  const   Attributes&     attrs
)
{
  std::wcout << "start element: uri: " << uri;
  std::wcout << ",  localname: " << localname;
  std::wcout << ",  qname: " << qname;
  std::wcout << std::endl;
}

void XmlParseHandler::characters(
  const   XMLCh* const    chars
  , const XMLSize_t       length
)
{

}

void XmlParseHandler::endElement(
  const XMLCh* const uri,
  const XMLCh* const localname,
  const XMLCh* const qname
)
{
  
}

void XmlParseHandler::fatalError(const SAXParseException&)
{

}
