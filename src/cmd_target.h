#ifndef _CMD_TARGET_H_
#define _CMD_TARGET_H_

#include "src/managed_object.h"
#include <vector>
#include <functional>

class CmdTarget;
typedef void (CmdTarget::*AF_MSG)();

union MethodsMap
{
    AF_MSG pfn; // generic member function pointer, is similar to void*
    
    // specific type safe variants
    //
    void (CmdTarget::*pfn_v)(void);
    void (CmdTarget::*pfn_b)(bool);
    void (CmdTarget::*pfn_i)(int);
    void (CmdTarget::*pfn_d)(double);
    void (CmdTarget::*pfn_s)(const std::string&);
    
    void (CmdTarget::*pfn_ii)(int, int);
    void (CmdTarget::*pfn_dd)(double, double);
    void (CmdTarget::*pfn_ss)(const std::string&, const std::string&);
    
    void (CmdTarget::*pfn_id)(int, double);
    void (CmdTarget::*pfn_di)(double, int);
    
    void (CmdTarget::*pfn_ds)(double, const std::string&);
    void (CmdTarget::*pfn_sd)(const std::string&, double);
    
    void (CmdTarget::*pfn_is)(int, const std::string&);
    void (CmdTarget::*pfn_si)(const std::string&, int);
    
    void (CmdTarget::*pfn_iii)(int, int, int);
    void (CmdTarget::*pfn_ddd)(double, double, double);
    void (CmdTarget::*pfn_sss)(const std::string&, const std::string&, const std::string&);
    
    void (CmdTarget::*pfn_iid)(int, int, double);
    void (CmdTarget::*pfn_iis)(int, int, const std::string&);
    
    void (CmdTarget::*pfn_ddi)(double, double, int);
    void (CmdTarget::*pfn_dds)(double, double, const std::string&);
    
    void (CmdTarget::*pfn_ssi)(const std::string&, const std::string&, int);
    void (CmdTarget::*pfn_ssd)(const std::string&, const std::string&, double);
    
    void (CmdTarget::*pfn_idi)(int, double, int);
    void (CmdTarget::*pfn_idd)(int, double, double);
    void (CmdTarget::*pfn_ids)(int, double, const std::string&);
    
    void (CmdTarget::*pfn_isi)(int, const std::string&, int);
    void (CmdTarget::*pfn_isd)(int, const std::string&, double);
    void (CmdTarget::*pfn_iss)(int, const std::string&, const std::string&);
    
    void (CmdTarget::*pfn_dii)(double, int, int);
    void (CmdTarget::*pfn_did)(double, int, double);
    void (CmdTarget::*pfn_dis)(double, int, const std::string&);
    
    void (CmdTarget::*pfn_dsi)(double, const std::string&, int);
    void (CmdTarget::*pfn_dsd)(double, const std::string&, double);
    void (CmdTarget::*pfn_dss)(double, const std::string&, const std::string&);
    
    void (CmdTarget::*pfn_sii)(const std::string&, int, int);
    void (CmdTarget::*pfn_sid)(const std::string&, int, double);
    void (CmdTarget::*pfn_sis)(const std::string&, int, const std::string&);   
    
    void (CmdTarget::*pfn_sdi)(const std::string&, double, int);
    void (CmdTarget::*pfn_sdd)(const std::string&, double, double);
    void (CmdTarget::*pfn_sds)(const std::string&, double, const std::string&);  
};


enum METHOD_TYPE
{
    METHOD_END = 0,
    
    // 1 param
    //
    METHOD_V, // (void)
    METHOD_B, // (bool)
    METHOD_I, // (int)
    METHOD_D, // (double)
    METHOD_S, // (string)
    
    // 2 params
    //
    METHOD_II, // (int, int)
    METHOD_DD, // (double, double)
    METHOD_SS, // (string, string)
    
    METHOD_ID, // (int, double)
    METHOD_DI, // (double, int)
    
    METHOD_DS, // (double, string)
    METHOD_SD, // (string, double)
    
    METHOD_IS, // (int, string)
    METHOD_SI, // (string, int)
    
    // 3 params
    //
    METHOD_III, // (int, int, int)
    METHOD_DDD, // (double, double, double)
    METHOD_SSS, // (string, string, string)
    
    METHOD_IID, // (int, int, double)
    METHOD_IIS, // (int, int, string)
    
    METHOD_DDI, // (double, double, int)
    METHOD_DDS, // (double, double, string)
    
