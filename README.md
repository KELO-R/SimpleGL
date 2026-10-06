# SimpleGL is a helper library for OpenGL

## Content
Wrappers for core OpenGL objects, such as:
* meshes
* textures
* shaders

## Requirements
SimpleGL requires C++ 23

## Build

clone repository via
```
git clone --recurse-submodules https://github.com/KELO-R/SimpleGL.git
```

CMake is supported. Examples are not build by default, they can be enabled via -DSIMPLE_GL_USE_EXAMPLES=ON option.
```
cd path\to\project
mkdir build
cmake -S. -B build
```

## Examples
Project has example projects that show how SimpleGL can be used.
