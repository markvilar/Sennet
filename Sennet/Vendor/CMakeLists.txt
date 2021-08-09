# -----------------------------------------------------------------------------
# ---- glad -------------------------------------------------------------------
# -----------------------------------------------------------------------------

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/glad/CMakeLists.txt")
    	message(FATAL_ERROR "Sennet submodule glad was not downloaded! "
		"GIT_SUBMODULE was turned off or failed. Please update submodules and "
		"try again.")
endif()

add_subdirectory(glad)

# -----------------------------------------------------------------------------
# ---- glfw -------------------------------------------------------------------
# -----------------------------------------------------------------------------

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/glfw/CMakeLists.txt")
    	message(FATAL_ERROR "Sennet submodule glfw was not downloaded! "
		"GIT_SUBMODULE was turned off or failed. Please update submodules and "
		"try again.")
endif()

add_subdirectory(glfw)

# -----------------------------------------------------------------------------
# ---- imgui ------------------------------------------------------------------
# -----------------------------------------------------------------------------

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/imgui/CMakeLists.txt")
    	message(FATAL_ERROR "Sennet submodule imgui was not downloaded! "
		"GIT_SUBMODULE was turned off or failed. Please update submodules and "
		"try again.")
endif()

add_subdirectory(imgui)
