function(set_compiler_cxx_flags interface_target)
    # Sets C++ flags based on the specific compiler

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # using Clang
        target_compile_features(${interface_target} INTERFACE cxx_std_20)
        target_compile_options(${interface_target} INTERFACE -std=c++20)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # using GCC
        target_compile_features(${interface_target} INTERFACE cxx_std_20)
        target_compile_options(${interface_target} INTERFACE -std=c++2a)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
        # using Intel C++
        target_compile_features(${interface_target} INTERFACE cxx_std_20)
        target_compile_options(${interface_target} INTERFACE -std=c++20)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        # using Visual Studio C++
        target_compile_features(${interface_target} INTERFACE cxx_std_20)
        target_compile_options(${interface_target} INTERFACE -std=c++20)
    endif()

endfunction()
