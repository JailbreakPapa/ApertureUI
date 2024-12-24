ns_cmake_init()
ns_vcpkg_init()

ns_requires(NS_APUI_ALLOW_TEXTTOSPEECH_PLUGIN)

set(PROJECT_NAME apertureuitexttospeech)

ns_create_target(SHARED_LIBRARY ${PROJECT_NAME} NO_NS_PREFIX)

ns_vcpkg_install(onnx)
ns_vcpkg_install(spdlog)
ns_vcpkg_install(fmt)

ns_integrate_vcpkg(${PROJECT_NAME} onnx)
ns_integrate_vcpkg(${PROJECT_NAME} spdlog) 
ns_integrate_vcpkg(${PROJECT_NAME} fmt)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  apertureuicore
  apertureuinetwork
)

