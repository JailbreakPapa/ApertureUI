#pragma once
#include <Foundation/Logging/Log.h>
#include <Foundation/Basics.h>

// Configure the DLL Import/Export Define
#if NS_ENABLED(NS_COMPILE_ENGINE_AS_DLL)
#  ifdef BUILDSYSTEM_BUILDING_APERTUREUIV8_LIB
#    define NS_V8ENGINE_DLL NS_DECL_EXPORT
#    define NS_V8ENGINE_DLL_FRIEND NS_DECL_EXPORT_FRIEND
#  else
#    define NS_V8ENGINE_DLL NS_DECL_IMPORT
#    define NS_V8ENGINE_DLL_FRIEND NS_DECL_IMPORT_FRIEND
#  endif
#else
#  define NS_V8ENGINE_DLL
#  define NS_V8ENGINE_DLL_FRIEND
#endif

#include <APCore/Interfaces/APCPlatform.h>
// V8 Includes.
#include <libplatform/libplatform.h>
#include <v8-inspector.h>
#include <v8-persistent-handle.h>
#include <v8-version-string.h>
#include <v8-source-location.h>
#include <v8.h>

#define NS_ISE_JSI_CHECK(CHECKER) \
if(!CHECKER)  \
{                     \
  nsLog::Error("V8Engine: JSB Error: A Function/Value Check Failed! File: {0} Function: {1}", NS_SOURCE_FILE, NS_SOURCE_FUNCTION); \
}                      
