set(NS_HERMES_ROOT "${CMAKE_SOURCE_DIR}/${NS_CMAKE_RELPATH_CODE}/ThirdParty/hermes" CACHE PATH "Path to the directory where hermes is located/wanted installed.")
set(NS_HERMES_PREBUILT ON CACHE BOOL "If manually building ends up failing, you can use the latest uploaded version of Hermes. that works with apertureui.")
set(NS_JSC_ROOT "${CMAKE_SOURCE_DIR}/${NS_CMAKE_RELPATH_CODE}/ThirdParty/JavaScriptCore" CACHE PATH "Path to the directory where JavaScriptCore is located/wanted installed.")
set(NS_JSC_PREBUILT ON CACHE BOOL "If manually building ends up failing, you can use the latest uploaded version of JavaScriptCore that works with apertureui.")
set(NS_HERMES_ALLOW_GITHUB_PREBUILT OFF CACHE BOOL "Allow CMake to grab the Hermes prebuilt from github? you have to have access to our dev portal for this.")
set(NS_JSC_ALLOW_GITHUB_PREBUILT OFF CACHE BOOL "Allow CMake to grab the JavaScriptCore prebuilt from github? you have to have access to our dev portal for this.")
set(NS_JSC_PLATFORM_BUILD "Prospero" CACHE STRING "Platform to build/use for the JavaScriptCore Engine.")

function(ns_hermes_downloadprebuilt)
    # REALLY RECOMMENDED TO USE JSC.
    if(NS_HERMES_ALLOW_GITHUB_PREBUILT)
        # Assume user wants latest prebuilt version
        set(DOWNLOAD_LK "https://github.com/WatchDogStudios/WD_THIRDPARTY_INTERNAL/blob/main/hermes_prospero_build.7z")
        set(NS_HERMES_ROOT "${CMAKE_SOURCE_DIR}/${NS_CMAKE_RELPATH_CODE}/ThirdParty/hermes/hermes_prospero_build")
        ns_download_and_extract("https://github.com/WatchDogStudios/WD_THIRDPARTY_INTERNAL/blob/main/hermes_prospero_build.7z" "${NS_HERMES_ROOT}" "hermes_prospero_build")
    else()
        message(NOTICE "NS_HERMES_ALLOW_GITHUB_PREBUILT is set to Off, so we wont fetch data from there. it is recommended that you manually extract it to the hermes directory, since we cant have NDA packages open-source like this. buildbots (provided to authorized developers) should be able to still grab access with a key.")
    endif()
endfunction()

function(ns_jsc_download_prebuilt)
    if(NS_JSC_ALLOW_GITHUB_PREBUILT)
        # Assume user wants latest prebuilt version
        set(DOWNLOAD_LK "https://github.com/WatchDogStudios/WD_THIRDPARTY_INTERNAL/blob/main/JavaScriptCore.7z")
        set(NS_HERMES_ROOT "${CMAKE_SOURCE_DIR}/${NS_CMAKE_RELPATH_CODE}/ThirdParty/JavaScriptCore/JavaScriptCore")
        ns_download_and_extract("https://github.com/WatchDogStudios/WD_THIRDPARTY_INTERNAL/blob/main/JavaScriptCore.7z" "${NS_JSC_ROOT}" "JavaScriptCore")
    else()
        message(NOTICE "NS_JSC_ALLOW_GITHUB_PREBUILT is set to Off, so we wont fetch data from there. it is recommended that you manually extract it to the JavaScriptCore directory, since we cant have NDA packages open-source like this. private buildbots (provided to authorized developers) should be able to still grab access with a key.")
    endif()
endfunction()

function(ns_hermes_export_prebuilt_dll TARGET_PROJECT)
    # Check what platform we are exporting to, we are assuming PS5 here.
    # Actually, no need. all platforms should have heremes staticly built.
    if(NOT NS_CMAKE_PLATFORM_PLAYSTATION_5)
        file(GLOB HERMES_DLL ${TARGET_PROJECT}/*.a)
        add_custom_command(TARGET ${TARGET_PROJECT}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${HERMES_DLL} $<TARGET_FILE_DIR:${TARGET_PROJECT}>
            WORKING_DIRECTORY
        )
    endif()
endfunction()

function(ns_link_hermes_target TARGET_PROJECT)
    # Define variables
    ns_hermes_downloadprebuilt()

    # Link target to libraries.
    ns_glob_library_files(${NS_HERMES_ROOT}/lib/Release HERMES_NEEDED_LIBS)

    target_link_libraries(${TARGET_PROJECT} PUBLIC ${HERMES_NEEDED_LIBS})

    # Link target to dynamic link libraries.
    ns_glob_dynamiclink_files(${NS_HERMES_ROOT}/lib/Release HERMES_DLLS)

    # Make DLLs export to target output directory.
    add_custom_command(TARGET ${TARGET_PROJECT} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${HERMES_DLLS} $<TARGET_FILE_DIR:${TARGET_PROJECT}>
        WORKING_DIRECTORY ${CMAKE_CURRENT_FUNCTION_LIST_DIR}
    )

    # Make target include HERMES headers.
    target_include_directories(${TARGET_PROJECT} PUBLIC ${NS_HERMES_ROOT}/include)

    # Export needed resources for NS_HERMES to work.
    add_custom_command(TARGET ${TARGET_PROJECT} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${NS_HERMES_ROOT}/resources/${NS_HERMES_DLL_TYPE}/icudtl.dat ${CMAKE_CURRENT_LIST_DIR}/resources
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${NS_HERMES_ROOT}/resources/${NS_HERMES_DLL_TYPE}/natives_blob.bin ${CMAKE_CURRENT_LIST_DIR}/resources
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${NS_HERMES_ROOT}/resources/${NS_HERMES_DLL_TYPE}/snapshot_blob.bin ${CMAKE_CURRENT_LIST_DIR}/resources
        WORKING_DIRECTORY ${CMAKE_CURRENT_FUNCTION_LIST_DIR}
    )
endfunction()