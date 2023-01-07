# -----------------------------------------------------------------------------
# Test math
# -----------------------------------------------------------------------------

add_executable(test_math test_math.cpp)
target_compile_features(test_math PRIVATE cxx_std_17)
target_compile_options(test_math PRIVATE -std=c++17)
target_compile_definitions(test_math PRIVATE)
target_link_libraries(test_math PRIVATE pine::pine)

set_target_properties(test_math PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
)

add_test(NAME math_test COMMAND test_math)

# -----------------------------------------------------------------------------
# Test window
# -----------------------------------------------------------------------------

add_executable(test_window test_window.cpp)
target_compile_features(test_window PRIVATE cxx_std_17)
target_compile_options(test_window PRIVATE -std=c++17)
target_compile_definitions(test_window PRIVATE)
target_link_libraries(test_window PRIVATE pine::pine)

set_target_properties(test_window PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
)

# FIXME: Implement test for headless environments
# add_test(NAME window_test COMMAND test_window)
