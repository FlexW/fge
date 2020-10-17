# FGE - Felix's Game Engine

## Build

Be sure OpenGL 4.6 is available.

### Linux

Clone the repo and init submodules
```
git clone https://github.com/FlexW/fge
cd fge
git submodule update --init --recursive
```

Packages for Fedora:
```
cmake
ninja
libXcursor-devel
libXrandr-devel
libXinerama-devel
libXext-devel
libXi-devel
mesa-libGL-devel
```

```
mkdir build
cd build
cmake ..
make
```

## Models

Character model is from
[ThinMatrix](https://www.youtube.com/watch?v=z0jb1OBw45I) other models
are downloaded from Morgan McGuire's [Computer Graphics
Archive](https://casual-effects.com/data)
