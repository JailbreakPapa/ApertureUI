ns_cmake_init()

ns_requires_desktop()

# Get the name of this folder as the project name
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)

ns_create_target(APPLICATION ${PROJECT_NAME} NO_NS_PREFIX)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  Foundation
)
