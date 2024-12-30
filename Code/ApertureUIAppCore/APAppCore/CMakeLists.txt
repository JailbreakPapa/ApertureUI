ns_cmake_init()
ns_requires(NS_APUI_ALLOW_APUI_APP_CORE)

# Get the name of this folder as the project name
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)

ns_requires(NS_APUI_ALLOW_APUI_APP_CORE)

ns_create_target(LIBRARY ${PROJECT_NAME} NO_NS_PREFIX)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  apertureuicore
)