
# Hardwar - Computer Game, Sci-fi Flight sim

This is a re-make of another game with the same name which is also called "Hardwar".

The game is currently is pre-alpha state. There is currently no game to play.

If you are interested in helping develop the game then please see below for build instructions.

## Build prerequisites

Hardwar requires the following libraries be pre-installed.

* Ogre3D - 1.8.0
* OIS - 1.2
* CMake
* The Boost libraries
* CEGUI

## Building

The building instructions should be similar regardless of if you're on windows, osx or linux.

###Linux

    mkdir hardwar (git clone here)
    mkdir build && cd ./build
    cmake ../hardwar
    make

###Windows and OSX

Copy the same steps as above, create your git repo and use cmake to generate either visual studio project files or xcode files for your platform.

You should then only have to open the xcode or visual studio file and compile. If that doesn't work please file bugs or pull requests.
