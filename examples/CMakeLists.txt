add_executable(sandbox Sandbox.cpp)
target_compile_features(sandbox PRIVATE cxx_std_17)
target_compile_options(sandbox PRIVATE -std=c++17)
target_compile_definitions(sandbox PRIVATE)
target_link_libraries(sandbox PRIVATE pine::pine)

set_target_properties(sandbox PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                         "${CMAKE_BINARY_DIR}/bin")

add_executable(server Server.cpp)
target_compile_features(server PRIVATE cxx_std_17)
target_compile_options(server PRIVATE -std=c++17)
target_compile_definitions(server PRIVATE)
target_link_libraries(server PRIVATE pine::pine)

set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                         "${CMAKE_BINARY_DIR}/bin")
