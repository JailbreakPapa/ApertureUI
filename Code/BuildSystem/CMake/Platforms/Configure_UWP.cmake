include("${CMAKE_CURRENT_LIST_DIR}/Configure_Default.cmake")

message(STATUS "Configuring Platform: UWP")

set_property(GLOBAL PROPERTY NS_CMAKE_PLATFORM_WINDOWS ON)
set_property(GLOBAL PROPERTY NS_CMAKE_PLATFORM_WINDOWS_UWP ON)

if(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
    set(NS_CMAKE_WINDOWS_SDK_VERSION ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION})
else()
    set(NS_CMAKE_WINDOWS_SDK_VERSION ${CMAKE_SYSTEM_VERSION})
    string(REGEX MATCHALL "\\." NUMBER_OF_DOTS "${NS_CMAKE_WINDOWS_SDK_VERSION}")
    list(LENGTH NUMBER_OF_DOTS NUMBER_OF_DOTS)

    if(NUMBER_OF_DOTS EQUAL 2)
        set(NS_CMAKE_WINDOWS_SDK_VERSION "${NS_CMAKE_WINDOWS_SDK_VERSION}.0")
    endif()
endif()

set_property(GLOBAL PROPERTY NS_CMAKE_WINDOWS_SDK_VERSION ${NS_CMAKE_WINDOWS_SDK_VERSION})

# #####################################
# ## General settings
# #####################################
set(NS_COMPILE_ENGINE_AS_DLL ${NS_WDCORESTDLIB_COMPILE_ENGINE_AS_DLL} CACHE BOOL "Whether to compile the code as a shared libraries (DLL).")
mark_as_advanced(FORCE NS_COMPILE_ENGINE_AS_DLL)

macro(ns_platform_pull_properties)

	get_property(NS_CMAKE_PLATFORM_WINDOWS GLOBAL PROPERTY NS_CMAKE_PLATFORM_WINDOWS)
	get_property(NS_CMAKE_PLATFORM_WINDOWS_UWP GLOBAL PROPERTY NS_CMAKE_PLATFORM_WINDOWS_UWP)
	get_property(NS_CMAKE_WINDOWS_SDK_VERSION GLOBAL PROPERTY NS_CMAKE_WINDOWS_SDK_VERSION)
	
endmacro()

macro(ns_platform_detect_generator)
	string(FIND ${CMAKE_VERSION} "MSVC" VERSION_CONTAINS_MSVC)

	if(${VERSION_CONTAINS_MSVC} GREATER -1)
		message(STATUS "CMake was called from Visual Studio Open Folder workflow")
		set_property(GLOBAL PROPERTY NS_CMAKE_INSIDE_VS ON)
	endif()

    if(CMAKE_GENERATOR MATCHES "Visual Studio")
        # Visual Studio (All VS generators define MSVC)
        message(STATUS "Generator is MSVC (NS_CMAKE_GENERATOR_MSVC)")

        set_property(GLOBAL PROPERTY NS_CMAKE_GENERATOR_MSVC ON)
        set_property(GLOBAL PROPERTY NS_CMAKE_GENERATOR_PREFIX "Vs")
        set_property(GLOBAL PROPERTY NS_CMAKE_GENERATOR_CONFIGURATION $<CONFIGURATION>)
    elseif(CMAKE_GENERATOR MATCHES "Ninja") # Ninja makefiles. Only makefile format supported by Visual Studio Open Folder
        message(STATUS "Buildsystem is Ninja (NS_CMAKE_GENERATOR_NINJA)")

        set_property(GLOBAL PROPERTY NS_CMAKE_GENERATOR_NINJA ON)
        set_property(GLOBAL PROPERTY NS_CMAKE_GENERATOR_PREFIX "Ninja")
        set_property(GLOBAL PROPERTY NS_CMAKE_GENERATOR_CONFIGURATION ${CMAKE_BUILD_TYPE})
    else()
        message(FATAL_ERROR "Generator '${CMAKE_GENERATOR}' is not supported on UWP! Please extend ns_platform_detect_generator()")
    endif()
endmacro()

macro (ns_platformhook_make_windowapp TARGET_NAME)
    set_property(TARGET ${TARGET_NAME} PROPERTY WIN32_EXECUTABLE ON)
endmacro()
