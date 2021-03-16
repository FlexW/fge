# FGE - Game Engine

## Build

You need a C++17 compiler like `clang++` or `g++` and `conan`, `cmake`
and `ninja` installed.

Note, that you might need to set the following in `~/.conan/profiles/default`:
```
[settings]
os=Linux
os_build=Linux
arch=x86_64
arch_build=x86_64
compiler=gcc
compiler.version=10
compiler.libcxx=libstdc++11
build_type=Release
[options]
[build_requires]
[env]

```
Execute the following to build the project:

```
mkdir build
cd build
conan install ..
cmake .. -G Ninja
ninja
```

## Tests

```
ninja test
```