    METHOD_SSI, // (string, string, int)
    METHOD_SSD, // (string, string, double)

    METHOD_IDI, // (int, double, int)
    METHOD_IDD, // (int, double, double)
    METHOD_IDS, // (int, double, string)
    
    METHOD_ISI, // (int, string, int)
    METHOD_ISD, // (int, string, double)
    METHOD_ISS, // (int, string, string)
    
    METHOD_DII, // (double, int, int)
    METHOD_DID, // (double, int, double)
    METHOD_DIS, // (double, int, string)
    
    METHOD_DSI, // (double, string, int)
    METHOD_DSD, // (double, string, double)
    METHOD_DSS, // (double, string, string)
    
    METHOD_SII, // (string, int, int)
    METHOD_SID, // (string, int, double)
    METHOD_SIS, // (string, int, string)
    
    METHOD_SDI, // (string, double, int)
    METHOD_SDD, // (string, double, double)
    METHOD_SDS, // (string, double, string)
};


// typedef void (CmdTarget::*FUNC_REG)();

typedef union ChannelRegistFunction
{
  bool (CmdTarget::*pfn)();

  bool (CmdTarget::*pfn_ri)(int channelNumber, int* value);                  // read only int, for read
  bool (CmdTarget::*pfn_rd)(int channelNumber, double* value);               // read only double, for read
  bool (CmdTarget::*pfn_rs)(int channelNumber, std::string& value);          // read only string, for read
  bool (CmdTarget::*pfn_wi)(int channelNumber, int value);
  bool (CmdTarget::*pfn_wd)(int channelNumber, double value);
  bool (CmdTarget::*pfn_ws)(int channelNumber, const std::string& value);

}FUNC_REG;


struct MSG_ENTITY
{
    std::string m_msg;
    
    int m_msgType;
    
  MethodsMap methodMap;
  
  MSG_ENTITY()                                                                                                         { m_msg = ""; m_msgType = METHOD_END;  methodMap.pfn = NULL; }

  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)())                                               { m_msg = msg; m_msgType = method_type;  methodMap.pfn = pfn; }

  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(bool))                                           { m_msg = msg; m_msgType = method_type;  methodMap.pfn_b = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(int))                                            { m_msg = msg; m_msgType = method_type;  methodMap.pfn_i = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(double))                                         { m_msg = msg; m_msgType = method_type;  methodMap.pfn_d = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(const std::string&))                             { m_msg = msg; m_msgType = method_type;  methodMap.pfn_s = pfn; }
  
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(int,int))                                        { m_msg = msg; m_msgType = method_type;  methodMap.pfn_ii = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(double,double))                                  { m_msg = msg; m_msgType = method_type;  methodMap.pfn_dd = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(const std::string&, const std::string&))         { m_msg = msg; m_msgType = method_type;  methodMap.pfn_ss = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(int,double))                                     { m_msg = msg; m_msgType = method_type;  methodMap.pfn_id = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(double, int))                                    { m_msg = msg; m_msgType = method_type;  methodMap.pfn_di = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(int, const std::string&))                        { m_msg = msg; m_msgType = method_type;  methodMap.pfn_is = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(const std::string&, int))                        { m_msg = msg; m_msgType = method_type;  methodMap.pfn_si = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(double, const std::string&))                     { m_msg = msg; m_msgType = method_type;  methodMap.pfn_ds = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(const std::string&, double))                     { m_msg = msg; m_msgType = method_type;  methodMap.pfn_sd = pfn; }

  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(int,int,int))                                    { m_msg = msg; m_msgType = method_type;  methodMap.pfn_iii = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(double,double,double))                           { m_msg = msg; m_msgType = method_type;  methodMap.pfn_ddd = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(const std::string&, const std::string&, const std::string&)) 
                                                                                                                       { m_msg = msg; m_msgType = method_type;  methodMap.pfn_sss = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(int,int,double))                                 { m_msg = msg; m_msgType = method_type;  methodMap.pfn_iid = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(int,int,const std::string&))                     { m_msg = msg; m_msgType = method_type;  methodMap.pfn_iis = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(double, double, const std::string&))             { m_msg = msg; m_msgType = method_type;  methodMap.pfn_dds = pfn; }  
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(const std::string&, const std::string&, int))    { m_msg = msg; m_msgType = method_type;  methodMap.pfn_ssi = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(const std::string&, const std::string&, double)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_ssd = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(int, double, int))                               { m_msg = msg; m_msgType = method_type;  methodMap.pfn_idi = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(int, double, double))                            { m_msg = msg; m_msgType = method_type;  methodMap.pfn_idd = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(int, double, const std::string&))                { m_msg = msg; m_msgType = method_type;  methodMap.pfn_ids = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(int, const std::string&, int))                   { m_msg = msg; m_msgType = method_type;  methodMap.pfn_isi = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(int, const std::string&,double))                 { m_msg = msg; m_msgType = method_type;  methodMap.pfn_isd = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(int, const std::string&, const std::string&))    { m_msg = msg; m_msgType = method_type;  methodMap.pfn_iss = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(double, int, int))                               { m_msg = msg; m_msgType = method_type;  methodMap.pfn_dii = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(double, int, double))                            { m_msg = msg; m_msgType = method_type;  methodMap.pfn_did = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(double, int, const std::string&))                { m_msg = msg; m_msgType = method_type;  methodMap.pfn_dis = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(double, const std::string&, int))                { m_msg = msg; m_msgType = method_type;  methodMap.pfn_dsi = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(double, const std::string&, double))             { m_msg = msg; m_msgType = method_type;  methodMap.pfn_dsd = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(double, const std::string&, const std::string&)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_dss = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(const std::string&, int, int))                   { m_msg = msg; m_msgType = method_type;  methodMap.pfn_sii = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(const std::string&, int, double))                { m_msg = msg; m_msgType = method_type;  methodMap.pfn_sid = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(const std::string&, int, const std::string&))    { m_msg = msg; m_msgType = method_type;  methodMap.pfn_sis = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(const std::string&, double, int))                { m_msg = msg; m_msgType = method_type;  methodMap.pfn_sdi = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(const std::string&, double, double))             { m_msg = msg; m_msgType = method_type;  methodMap.pfn_sdd = pfn; }
  MSG_ENTITY(std::string msg, int method_type, void (CmdTarget::*pfn)(const std::string&, double, const std::string&)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_sds = pfn; }
  
};

