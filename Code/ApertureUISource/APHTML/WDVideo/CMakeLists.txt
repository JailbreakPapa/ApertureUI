ns_cmake_init()

set(PROJECT_NAME "wdvideo_core")

ns_create_target(SHARED_LIBRARY ${PROJECT_NAME} NO_NS_PREFIX NO_WARNINGS_AS_ERRORS)

set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  Foundation
 
)


