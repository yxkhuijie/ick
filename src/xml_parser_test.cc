#include "src/converter.h"
#include "src/logger.h"
#include "src/xml_parser.h"

int main(int argc, char** argv) {
  // const XMLCh* tmp1 = StrX("dataType").xerceForm();
  // if (tmp1 == nullptr) {
  // 	Logger::getInstance()->Error("tmp1 nullptr");
  // 	return 0;
  // }
  /*
  XMLPlatformUtils::Initialize();
  XMLCh buff[9];
  StrX::convert("dataType", buff);

  for (int i = 0;i<9;i++) {
  	Logger::getInstance()->Info("i: " + std::to_string(buff[i]));
  }
  char* tmp2 = XMLString::transcode(buff);

  Logger::getInstance()->Info("tmp2: " + std::string(tmp2));

  const XMLCh* tmp1 = StrX("dataType").xerceForm();
  tmp2 = XMLString::transcode(tmp1);
  Logger::getInstance()->Info("tmp2: " + std::string(tmp2));
  */  
  /*
  const std::string& xml_data =
      "<?xml version='1.0'?>"
      "<root>"
      "  <setSmsTemplateText type='method'>【创信】你的验证码是：#code#，3分钟内有效！</setSmsTemplateText>"
      "</root>";
  std::string path = "/vagrant/github/ai/aitoolkit/server/config/control_config.xml";
  try {
  	XMLPlatformUtils::Initialize();
  	XercesDOMParser xerces_dom_parser;
    xerces_dom_parser.parse(path.c_str());
    DOMDOC* doc = xerces_dom_parser.getDocument();
    if (doc == nullptr) {
      Logger::getInstance()->Error("doc is null error!!");
      return 0;
    }
    DOMElement* root = doc->getDocumentElement();
    DOMElement* childNode = root->getFirstElementChild();
    while (childNode != NULL) {
      int childElementCount = childNode->getChildElementCount();
      int childAttributeCount = childNode->getAttributes()->getLength();
      if (childElementCount > 0 || childAttributeCount > 0) {
        const XMLCh* p = childNode->getTextContent();
        std::string s = Converter::convertUtf16ToUtf8(p);
        // Logger::getInstance()->Info("result: " + s);
        std::vector<uint16_t> data;
        Converter::convertUtf8ToUtf16(s, &data);
        bool equal = true;
        if (!data.empty()) {
          int i = 0;
          for (const auto& it : data) {
          	if (it != *(p + i)) {
          	  equal = false;
          	  Logger::getInstance()->Error("equal false!!");
          	  break;
          	}
          }
        } else {
          Logger::getInstance()->Error("equal empty!!");
          equal = false;
        }
        Logger::getInstance()->Info("equal value: " + std::to_string(equal));
      }
      childNode = childNode->getNextElementSibling();
    }
    return true;
  } catch (const SAXException& e) {
    // Logger::getInstance()->Error("load xml from string error: " + std::to_string(e.code) + ": " + std::string(e.msg));
  } catch (const XMLException& e) {
    // Logger::getInstance()->Error("load xml from string error: " + std::to_string(e.code) + ": " + std::string(e.msg));
  } catch (const DOMException& e) {
    // Logger::getInstance()->Error("load xml from string error: " + std::to_string(e.code) + ": " + std::string(e.msg));
  }
  */
}
