ns_cmake_init()

set(PROJECT_NAME apertureuicore)

ns_create_target(SHARED_LIBRARY ${PROJECT_NAME} NO_NS_PREFIX)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  Foundation
  mimalloc-static
)

