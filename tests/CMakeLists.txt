# Test window
add_executable(test_window test_window.cpp)
target_compile_features(test_window PRIVATE cxx_std_17)
target_compile_options(test_window PRIVATE -std=c++17)
target_compile_definitions(test_window PRIVATE)
target_link_libraries(test_window PRIVATE pine::pine)

set_target_properties(test_window PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
)
