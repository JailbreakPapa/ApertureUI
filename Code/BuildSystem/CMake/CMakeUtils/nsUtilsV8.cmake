# #####################################
# ## ns_v8_init()
# #####################################
set(NS_V8_ROOT "${CMAKE_SOURCE_DIR}/${NS_CMAKE_RELPATH_CODE}/ThirdParty/v8" CACHE PATH "Path to the directory where v8 is located/wanted installed.")
set(NS_V8_BUILD_PATH "${CMAKE_SOURCE_DIR}/${NS_CMAKE_RELPATH_CODE}/ThirdParty/v8" CACHE PATH "Path to the directory where v8 will be cloned and built.")
set(NS_V8_READY_TO_BUILD ON CACHE BOOL "Are you ready to build v8? Make sure that you set up depot_tools. you should open the extracted directory and put: (gclient) to fully initialize everything.")
set(NS_V8_MANUAL_BUILD OFF CACHE BOOL "If you want to build the latest provided version of v8, then you can manually build v8. WARNING: May have Breaking API Changes.")
set(NS_V8_BINARY_PATH_NAME "windows_x86_64_release" CACHE STRING "Name of the binary path for v8. This is used to find the correct path to the v8 binaries.")
set(NS_V8_PREBUILTNAME "v8_prebuiltzip" CACHE STRING "Name of the prebuilt v8 directory.")
set(NS_V8_PREBUILT ON CACHE BOOL "If manually building ends up failing, you can use the latest uploaded version of v8 that works with apertureui.")
set(NS_V8_DLL_TYPE "x64.release")

