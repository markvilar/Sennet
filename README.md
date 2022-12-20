# Pine

[![Build](https://github.com/markvilar/pine/actions/workflows/build.yml/badge.svg)](https://github.com/markvilar/pine/actions/workflows/build.yml)
[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

## Description
Pine is a lightweight, cross-platform library for creating C++ applications 
with graphics and networking functionality. Pine has been developed with focus 
on maintaining a lean dependency structure, and leveraging modern C++
development tools.

## Requirements
Pine requires the follow tools to be built:
- C++17
- CMake 3.16+
- Conan 1.39+ (optional)

## Dependencies

| **Library** | **Version**  | **Library purpose.**                |
|-------------|--------------|-------------------------------------|
| asio        | 1.21.0       | Networking and asynchronicity.      |
| glad        | 0.1.34       | Bindings and loaders of OpenGL 4.6. |
| glfw        | 3.3.4        | Window and input handling.          |
| glm         | 0.9.9.8      | Vectorized mathematical operations. |
| imgui       | 1.85         | Graphical user interface.           |
| spdlog      | 1.9.2        | Console and file logging.           |

## Workflows
By default, Pine utilizes CMake for build generation and Conan for package
management. However, all the native features of CMake are preserved by Pine.
This means that the user can utilize another package manager by simply 
providing paths to ```FindXXX.cmake``` files for each of the dependencies in 
the ```CMAKE_MODULE_PATH``` variable.

### Linux Workflow

#### Building

```shell
# Clone pine
git clone https://github.com/markvilar/pine.git
cd pine

# Allow Conan to install OS requirements
export CONAN_SYSREQUIRES_MODE=enabled

# Generate build with CMake
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build
```

#### Installing
```shell
# Install Pine with CMake
cmake --install build --prefix install --config Release
```

#### Packaging
```shell
# Allow Conan to install OS requirements
export CONAN_SYSREQUIRES_MODE=enabled

# Create a local Conan package for Pine
conan create . --build missing
```

## Development Plan

[Trello board](https://trello.com/b/iZZPB2t0/pine)

## Acknowledgements
I would like to acknowledge the following projects for inspiring this one:
- [Hazel](https://github.com/TheCherno/Hazel)
- [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine)
