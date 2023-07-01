# Pine

[![Build](https://github.com/markvilar/pine/actions/workflows/ci.yml/badge.svg)](https://github.com/markvilar/pine/actions/workflows/ci.yml)
[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

## Description
Pine is a lightweight, cross-platform library for creating C++ applications 
with graphics and networking functionality. Pine has been developed with focus 
on maintaining a lean dependency structure, and leveraging modern C++
development tools.

## Requirements
Pine requires the follow tools to be built:
- C++20 compiler (recommended minimum is gcc-10, clang-10)
- CMake 3.19+
- Conan 2.0+

## Dependencies

| **Library** |  **Library purpose.**                |
|-------------|--------------------------------------|
| argparse    |  Command line argument parsing.      |
| asio        |  Networking and asynchronicity.      |
| glad        |  OpenGL 4.6 bindings and loaders.    |
| glfw        |  Window and input handling.          |
| glm         |  Vectorized mathematical operations. |
| spdlog      |  Console and file logging.           |
| stb         |  Image file loading.                 |
|-------------|--------------------------------------|
| imgui       |  GUI windows, widgets, etc.          |
| implot      |  GUI plotting.                       |


## Workflows

### Linux and Conan Workflow

```shell
# Clone pine and checkout a tag
git clone https://github.com/markvilar/pine.git
cd pine
git checkout tags/<tag> -b <branch>

# Install prerequisites
apt install -y cmake gcc clang python3-dev python3-pip pkg-config 

# Install conan
pip3 install --user conan

# Create a default profile
conan profile detect

# Install the dependencies (check out profiles in the `conan/profiles` directory)
conan install . --profile <path/to/profile> --build missing

# Build the package
conan build . --profile <path/to/profile> --build missing

# Create the package
conan create . --profile <path/to/profile> --build missing
```

## Development Plan

[Trello board](https://trello.com/b/iZZPB2t0/pine)

## Acknowledgements
I would like to acknowledge the following projects for inspiring this one:
- [Hazel](https://github.com/TheCherno/Hazel)
- [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine)
