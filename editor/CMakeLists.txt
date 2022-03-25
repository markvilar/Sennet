# -----------------------------------------------------------------------------
# Editor library
# -----------------------------------------------------------------------------

add_library(editor STATIC)
add_library(pine::editor ALIAS editor)

target_sources(editor
    PUBLIC 
        include/Editor/EditorLayer.hpp
    PRIVATE 
        src/EditorLayer.cpp)

target_include_directories(editor PUBLIC include)

target_link_libraries(editor PUBLIC pine::pine)
target_compile_definitions(editor PUBLIC)
target_compile_features(editor PUBLIC cxx_std_17)
target_compile_options(editor PRIVATE -std=c++17)

set_target_properties(editor PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

# -----------------------------------------------------------------------------
# Editor application
# -----------------------------------------------------------------------------

add_executable(editor-app Editor.cpp)
target_compile_definitions(editor-app PRIVATE)
target_compile_features(editor-app PRIVATE cxx_std_17)
target_link_libraries(editor-app PRIVATE pine::editor)

set_target_properties(editor-app PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_custom_command(TARGET editor-app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/resources"
    "$<TARGET_FILE_DIR:editor-app>/resources")
