# Flag-simulator
This is a program using openGL to do flag simulation. The physics simulation is based on the array of particles on the flag, while the shading is done with Minnaert shading algorithm.

## Requirements
1. cmake
2. conan - package manager
```bash
pip3 install conan
```

## Project setup
```bash
mkdir build && cd build
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan install .. --build glad -sbuild_type=Debug
```

## Configure
```bash
cd build
cmake .. -G "Visual Studio 15 2017 Win64"
```

## Build
```bash
cd build
cmake --build .
```

## Thirdparty
1. GLFW3
2. ImGui
