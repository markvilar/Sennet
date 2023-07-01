# -----------------------------------------------------------------------------
# Sandbox
# -----------------------------------------------------------------------------

add_executable(sandbox sandbox.cpp)
target_compile_features(sandbox PRIVATE cxx_std_20)
target_compile_options(sandbox PRIVATE -std=c++20)
target_compile_definitions(sandbox PRIVATE)
target_link_libraries(sandbox PRIVATE pine::pine)

set_target_properties(sandbox PROPERTIES 
    CXX_STANDARD 20
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
)

# -----------------------------------------------------------------------------
# serial example
# -----------------------------------------------------------------------------

add_executable(serial_example serial.cpp)
target_compile_features(serial_example PRIVATE cxx_std_20)
target_compile_options(serial_example PRIVATE -std=c++20)
target_compile_definitions(serial_example PRIVATE)
target_link_libraries(serial_example PRIVATE pine::pine)

set_target_properties(serial_example PROPERTIES 
    CXX_STANDARD 20
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
)

# -----------------------------------------------------------------------------
# Server
# -----------------------------------------------------------------------------

add_executable(server server.cpp)
target_compile_features(server PRIVATE cxx_std_20)
target_compile_options(server PRIVATE -std=c++20)
target_compile_definitions(server PRIVATE)
target_link_libraries(server PRIVATE pine::pine)

set_target_properties(server PROPERTIES 
    CXX_STANDARD 20
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
)
