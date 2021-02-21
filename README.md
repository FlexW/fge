# FGE - Felix's Game Engine

## Build

Be sure OpenGL 4.6 is available.

Clone the repo and init submodules
```
git clone https://github.com/FlexW/fge
cd fge
git submodule update --init --recursive
```

### Linux

Install all needed packages for your distro. The needed packages for Fedora are:
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

Then build the project:
```
mkdir build
cd build
cmake -G Ninja ..
ninja
```

### Windows
To build with Visual Studio just open Visual Studio and then open the project directory. 
Visual Studio will automatically configure everything. Choose `editor.exe` as start up element.

## Models

Character model is from
[ThinMatrix](https://www.youtube.com/watch?v=z0jb1OBw45I) other models
are downloaded from Morgan McGuire's [Computer Graphics
Archive](https://casual-effects.com/data)

## Development

### Tests
Run the tests with `ninja check`. It's also possible to execute the
tests with `ninja test` but this will you get no output on
failure. Note that `ninja check` will not (re)build the project so for
development it might be handy to execute `ninja && ninja check`
instead of just `ninja check`.
