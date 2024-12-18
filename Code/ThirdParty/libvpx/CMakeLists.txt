ns_cmake_init()

# Get the name of this folder as the project name
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)

ns_create_target(STATIC_LIBRARY ${PROJECT_NAME} NO_NS_PREFIX NO_WARNINGS_AS_ERRORS)

if(NS_CMAKE_PLATFORM_ANDROID)
    target_compile_definitions(${PROJECT_NAME} PUBLIC VPX_PLATFORM_ANDROID)
else()
    list(REMOVE_ITEM ALL_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/third_party/android/cpu-features.h","${CMAKE_CURRENT_SOURCE_DIR}/third_party/android/cpu-features.c")
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)