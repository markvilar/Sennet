# Editor
add_executable(editor-app Editor.cpp)
target_compile_definitions(editor-app PRIVATE)
target_compile_features(editor-app PRIVATE cxx_std_17)
target_link_libraries(editor-app PRIVATE sennet::editor)

set_target_properties(
    editor-app
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
               LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
               RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
