if(NS_APUI_COMPILE_AS_LIB)
  list(APPEND CORE_PUBLIC_DEFS -DAPUI_STATIC_LIB)
  message("-- Building static libraries. Make sure to #define APUI_STATIC_LIB before including APUI in your project.")
endif()

option(APUI_MATRIX_ROW_MAJOR "Tells Harrlow to Use/Convert to row-major matrices. Column-major matrices are used by default." OFF)

ns_cmake_init()

ns_vcpkg_init()

ns_vcpkg_install(icu)

# Get the name of this folder as the project name
set(PROJECT_NAME "apertureuihtmlengine")

ns_create_target(SHARED_LIBRARY ${PROJECT_NAME} NO_NS_PREFIX NO_UNITY NO_WARNINGS_AS_ERRORS)

set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

ns_integrate_vcpkg(${PROJECT_NAME} icu) 

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Code/ThirdParty/Boost.Parser/include)
target_compile_definitions(${PROJECT_NAME} PRIVATE BOOST_PARSER_DISABLE_HANA_TUPLE)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  Foundation
  mimalloc-static
  apertureuiharrlowengine
  pugixml
  yogacore
  stb_image
  uriparser
  harfbuzz
  freetype
  rlottie
  lunasvg
  libvpx
  libtheora
  libsimplewebm
  vorbis
  
)
add_dependencies(${PROJECT_NAME}
  pugixml
  yogacore
  harfbuzz
  freetype
  stb_image
  uriparser
  rlottie
  lunasvg
  libvpx
  libtheora
  libsimplewebm
  vorbis
)

ns_link_v8_target(${PROJECT_NAME})