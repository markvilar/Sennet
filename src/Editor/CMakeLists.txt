add_library(editor STATIC)
add_library(sennet::editor ALIAS editor)

target_sources(
    editor
    PUBLIC include/Editor/EditorLayer.hpp
    PRIVATE src/Editor/EditorLayer.cpp)

target_include_directories(editor PUBLIC include)

add_subdirectory(app)

target_link_libraries(editor PUBLIC sennet::sennet)
target_compile_definitions(editor PUBLIC)
target_compile_features(editor PUBLIC cxx_std_20)
target_compile_options(editor PRIVATE -std=c++2a)

set_target_properties(
    editor
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
               LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
               RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
