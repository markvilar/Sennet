add_subdirectory(core)

# Add editor
if(PINE_BUILD_EDITOR)
    add_subdirectory(editor)
endif()
