# #####################################
# ## ns_vcpkg_show_info()
# #####################################

function(ns_vcpkg_show_info)
	message("This project can use vcpkg to find its 3rd party dependencies.")
	message("You can install vcpkg either manually or using InstallVcpkg.bat.")
	message("Make sure to set the environment variable 'VCPKG_ROOT' to point to your installation of vcpkg.")
	message("")
endfunction()


# #####################################
# ## ns_vcpkg_init()
# #####################################
function(ns_vcpkg_init)
	message(STATUS "EnvVar %VCPKG_ROOT% is set to '$ENV{VCPKG_ROOT}'")

	if(CMAKE_TOOLCHAIN_FILE)
		message(STATUS "CMAKE_TOOLCHAIN_FILE is already set to '${CMAKE_TOOLCHAIN_FILE}' - not going to modify it.")
		get_filename_component(NS_VCPKG_ROOT "${CMAKE_TOOLCHAIN_FILE}" DIRECTORY)
		get_filename_component(NS_VCPKG_ROOT "${NS_VCPKG_ROOT}" DIRECTORY)
		get_filename_component(NS_VCPKG_ROOT "${NS_VCPKG_ROOT}" DIRECTORY)

	else()
		if(DEFINED ENV{VCPKG_ROOT})
			set(NS_VCPKG_ROOT "$ENV{VCPKG_ROOT}")
			message(STATUS "EnvVar VCPKG_ROOT is specified, using that.")
		else()
			set(NS_VCPKG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg")
			message(STATUS "EnvVar VCPKG_ROOT is not specified, using '${CMAKE_CURRENT_SOURCE_DIR}/vcpkg'.")
		endif()

		if(NOT EXISTS "${NS_VCPKG_ROOT}/vcpkg.exe" OR NOT EXISTS "${NS_VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
			message("vcpkg is not installed. Either install it manually and set the environment variable VCPKG_ROOT to its directory, or run InstallVcpkg.bat")
			return()
		endif()

		set(CMAKE_TOOLCHAIN_FILE "${NS_VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "" FORCE)
		message(STATUS "Forcing CMAKE_TOOLCHAIN_FILE to point to '${CMAKE_TOOLCHAIN_FILE}'")
	endif()

	message(STATUS "NS_VCPKG_ROOT is '${NS_VCPKG_ROOT}'")
	set_property(GLOBAL PROPERTY "NS_VCPKG_ROOT" ${NS_VCPKG_ROOT})
endfunction()




# #####################################
# ## ns_vcpkg_install(<packages>)
# #####################################
function(ns_vcpkg_install PACKAGES)
	ns_cmake_init()

	get_property(NS_VCPKG_ROOT GLOBAL PROPERTY "NS_VCPKG_ROOT")

	if(NS_CMAKE_PLATFORM_WINDOWS)
		if(NS_CMAKE_ARCHITECTURE_64BIT)
			set(VCPKG_TARGET_TRIPLET "x64-windows")
		else()
			set(VCPKG_TARGET_TRIPLET "x86-windows")
		endif()
	elseif(NS_CMAKE_PLATFORM_LINUX)
		if(NS_CMAKE_ARCHITECTURE_64BIT)
			set(VCPKG_TARGET_TRIPLET "x64-linux")
		else()
			set(VCPKG_TARGET_TRIPLET "x86-linux")
		endif()
	else()
		message(FATAL_ERROR "vcpkg target triplet is not configured for this platform")
	endif()

	foreach(PACKAGE ${PACKAGES})
		message("VCPKG: Installing '${PACKAGE}', this may take a while.")
		execute_process(COMMAND "${NS_VCPKG_ROOT}/vcpkg.exe" install "${PACKAGE}:${VCPKG_TARGET_TRIPLET}" WORKING_DIRECTORY "${NS_VCPKG_ROOT}")
	endforeach()
endfunction()

# Function to integrate vcpkg manually for a specific library and its dependencies
function(ns_integrate_vcpkg target_name library_name)
if(NOT DEFINED ENV{VCPKG_ROOT})
message(FATAL_ERROR "VCPKG_ROOT environment variable is not defined.")
endif()
set(VCPKG_ROOT $ENV{VCPKG_ROOT})
set(VCPKG_TRIPLET "x64-windows")
set(VCPKG_LIB_PATH "${VCPKG_ROOT}/installed/${VCPKG_TRIPLET}/lib")
set(VCPKG_BIN_PATH "${VCPKG_ROOT}/installed/${VCPKG_TRIPLET}/bin")
set(VCPKG_INCLUDE_PATH "${VCPKG_ROOT}/installed/${VCPKG_TRIPLET}/include")

if(NOT EXISTS ${VCPKG_LIB_PATH})
message(FATAL_ERROR "Library path not found: ${VCPKG_LIB_PATH}")
endif()
if(NOT EXISTS ${VCPKG_BIN_PATH})
message(FATAL_ERROR "Binary path not found: ${VCPKG_BIN_PATH}")
endif()
if(NOT EXISTS ${VCPKG_INCLUDE_PATH})
message(FATAL_ERROR "Include path not found: ${VCPKG_INCLUDE_PATH}")
endif()

function(parse_dependencies library_name dependencies)
set(PORT_PATH "${VCPKG_ROOT}/ports/${library_name}/vcpkg.json")
if(NOT EXISTS ${PORT_PATH})
	message(FATAL_ERROR "vcpkg.json not found for library: ${library_name}")
endif()

file(READ ${PORT_PATH} JSON_CONTENT)
string(JSON DEP_LIST GET ${JSON_CONTENT} dependencies)
if(DEP_LIST MATCHES "^\[.*\]$")
	string(JSON DEP_COUNT LENGTH ${DEP_LIST})
	foreach(DEP_INDEX RANGE 0 ${DEP_COUNT} EQUAL OFF BY 1)
		string(JSON DEP_NAME GET ${DEP_LIST} ${DEP_INDEX} name)
		list(APPEND ${dependencies} ${DEP_NAME})
	endforeach()
else()
	message(WARNING "No dependencies found in ${PORT_PATH}.")
endif()
endfunction()

set(ALL_DEPENDENCIES ${library_name})
list(APPEND PROCESSED_LIBRARIES)

function(resolve_all_dependencies library_name)
if(NOT library_name IN_LIST PROCESSED_LIBRARIES)
	list(APPEND PROCESSED_LIBRARIES ${library_name})
	set(NEW_DEPENDENCIES)
	parse_dependencies(${library_name} NEW_DEPENDENCIES)
	foreach(DEP IN LISTS NEW_DEPENDENCIES)
		list(APPEND ALL_DEPENDENCIES ${DEP})
		resolve_all_dependencies(${DEP})
	endforeach()
endif()
endfunction()

resolve_all_dependencies(${library_name})

set(VCPKG_LIBS)
set(VCPKG_DLLS)

foreach(DEP IN LISTS ALL_DEPENDENCIES)
file(GLOB_RECURSE DEP_LIBS "${VCPKG_LIB_PATH}/${DEP}*.lib")
file(GLOB_RECURSE DEP_DLLS "${VCPKG_BIN_PATH}/${DEP}*.dll")
list(APPEND VCPKG_LIBS ${DEP_LIBS})
list(APPEND VCPKG_DLLS ${DEP_DLLS})
endforeach()

include_directories(${VCPKG_INCLUDE_PATH})
if(VCPKG_LIBS)
target_link_libraries(${target_name} PRIVATE ${VCPKG_LIBS})
else()
message(FATAL_ERROR "No libraries found for ${library_name} or its dependencies in ${VCPKG_LIB_PATH}")
endif()

set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
foreach(DLL ${VCPKG_DLLS})
add_custom_command(
	TARGET ${target_name} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${DLL}" "${CMAKE_BINARY_DIR}/bin"
)
endforeach()

set_property(GLOBAL PROPERTY EXPORT_COMPILE_COMMANDS ON)
message(STATUS "VCPKG_LIBS for ${library_name} and dependencies: ${VCPKG_LIBS}")
message(STATUS "VCPKG_DLLS for ${library_name} and dependencies: ${VCPKG_DLLS}")
endfunction()