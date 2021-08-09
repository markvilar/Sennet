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

## Building with CMake

<details>
<summary>Single configuration generators (Unix Makefiles)</summary>

- Debug mode:
```
git clone https://gitub.com/markvilar/Sennet.git
cd Sennet
cmake -S . -B build/ -G "Unix Makefiles" "-DCMAKE_BUILD_TYPE=Debug"
cmake --build build/
```

- Realese mode:
```
git clone https://gitub.com/markvilar/Sennet.git
cd Sennet
cmake -S . -B build/ -G "Unix Makefiles" "-DCMAKE_BUILD_TYPE=Realese"
cmake --build build/
```
</details>


<details>
<summary>Multiple configuration generators (Visual Studio)</summary>

- Debug mode:
```
git clone https://gitub.com/markvilar/Sennet.git
cd Sennet
cmake -S . -B build/ -G "Visual Studio 16 2019" -A x64
cmake --build build/ --config Debug
```

- Realese mode:
```
git clone https://gitub.com/markvilar/Sennet.git
cd Sennet
cmake -S . -B build/ -G "Visual Studio 16 2019" -A x64
cmake --build build/ --config Realese
```
</details>

## Optional Dependencies

### GLFW

Sennet currently uses GLFW for its window and input system. Depending on the
windowing system of the OS, GLFW requires installation of additional libraries
on Linux. For example, Linux distributions using the X11 windowing system
requires installation of the following dependencies: `libxrandr-dev` 
`libxinerama-dev` `libxcursor-dev` `libxi-dev` `libxext-dev`

To install via `apt`:
```
apt install libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxext-dev
```

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
