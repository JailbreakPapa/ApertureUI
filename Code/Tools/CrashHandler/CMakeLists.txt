ns_cmake_init()

ns_requires_qt()

ns_requires(NS_3RDPARTY_ADS_SUPPORT)

set(PROJECT_NAME crashhandler_dev)

ns_create_target(APPLICATION ${PROJECT_NAME})

ns_link_target_qt(TARGET ${PROJECT_NAME} COMPONENTS Core Gui Widgets)

ns_make_windowapp(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  GuiFoundation
  ads
)

