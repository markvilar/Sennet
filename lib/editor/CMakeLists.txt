# -----------------------------------------------------------------------------
# editor library
# -----------------------------------------------------------------------------

add_library(editor STATIC)
add_library(pine::editor ALIAS editor)

target_sources(editor
    PRIVATE 
        src/editor/editor.cpp
        src/editor/editor.hpp
)

target_include_directories(editor PUBLIC src)

target_link_libraries(editor PUBLIC pine::pine)
target_compile_definitions(editor PUBLIC)

set_target_properties(editor PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# -----------------------------------------------------------------------------
# editor application
# -----------------------------------------------------------------------------

add_executable(editor_app bin/editor.cpp)
target_compile_definitions(editor_app PRIVATE)

target_link_libraries(editor_app PRIVATE pine::editor)

set_target_properties(editor_app PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_custom_command(TARGET editor_app 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/resources"
    "$<TARGET_FILE_DIR:editor_app>/resources"
)
