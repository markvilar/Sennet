# -----------------------------------------------------------------------------
# Sandbox
# -----------------------------------------------------------------------------

add_executable(sandbox sandbox.cpp)
target_compile_definitions(sandbox PRIVATE)
target_link_libraries(sandbox PRIVATE pine::pine)

set_target_properties(sandbox PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
)

# -----------------------------------------------------------------------------
# serial example
# -----------------------------------------------------------------------------

add_executable(serial_example serial.cpp)
target_compile_definitions(serial_example PRIVATE)
target_link_libraries(serial_example PRIVATE pine::pine)

set_target_properties(serial_example PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
)

# -----------------------------------------------------------------------------
# Server
# -----------------------------------------------------------------------------

add_executable(server server.cpp)
target_compile_definitions(server PRIVATE)
target_link_libraries(server PRIVATE pine::pine)

set_target_properties(server PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
)
