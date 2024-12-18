ns_cmake_init()

# Get the name of this folder as the project name
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)

ns_create_target(STATIC_LIBRARY ${PROJECT_NAME} NO_NS_PREFIX NO_WARNINGS_AS_ERRORS)

set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(${PROJECT_NAME} PUBLIC ogg)
