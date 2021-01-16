# FGE - Felix's Game Engine

## Build

### Requirements

- CMake 3.17 or higher
- Ninja (optional)
- Git
- Doxygen (optional)

To build the project run the following commands:
```
git clone --recursive https://github.com/FlexW/fge fge
cd fge
git submodule update --init --recursive
mkdir build
cd build
cmake \
  .. \
  # -G Ninja # For faster building
  # -DBUILD_SHARED_LIBS=ON # For building shared libraries
  # -DCMAKE_EXPORT_COMPILE_COMMANDS=ON # Export compilation database
  # -DCMAKE_BUILD_TYPE=Debug # Build with debug symbols
  # -DCMAKE_C_COMPILER=clang # Use clang as C compiler
  # -DCMAKE_CXX_COMPILER=clang++ # Use clang++ as C++ compiler
make # or ninja if you have picked -G Ninja
```
In the follwoing pick `ninja` instead of `make` if you have set `-G Ninja` in the previous step.

Execute tests with
```
make test
```

Finally install the library with
```
make install # Needs root rights
```
