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
- C++17 compiler
- CMake 3.19+
- Conan 2.0+

## Dependencies

| **Library** | **Version**  | **Library purpose.**                |
|-------------|--------------|-------------------------------------|
| argparse    | >=2.9        | Command line argument parsing.      |
| asio        | 1.21.0       | Networking and asynchronicity.      |
| glad        | 0.1.36       | OpenGL 4.6 bindings and loaders.    |
| glfw        | 3.3.4        | Window and input handling.          |
| glm         | 0.9.9.8      | Vectorized mathematical operations. |
| spdlog      | 1.9.2        | Console and file logging.           |
| stb         | cci.20210713 | Image file loading.                 |
|-------------|--------------|-------------------------------------|
| imgui       | 1.85         | GUI windows, widgets, etc.          |
| implot      | 0.14         | GUI plotting.                       |


## Workflows

### Linux Workflow

#### Building

```shell
# Clone pine and checkout a tag
git clone https://github.com/markvilar/pine.git
cd pine
git checkout tags/<tag> -b <branch>

# Install prerequisites
apt install -y cmake gcc python3-dev python3-pip pkg-config 

# Install conan
pip3 install conan

# Create a Conan profile
conan profile detect --name <profile>

# Build the package
conan build . \
    -s build_type=Release \
    -s compiler.cppstd=17 \
    -pr <profile> \
    --build missing

# Create the package
conan create . \
    -s build_type=Release \
    -s compiler.cppstd=17 \
    -pr <profile> \
    --build missing
```

## Development Plan

[Trello board](https://trello.com/b/iZZPB2t0/pine)

## Acknowledgements
I would like to acknowledge the following projects for inspiring this one:
- [Hazel](https://github.com/TheCherno/Hazel)
- [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine)
