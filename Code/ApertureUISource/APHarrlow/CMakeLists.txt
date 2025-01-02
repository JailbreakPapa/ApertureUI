ns_cmake_init()

# Get the name of this folder as the project name
# HARRLOW: Engine that breaks everything into Graphics API Commands. 
set(PROJECT_NAME "apertureuiharrlowengine")

ns_create_target(LIBRARY ${PROJECT_NAME} NO_NS_PREFIX)

if(NS_CMAKE_PLATFORM_WINDOWS)
  target_link_libraries(${PROJECT_NAME} PRIVATE shlwapi imm32)
  target_compile_definitions(${PROJECT_NAME} PRIVATE VK_USE_PLATFORM_WIN32_KHR)
elseif(NS_CMAKE_PLATFORM_LINUX)
  target_compile_options(${PROJECT_NAME} PRIVATE "-Wno-nullability-completeness" "-Wno-enum-compare" "-Wno-switch")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}
)

ns_setup_dependancys_apui(${PROJECT_NAME})

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX- /GR)
endif()

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  zlib
  freetype
  harfbuzz
  lunasvg
  msdfgen
  Foundation
)

configure_file(
  resource.h
  apertureuiharrlowengine.rc
)




