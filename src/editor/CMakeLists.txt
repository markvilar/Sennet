# -----------------------------------------------------------------------------
# editor library
# -----------------------------------------------------------------------------

add_library(editor STATIC)
add_library(pine::editor ALIAS editor)

target_sources(editor
    PUBLIC 
        include/editor/editor_layer.hpp
    PRIVATE 
        src/editor_layer.cpp
)

target_include_directories(editor PUBLIC include)

target_link_libraries(editor PUBLIC pine::pine)
target_compile_definitions(editor PUBLIC)
target_compile_features(editor PUBLIC cxx_std_17)
target_compile_options(editor PRIVATE -std=c++17)

set_target_properties(editor PROPERTIES 
    CXX_STANDARD 17
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# -----------------------------------------------------------------------------
# editor application
# -----------------------------------------------------------------------------

add_executable(editor_app editor.cpp)
target_compile_definitions(editor_app PRIVATE)
target_compile_features(editor_app PRIVATE cxx_std_17)
target_link_libraries(editor_app PRIVATE pine::editor)

set_target_properties(editor_app PROPERTIES 
    CXX_STANDARD 17
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
