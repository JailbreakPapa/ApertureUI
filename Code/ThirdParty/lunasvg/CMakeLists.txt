ns_cmake_init()

set (PROJECT_NAME "lunasvg")

ns_create_target(STATIC_LIBRARY ${PROJECT_NAME} NO_UNITY NO_NS_PREFIX NO_WARNINGS_AS_ERRORS)

# override the default IDE folder
ns_set_project_ide_folder(${PROJECT_NAME} "${NS_ROOT}/Code/ThirdParty/lunasvg")

target_include_directories(${PROJECT_NAME}
  PRIVATE
  "3rdparty/plutovg"
  "3rdparty/software"

  PUBLIC
  "include"
)

if(NS_CMAKE_COMPILER_MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /wd4456 /wd4458 /wd4701)
endif()