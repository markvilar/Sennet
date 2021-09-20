set(SENNET_EDITOR_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Editor/Include")
set(SENNET_EDITOR_SOURCE_DIR "${PROJECT_SOURCE_DIR}/Editor/Source")
set(SENNET_EDITOR_VENDOR_DIR "${PROJECT_SOURCE_DIR}/Editor/Vendor")

set(SENNET_EDITOR_HDR "${SENNET_EDITOR_INCLUDE_DIR}/Editor/EditorLayer.hpp")
set(SENNET_EDITOR_SRC "${SENNET_EDITOR_SOURCE_DIR}/Editor/EditorLayer.cpp")

add_library(Sennet-Editor STATIC ${SENNET_EDITOR_HDR} ${SENNET_EDITOR_SRC})

target_include_directories(Sennet-Editor PUBLIC "${SENNET_EDITOR_INCLUDE_DIR}"
)# glm, imgui, spdlog

add_subdirectory(Application)

target_link_libraries(Sennet-Editor PRIVATE Sennet)
target_compile_definitions(Sennet-Editor PRIVATE)
target_compile_features(Sennet-Editor PUBLIC cxx_std_20)
