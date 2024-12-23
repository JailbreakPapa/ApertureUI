ns_cmake_init()

set(PROJECT_NAME apertureuiv8)

ns_create_target(SHARED_LIBRARY ${PROJECT_NAME} NO_NS_PREFIX NO_UNITY)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  Foundation
  apertureuicore
)

ns_link_v8_target(${PROJECT_NAME})
