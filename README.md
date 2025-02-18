# Nimble-Renderer
Nimble is a part time project of mine to create a tool for myself creating my own Video game!\
It consists of different parts:
- ***The Renderer***
- Math library
- Engine
- Game
This Project is the pure renderer that can be reused anywhere else.\
The Project is open to use for everyone under the [MIT License](https://mit-license.org/).

## Rendering
Rendering is achieved by using the [glfw](https://www.glfw.org/) library with [OpenGL](https://www.opengl.org/).

## Cloning the repo:
```sh
git clone --recurse-submodules https://github.com/BigAgg/Nimble-Renderer.git
```

## Building the Renderer
### Windows:
***Requirements***:
- [CMake](https://cmake.org/) Version 3.8 or higher
- Any c++ Compiler that supports c++20 or higher like gcc

### Preparing the Project:
cd into the cloned repository, create a folder named *build* and cd into the *build* folder
```sh
mkdir build
cd build
cmake ..
```
If you are using **Visual Studio**, it generates a *Solution File* inside the build directory that is ready to use.\
To test the engine simply open the *Solution File* and select an example project of your joice by right clicking it and setting it as default project and click build and run.

### Building debug:
cd into the build directory *Preparing the Project* and build it in debug:
```sh
cmake --build .
```
After building the Project in **debug mode** you can find the **Library** under "build/Debug/NimbleRenderer.lib"

### Building release:
cd into the build directory *Preparing the Project* and build it in release:
```sh
cmake --build . --config Release
```
After building the Project in **release mode** you can find the **Library** under "build/Release/NimbleRenderer.lib"

### Linux:
Linux is currently not supported but it is planned to be in the future!

## Implementing NimbleRenderer.h 
To implement the Renderer you just need to link the generated library and include directory

## Resources
[glfw](https://www.glfw.org/)\
[OpenGL](https://www.opengl.org/)

### External deps
[glad](https://github.com/Dav1dde/glad/tree/c)\
[stb](https://github.com/nothings/stb/blob/master/stb_image.h)

## License
[MIT](https://mit-license.org/)
