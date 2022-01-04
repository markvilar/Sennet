# Pine

[![Build](https://github.com/markvilar/pine/actions/workflows/build.yml/badge.svg)](https://github.com/markvilar/pine/actions/workflows/build.yml)
[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

## Description
Pine is a lightweight, static, cross-platform library for creating C++ 
applications with graphics and networking functionality. Pine has been developed
with focus on data visualization, lean dependencies, and sensor networking.

## Requirements
The requirements are:
- C++17 compiler
- CMake 3.16 (or higher)

## Dependencies

| **Library** | **Version**  | **Library purpose.**                |
|-------------|--------------|-------------------------------------|
| asio        | 1.18.0       | Network and asynchronicity.         |
| glad        | 0.1.34       | Bindings and loaders of OpenGL 4.6. |
| glfw        | 3.3.4        | Window and input handling.          |
| glm         | 0.9.9.8      | Vectorized mathematical operations. |
| imgui       | 1.85         | Graphical user interface.           |
| spdlog      | 1.9.2        | Console and file logging.           |

## Workflow
By default, Pine utilizes CMake for build generation and Conan for package
management. However, all the native features of CMake are preserved by Pine.
This means that the user can utilize another package manager by simply 
providing ```FindXXX.cmake```-files for each of the dependencies in the 
```CMAKE_MODULE_PATH``` variable.

### Linux Workflow

```
git clone https://github.com/markvilar/pine.git
cd pine

export CMAKE_GENERATOR="${GENERATOR}"
export CMAKE_BUILD_TYPE="${BUILD_TYPE}"
export CMAKE_CXX_COMPILER="${CXX_COMPILER}"

conan install . \
    --install-folder "${PACKAGE_DIR}" \
    --settings build_type="${BUILD_TYPE}" \
    --profile="${CONAN_PROFILE}" \
    --build missing

cmake -S . -B "${BUILD_DIR}" -D CMAKE_MODULE_PATH="${PACKAGE_DIR}"
cmake --build "${BUILD_DIR}"
```

## Development Plan

[Trello board](https://trello.com/b/iZZPB2t0/pine)

## Acknowledgements
I would like to acknowledge the following projects for inspiring this one:
- [Hazel](https://github.com/TheCherno/Hazel)
- [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine)
