# Pine

[![Build](https://github.com/markvilar/pine/actions/workflows/main_linux.yml/badge.svg)](https://github.com/markvilar/pine/actions/workflows/main_linux.yml)
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

# Install prerequisites
apt install -y cmake gcc python3-dev python3-pip pkg-config 

# Install conan
pip3 install conan

# Set up a default conan profile if one does not exist. Optionally, change the compiler, compiler version, and C++ standard library implementation by updating the conan profile.
conan profile new default --detect

# Install system dependencies
apt install -y libgl-dev libx11-xcb-dev libx11-xcb-dev libfontenc-dev \
    libice-dev libsm-dev libxaw7-dev libxcomposite-dev libxcursor-dev \
    libxdamage-dev libxi-dev libxinerama-dev libxkbfile-dev libxmuu-dev \
    libxrandr-dev libxres-dev libxss-dev libxtst-dev libxv-dev libxvmc-dev \
    libxxf86vm-dev libxcb-render0-dev libxcb-render-util0-dev libxcb-xkb-dev \
    libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev libxcb-randr0-dev \
    libxcb-shape0-dev libxcb-sync-dev libxcb-xfixes0-dev libxcb-xinerama0-dev \
    libxcb-dri3-dev libxcb-util-dev libxcb-util0-dev uuid-dev

# Alternatively, allow Conan to install system dependencies by giving the default profile system install and sudo permission
conan profile update conf.tools.system.package_manager:mode=install default
conan profile update conf.tools.system.package_manager:sudo=True default

# Generate build with CMake
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build
```

#### Packaging with Conan
```shell
conan create . --build missing
```

## Development Plan

[Trello board](https://trello.com/b/iZZPB2t0/pine)

## Acknowledgements
I would like to acknowledge the following projects for inspiring this one:
- [Hazel](https://github.com/TheCherno/Hazel)
- [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine)
