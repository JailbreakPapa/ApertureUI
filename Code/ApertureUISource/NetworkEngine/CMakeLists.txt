ns_cmake_init()

ns_requires(NS_APUI_ALLOW_NETWORKING_PLUGIN)

set(PROJECT_NAME apertureuinetwork)

ns_create_target(SHARED_LIBRARY ${PROJECT_NAME} NO_NS_PREFIX)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  Foundation
  apertureuicore
  websockets
)
