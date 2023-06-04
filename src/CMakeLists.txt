add_subdirectory(pine)

# Add editor
if(PINE_EDITOR_ENABLED)
    add_subdirectory(editor)
endif()
