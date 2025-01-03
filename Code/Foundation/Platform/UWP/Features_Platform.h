#pragma once

/// \file

#define NS_PLATFORM_NAME "UWP"

#undef NS_PLATFORM_WINDOWS
#undef NS_PLATFORM_WINDOWS_UWP
#undef NS_PLATFORM_WINDOWS_DESKTOP

#define NS_PLATFORM_WINDOWS NS_ON
#define NS_PLATFORM_WINDOWS_UWP NS_ON
#define NS_PLATFORM_WINDOWS_DESKTOP NS_OFF

#undef NS_PLATFORM_LITTLE_ENDIAN
#define NS_PLATFORM_LITTLE_ENDIAN NS_ON

#undef NS_PLATFORM_PATH_SEPARATOR
#define NS_PLATFORM_PATH_SEPARATOR '\\'

/// If set to 1, the POSIX file implementation will be used. Otherwise a platform specific implementation must be available.
#undef NS_USE_POSIX_FILE_API
#define NS_USE_POSIX_FILE_API NS_ON

/// Iterating through the file system is supported
#undef NS_SUPPORTS_FILE_ITERATORS
#define NS_SUPPORTS_FILE_ITERATORS NS_OFF

/// Getting the stats of a file (modification times etc.) is supported.
#undef NS_SUPPORTS_FILE_STATS
#define NS_SUPPORTS_FILE_STATS NS_ON

/// Directory watcher is supported on non uwp platforms.
#undef NS_SUPPORTS_DIRECTORY_WATCHER
#define NS_SUPPORTS_DIRECTORY_WATCHER NS_OFF

/// Memory mapping a file is supported.
#undef NS_SUPPORTS_MEMORY_MAPPED_FILE
#define NS_SUPPORTS_MEMORY_MAPPED_FILE NS_OFF

/// Shared memory IPC is supported.
#undef NS_SUPPORTS_SHARED_MEMORY
#define NS_SUPPORTS_SHARED_MEMORY NS_OFF

/// Whether dynamic plugins (through DLLs loaded/unloaded at runtime) are supported
#undef NS_SUPPORTS_DYNAMIC_PLUGINS
#define NS_SUPPORTS_DYNAMIC_PLUGINS NS_ON

/// Whether applications can access any file (not sandboxed)
#undef NS_SUPPORTS_UNRESTRICTED_FILE_ACCESS
#define NS_SUPPORTS_UNRESTRICTED_FILE_ACCESS NS_OFF

/// Whether file accesses can be done through paths that do not match exact casing
#undef NS_SUPPORTS_CASE_INSENSITIVE_PATHS
#define NS_SUPPORTS_CASE_INSENSITIVE_PATHS NS_ON

/// Whether starting other processes is supported.
#undef NS_SUPPORTS_PROCESSES
#define NS_SUPPORTS_PROCESSES NS_OFF

// SIMD support
#undef NS_SIMD_IMPLEMENTATION

#if NS_ENABLED(NS_PLATFORM_ARCH_X86)
#  define NS_SIMD_IMPLEMENTATION NS_SIMD_IMPLEMENTATION_SSE
#  define NS_SSE_LEVEL NS_SSE_41
#elif NS_ENABLED(NS_PLATFORM_ARCH_ARM)
#  define NS_SIMD_IMPLEMENTATION NS_SIMD_IMPLEMENTATION_FPU
#else
#  error "Unknown architecture."
#endif

#ifdef _WIN64
#  undef NS_PLATFORM_64BIT
#  define NS_PLATFORM_64BIT NS_ON
#else
#  undef NS_PLATFORM_32BIT
#  define NS_PLATFORM_32BIT NS_ON
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#  define _CRT_SECURE_NO_WARNINGS
#endif
