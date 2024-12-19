option(APUI_MATRIX_ROW_MAJOR "Tells Harrlow to Use/Convert to row-major matrices. Column-major matrices are used by default." OFF)

if(NS_APUI_COMPILE_AS_LIB)
  list(APPEND CORE_PUBLIC_DEFS -DAPUI_STATIC_LIB)
  message("-- Building static libraries. Make sure to #define APUI_STATIC_LIB before including APUI in your project.")
endif()

ns_cmake_init()

# Get the name of this folder as the project name
set(PROJECT_NAME "apertureuihtmlengine")

ns_create_target(SHARED_LIBRARY ${PROJECT_NAME} NO_NS_PREFIX NO_UNITY)

configure_file(
  resource.h
  apertureuihtmlengine.rc
)

if(APUI_MATRIX_ROW_MAJOR)
  target_compile_definitions(${PROJECT_NAME} PUBLIC APUI_MATRIX_ROW_MAJOR)
endif()

ns_setup_dependancys_apui(${PROJECT_NAME})

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /-WX /GR)
endif()

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  apertureuicore
  pugixml
  yogacore
  stb_image
  uriparser
  harfbuzz
  freetype
  gumbo
  rlottie
  lunasvg
  Foundation
)
add_dependencies(${PROJECT_NAME}
  pugixml
  yogacore
  gumbo
  harfbuzz
  freetype
  stb_image
  uriparser
  rlottie
  lunasvg
)