struct MSG_MAP
{
    const MSG_MAP *m_baseMap;                // pointer of parent class's message map
    const MSG_ENTITY *m_MsgMapEntity;        // pointer of the current class's message map
};

#define DECLARE_MESSAGE_LIST                                       \
private:                                                          \
    static const MSG_ENTITY m_msgEntry[];                         \
protected:                                                        \
    static const MSG_MAP m_msgMap;                                \
public:                                                           \
    virtual const MSG_MAP* getMsgMap();          

#define IMPLEMENT_MESSAGE_LIST(class_name, base_class_name)        \
    const MSG_MAP class_name::m_msgMap =                          \
    {                                                             \
      &base_class_name::m_msgMap,                               \
      class_name::m_msgEntry                                    \
    };                                                            \
                                                                  \
    const MSG_MAP* class_name::getMsgMap()                        \
    {                                                             \
      return &class_name::m_msgMap;                             \
    }

#define SET_DEFINE(method_name)                                   \
    void method_name(const std::string& fullPath);

#define SET(class_name, method_name, variable_name, variable_type)                                            \
  void class_name::method_name(const std::string& fullPath)                                                 \
  {                                                                                                         \
    try                                                                                                   \
    {                                                                                                     \
      variable_name = dynamic_cast<variable_type>(ObjectManager::getInstance()->getObject(fullPath));   \
      if (variable_name == NULL)                                                                        \
      {                                                                                                 \
        std::string msg = std::string("Exception occured in ") + std::string(getFullName()) +         \
          std::string(": set ") + std::string(#method_name) + " failed!";                           \
        Logger::getInstance()->Error(msg);                                                            \
        throw msg;                                                                                    \
      }                                                                                                 \
    }                                                                                                     \
    catch(...)                                                                                            \
    {                                                                                                     \
        std::string msg = std::string("Exception occured in ") + std::string(getFullName()) +             \
        std::string(": set ") + std::string(#method_name) + " failed!";                               \
      Logger::getInstance()->Error(msg);                                                                \
      throw msg;                                                                                        \
    }                                                                                                     \
    }

#define BEGIN_MESSAGE_LIST(class_name) const MSG_ENTITY class_name::m_msgEntry[] = {

#define SET_V(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_V, (void (CmdTarget::*)(void))(&class_name::method_name) ),
#define SET_I(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_I, (void (CmdTarget::*)(int))(&class_name::method_name) ),
#define SET_B(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_B, (void (CmdTarget::*)(bool))(&class_name::method_name) ),
#define SET_D(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_D, (void (CmdTarget::*)(double))(&class_name::method_name) ),
#define SET_S(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_S, (void (CmdTarget::*)(const std::string&))(&class_name::method_name) ),

#define SET_II(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_II, (void (CmdTarget::*)(int,int))(&class_name::method_name) ),
#define SET_DD(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_DD, (void (CmdTarget::*)(double,double))(&class_name::method_name) ),
#define SET_SS(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_SS, (void (CmdTarget::*)(const std::string&,const std::string&))(&class_name::method_name) ),
#define SET_ID(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_ID, (void (CmdTarget::*)(int,double))(&class_name::method_name) ),
#define SET_DI(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_DI, (void (CmdTarget::*)(double,int))(&class_name::method_name) ),
#define SET_IS(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_IS, (void (CmdTarget::*)(int,const std::string&))(&class_name::method_name) ),
#define SET_SI(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_SI, (void (CmdTarget::*)(const std::string&,int))(&class_name::method_name) ),
#define SET_DS(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_DS, (void (CmdTarget::*)(double,const std::string))(&class_name::method_name) ),
#define SET_SD(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_SD, (void (CmdTarget::*)(const std::string,double))(&class_name::method_name) ),

#define SET_III(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_III, (void (CmdTarget::*)(int,int,int))(&class_name::method_name) ),
#define SET_DDD(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_DDD, (void (CmdTarget::*)(double,double,double))(&class_name::method_name) ),
#define SET_SSS(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_SSS, (void (CmdTarget::*)(const std::string&,const std::string&,const std::string&))(&class_name::method_name) ),
#define SET_IID(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_IID, (void (CmdTarget::*)(int,int,double))(&class_name::method_name) ),
#define SET_IIS(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_IIS, (void (CmdTarget::*)(int,int,const std::string))(&class_name::method_name) ),
#define SET_DDI(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_DDI, (void (CmdTarget::*)(double,double,int))(&class_name::method_name) ),
#define SET_DDS(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_DDS, (void (CmdTarget::*)(double,double,const std::string))(&class_name::method_name) ),
#define SET_SSI(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_SSI, (void (CmdTarget::*)(const std::string&,const std::string&,int))(&class_name::method_name) ),
#define SET_SSD(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_SSD, (void (CmdTarget::*)(const std::string&,const std::string&,double))(&class_name::method_name) ),
#define SET_SDD(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_SDD, (void (CmdTarget::*)(const std::string&,double,double))(&class_name::method_name) ),

#define SET_IDI(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_IDI, (void (CmdTarget::*)(int,double,int))(&class_name::method_name) ),
#define SET_IDD(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_IDD, (void (CmdTarget::*)(int,double,double))(&class_name::method_name) ),
#define SET_IDS(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_IDS, (void (CmdTarget::*)(int,double,const std::string))(&class_name::method_name) ),
#define SET_DII(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_DII, (void (CmdTarget::*)(double,int,int))(&class_name::method_name) ),
#define SET_DID(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_DID, (void (CmdTarget::*)(double,int,double))(&class_name::method_name) ),
#define SET_DIS(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_DIS, (void (CmdTarget::*)(double,int,const std::string))(&class_name::method_name) ),
#define SET_SII(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_SII, (void (CmdTarget::*)(const std::string&,int,int))(&class_name::method_name) ),
#define SET_SID(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_SID, (void (CmdTarget::*)(const std::string,int,double))(&class_name::method_name) ),
#define SET_SIS(method_name, class_name)       MSG_ENTITY( #method_name, METHOD_SIS, (void (CmdTarget::*)(const std::string&,int,const std::string&))(&class_name::method_name) ),


#define SET_END(method_name, class_name)       MSG_ENTITY(),

#define END_MESSAGE_LIST  MSG_ENTITY() };




class ick_api CmdTarget : public ManagedObject
{
  DECLARE_CLASS_DYNAMIC(CmdTarget)
  DECLARE_MESSAGE_LIST
public:
  CmdTarget(void);
  virtual ~CmdTarget(void);


  void dispatchMsg(const std::string& msgName, const std::string& params);
  
  std::vector<std::string> split(std::string str, std::string pattern);
  std::string trim(std::string s);
  
};

#endif

