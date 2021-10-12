# Sennet 
[![Build Status](https://travis-ci.com/markvilar/Sennet.svg?branch=master)](https://travis-ci.com/markvilar/Sennet)

## Description
Sennet is a static, cross-platform library for creating C++ applications 
with graphics and networking functionality, that is developed with focus on 
geometric data visualization and sensor networking.

## Requirements
The requirements are:
- C++17 compiler
- CMake 3.16 (or higher)

## Dependencies

asio
EnTT
glad (OpenGL 4.6+)
glfw
glm
imgui
spdlog

## Building with CMake and Conan

<details>
<summary>Single configuration generators</summary>

- Debug mode:
```
git clone https://gitub.com/markvilar/Sennet.git
cd Sennet
mkdir build && cd build
conan install .. -s build_type=Debug
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

- Realese mode:
```
git clone https://gitub.com/markvilar/Sennet.git
cd Sennet
mkdir build && cd build
conan install .. -s build_type=Release
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Realese
cmake --build .
```
</details>


<details>
<summary>Multiple configuration generators</summary>

```
git clone https://gitub.com/markvilar/Sennet.git
cd Sennet
mkdir build && cd build
conan install .. -s build_type=Release
conan install .. -s build_type=Debug
cmake .. -G "Ninja Multi-Config" -A x64
cmake --build . --config Release
```

</details>

## Development Plan

[Trello board](https://trello.com/b/iZZPB2t0/sennet)

## Acknowledgements
I would like to acknowledge the following projects for inspiring this one:
- [Hazel](https://github.com/TheCherno/Hazel)
- [FlexEngine](https://github.com/ajweeks/FlexEngine)
- [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine)
- [anki-3d-engine](https://github.com/godlikepanos/anki-3d-engine)
- [cilantro](https://github.com/kzampog/cilantro)
- [tinyply](https://github.com/ddiakopoulos/tinyply)
