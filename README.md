# Nonortho
[Nonortho](https://cflip.net/projects/nonortho/) is a WIP transport tycoon style isometric tile game.

## Build Instructions
First, clone the repository and use CMake to generate project files for your system.
It is recommended that you create a ``build`` directory for CMake to generate build files into.
```sh
git clone https://github.com/cflip/Nonortho
cd Nonortho
git submodule update
mkdir build && cd build
cmake ../
```
You can then use the generated project files to compile Nonortho.
