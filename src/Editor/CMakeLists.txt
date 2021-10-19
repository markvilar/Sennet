add_library(sennet-editor STATIC)

target_sources(
    sennet-editor
    PUBLIC include/Editor/EditorLayer.hpp
    PRIVATE src/Editor/EditorLayer.cpp)

target_include_directories(sennet-editor PUBLIC include)

add_subdirectory(app)

target_link_libraries(sennet-editor PUBLIC sennet)
target_compile_definitions(sennet-editor PUBLIC)
target_compile_features(sennet-editor PUBLIC cxx_std_20)