function(ns_v8_export_prebuilt_dll TARGET_PROJECT)
    file(GLOB V8_DLL ${TARGET_PROJECT}/*.dll)
    add_custom_command(TARGET ${TARGET_PROJECT}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${V8_DLL} $<TARGET_FILE_DIR:${TARGET_PROJECT}>
        WORKING_DIRECTORY
    )
endfunction()

function(ns_v8_init)
    if(NS_V8_ROOT STREQUAL "")
        message(FATAL_ERROR "A Directory was not provided to install v8 inside.")
    endif()

    message(STATUS "Setting up v8. this will take some time to configure and build.")
    message(STATUS "Make sure that there is no prior build to v8 (meaning any prebuilt versions) before starting.")
    message(STATUS "Currently, we are planning to do our work inside: ${NS_V8_ROOT}")
    message("")

    if(NS_V8_MANUAL_BUILD)
        if(NS_CMAKE_PLATFORM_LINUX)
            # see if we can clone gn for building cmake.
            execute_process(COMMAND "git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git" WORKING_DIRECTORY "${NS_V8_ROOT}")
            execute_process(COMMAND "export PATH=${NS_V8_ROOT}/depot_tools:$PATH" WORKING_DIRECTORY "${NS_V8_ROOT}")
        endif()

        if(NS_CMAKE_PLATFORM_WINDOWS AND NS_CMAKE_COMPILER_MSVC)
            set(DOWNLOAD_LK "${NS_DEPOT_TOOLS_INSTALL_LINK_WIN}")
            ns_download_and_extract("https://gitlab.com/watchdogsllc/PUBLIC_THIRDPARTY/-/raw/main/depot_toolszip.7z" "${NS_V8_ROOT}" "depot_tools")
        endif()

        message(WARNING "Depot tools should have been installed. MAKE SURE that depot_tools is the first thing in your OS(s) PATH. ")

        # once we have successfully extracted depot_tools, we should run gclient to set everything up.
        message(STATUS "Set up depot_tools. you should open the extracted directory and put: (gclient) to fully initialize everything.")
        execute_process(COMMAND "gclient" WORKING_DIRECTORY "${NS_V8_ROOT}")

        if(NS_V8_READY_TO_BUILD)
            ns_v8_configurate()
            ns_v8_build()
        endif()
    elseif(NS_V8_PREBUILT)
        # Assume user wants latest prebuilt version
        if(NS_CMAKE_PLATFORM_WINDOWS AND NS_CMAKE_COMPILER_MSVC AND NS_V8_READY_TO_BUILD)
            set(DOWNLOAD_LK "https://github.com/WatchDogStudios/THIRDPARTY/raw/main/v8_prebuiltzip.7z")
            ns_download_and_extract("https://github.com/WatchDogStudios/THIRDPARTY/raw/main/v8_prebuiltzip.7z" "${NS_V8_ROOT}" "v8_prebuiltzip")
        endif()
    endif()
endfunction()

function(ns_v8_configurate)
    # From this point on, we assume gclient was called within depot_tools, and set to the FRONT of path.
    if((NS_V8_BUILD_PATH STREQUAL "NS_V8_BUILD_PATH-NOTFOUND") OR(NS_V8_BUILD_PATH STREQUAL ""))
        execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${NS_V8_ROOT}/v8_final)
        set(NS_V8_BUILD_PATH "${NS_V8_ROOT}/v8_final")
    endif()

    # Its now time to fetch v8. notify the user that this might take a while, depending on the connection.
    message(STATUS "We are now going to fetch v8. this might take a while, depending on your connection.")
    execute_process(COMMAND "fetch v8" WORKING_DIRECTORY "${NS_V8_BUILD_PATH}")

    # After cloning, we need to sync and retrieve the dependency's.
    message(STATUS "Syncing v8.")
    execute_process(COMMAND "git fetch" WORKING_DIRECTORY "${NS_V8_BUILD_PATH}")
    execute_process(COMMAND "gclient sync" WORKING_DIRECTORY "${NS_V8_BUILD_PATH}")
endfunction()

function(ns_v8_build)
    # Notify the user that we will start building v8. this will take about 2-3 hours.
    message(WARNING "Building V8. (DLL VERSION) this will take about 2- hours with Jenkins. NOTE: PYTHON SHOULD ALSO BE IN PATH!")

    if(NS_V8_MANUAL_BUILD)
        execute_process(COMMAND "python tools/dev/gm.py x64.debug" WORKING_DIRECTORY "${NS_V8_BUILD_PATH}/v8")
        execute_process(COMMAND "python tools/dev/gm.py x64.release" WORKING_DIRECTORY "${NS_V8_BUILD_PATH}/v8")
    endif()
endfunction(ns_v8_build)

function(ns_link_v8_target TARGET_PROJECT)
 # Define variables
    # Link target to libraries.
    message(STATUS "Finding V8 libraries at: ${NS_V8_ROOT}/v8_prebuiltzip/${NS_V8_BINARY_PATH_NAME}")
    ns_glob_library_files("${NS_V8_ROOT}/v8_prebuiltzip/${NS_V8_BINARY_PATH_NAME}" V8_NEEDED_LIBS)
    message(STATUS "Linking V8 libraries: ${V8_NEEDED_LIBS}")

    #target_link_libraries(${TARGET_PROJECT} PUBLIC ${V8_NEEDED_LIBS})
    target_link_libraries(${TARGET_PROJECT} PUBLIC "${NS_V8_ROOT}/v8_prebuiltzip/${NS_V8_BINARY_PATH_NAME}/v8.dll.lib")
    target_link_libraries(${TARGET_PROJECT} PUBLIC "${NS_V8_ROOT}/v8_prebuiltzip/${NS_V8_BINARY_PATH_NAME}/v8_libbase.dll.lib")
    target_link_libraries(${TARGET_PROJECT} PUBLIC "${NS_V8_ROOT}/v8_prebuiltzip/${NS_V8_BINARY_PATH_NAME}/v8_libplatform.dll.lib")
    # Link target to dynamic link libraries.
    ns_glob_dynamiclink_files(${NS_V8_ROOT}/v8_prebuiltzip/${NS_V8_BINARY_PATH_NAME} V8_DLLS)

    # Target's output directory for runtime artifacts (DLLs and executables)
    set(TARGET_OUTPUT_DIR $<TARGET_FILE_DIR:${TARGET_PROJECT}>)

    # Copy DLLs to the target output directory
    add_custom_command(TARGET ${TARGET_PROJECT} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${NS_V8_ROOT}/v8_prebuiltzip/${NS_V8_BINARY_PATH_NAME}/v8.dll" ${TARGET_OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${NS_V8_ROOT}/v8_prebuiltzip/${NS_V8_BINARY_PATH_NAME}/v8_libbase.dll" ${TARGET_OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${NS_V8_ROOT}/v8_prebuiltzip/${NS_V8_BINARY_PATH_NAME}/v8_libplatform.dll" ${TARGET_OUTPUT_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_FUNCTION_LIST_DIR}
    )

    # Create a resources directory inside the target's output directory
    set(RESOURCES_DIR ${TARGET_OUTPUT_DIR}/resources)
    add_custom_command(TARGET ${TARGET_PROJECT} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${RESOURCES_DIR}
    )

    # Copy V8 resources to the target's resources directory
    add_custom_command(TARGET ${TARGET_PROJECT} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
            ${NS_V8_ROOT}/${NS_V8_PREBUILTNAME}/resources/icudtl.dat 
            ${NS_V8_ROOT}/${NS_V8_PREBUILTNAME}/resources/snapshot_blob.bin 
            ${RESOURCES_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_FUNCTION_LIST_DIR}
    )

    # Include V8 headers in the target's include directories
    target_include_directories(${TARGET_PROJECT} PUBLIC ${NS_V8_ROOT}/${NS_V8_PREBUILTNAME}/include)
endfunction()
