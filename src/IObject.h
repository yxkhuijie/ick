/*! @file
********************************************************************************
<PRE>
模块名       : <文件所属的模块名称>
文件名       : <文件名>
相关文件     : <与此文件相关的其它文件>
文件实现功能 : <描述该文件实现的主要功能>
作者         : <作者部门和姓名>
版本         : <当前版本号>
--------------------------------------------------------------------------------
多线程安全性 : <是/否>[，说明]
异常时安全性 : <是/否>[，说明]
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录     :
日 期        版本     修改人              修改内容
YYYY/MM/DD   X.Y      <作者或修改者名>    <修改内容>
</PRE>
*******************************************************************************/

#ifndef _IOBJECT_H_
#define _IOBJECT_H_

#ifdef _WIN32
#ifdef ICK_EXPORTS
#    if defined(ICK_STATIC_LIBRARY)
#        define ick_api  
#    else
#        define ick_api __declspec(dllexport)
#    endif
#else  
#    if defined(ICK_STATIC_LIBRARY)
#        define ick_api  
#    else
#        define ick_api __declspec(dllimport)
#    endif  
#endif
#elif __linux__
#define ick_api
#endif


#if (defined(_WIN32) || defined(WIN32))
     // define something for Windows (32-bit and 64bit, this part is common)
     #ifdef _WIN64
         // define something for Windows (64-bit only)
         #define __windows__
     #else
   // define something for Windows (32-bit only)
         #define __windows__
     #endif
#elif __APPLE_
     #include "TargetConditionals.h"
     #if TARGET_IPHONE_SIMULATOR
          // iOS Simulator
     #elif TARGET_OS_IPHONE
          // iOS device
     #elif TARGET_OS_MAC
          // Other kinds of Mac OS
     #else
          #error Unknown App platform
     #endif
#elif __ANDROID__
     // android
#elif __linux__
     // linux
#elif __unix__    // all unices not caught above
     // unix
#elif defined(_POSIX_VERSION)
     // POSIX
#else
     #error Unknown compiler
#endif


#include <stdio.h>
#include <iostream>
#include <string>
#include <memory>

#if defined(WIN32)
  #pragma warning( disable: 4251 )
  #pragma warning( disable: 4482 )
#endif

#ifdef ICK_TIME_DEBUG
#   define TIME_DEBUG(message)       \
  std::cout << TimeConverter::getCurrentTimeAsStr() << ": " << std::string(message) << std::endl;
#else 
#   define TIME_DEBUG(message) 
#endif

#ifndef ICK_NAMESPACE_BEGIN
#define ICK_NAMESPACE_BEGIN namespace Ick { using namespace Ick;
#endif

#ifndef ICK_NAMESPACE_END
#define ICK_NAMESPACE_END }
#endif

#if defined(__linux__)
typedef unsigned int HANDLE;
#endif
#ifndef INT_MIN
#define INT_MIN (-2147483647-1)
#endif
#ifndef INT_MAX
#define INT_MAX 2147483647
#endif

//ICK_NAMESPACE_BEGIN

class ick_api IObject
{
public:

  IObject(void);

  virtual ~IObject(void);

  virtual std::string toString();
};

//ICK_NAMESPACE_END

#endif

