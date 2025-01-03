#pragma once
/*
 *   Copyright (c) 2023 Watch Dogs LLC
 *   All rights reserved.
 *   You are only allowed access to this code, if given WRITTEN permission by WD Studios L.L.C.
 */

#include <Foundation/Basics.h>
#include <Foundation/Reflection/Reflection.h>
#include <Foundation/SimdMath/SimdMath.h>
#include <Foundation/Types/RefCounted.h>
#include <Foundation/Types/UniquePtr.h>
#include <Foundation/Types/SharedPtr.h>
#include <Foundation/Containers/DynamicArray.h>

// STD INCLUDES
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <functional>

/// NOTE: The DLL/PCH Header should always be included last.
#include <APHTML/APEngineDLL.h>
