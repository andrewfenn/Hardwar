
# Hardwar - Computer Game, Sci-fi Flight sim

This is a re-make of another game with the same name which is also called "Hardwar".

The game is currently is pre-alpha state. There is currently no game to play.

If you are interested in helping develop the game then please see below for build instructions.

## Building

The building instructions should be similar regardless of if you're on windows, osx or linux.

For library dependency management I use bower which requires node-js and npm to be installed. After you have these installed you should be able to download the code and run "bower install" to get all the dependencies needed to build the game.

###Debian / Ubuntu

    sudo apt-get install libfreetype6-dev nvidia-cg-toolkit libfreeimage-dev \
    zlib1g-dev libzzip-dev libois-dev libcppunit-dev doxygen libxt-dev libxaw7-dev \
    libxxf86vm-dev libxrandr-dev libglu-dev libboost-date-time-dev libboost-thread-dev \
    libboost-filesystem-dev git npm nodejs-legacy cmake

    git clone https://github.com/andrewfenn/Hardwar.git
    cd ./Hardwar
    sudo npm install -g bower
    bower install
    cmake -H. -B./hard.build
    cd ./hard.build
    make

###OSX

    Download/Install XCode from AppStore
    Open XCode and agree to the license agreement
    Download/Install XCode Command Line Tools: https://developer.apple.com/downloads/?name=for%20Xcode
    Download/Install brew: http://brew.sh/
    Download/Install nvidia-cg-toolkit: https://developer.nvidia.com/cg-toolkit-download
    brew install git cmake node pkgconfig boost libzzip gettext
    sudo brew link gettext --force
    git clone https://github.com/andrewfenn/Hardwar.git
    cd ./Hardwar
    sudo npm install -g bower
    bower install
    cmake -H. -B./hard.build
