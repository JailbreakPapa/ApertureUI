ns_cmake_init()

ns_requires(NS_APUI_ALLOW_JAVASCRIPT_PLUGIN)

set(PROJECT_NAME apertureuijsc)

ns_create_target(SHARED_LIBRARY ${PROJECT_NAME} NO_NS_PREFIX NO_UNITY)


target_link_libraries(${PROJECT_NAME}
  PUBLIC
  Foundation
  JavaScriptCorePP
)
#ns_link_v8_target(${PROJECT_NAME})
