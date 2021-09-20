if(SENNET_USE_VENDOR_IMGUI)
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/imgui/CMakeLists.txt")
        message(
            FATAL_ERROR
                "Submodule imgui was not downloaded! "
                "GIT_SUBMODULE was turned off or failed. Please update "
                "submodules and try again.")
    endif()
    add_subdirectory(imgui)
endif()
