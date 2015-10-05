
# Hardwar - Computer Game, Sci-fi Flight sim

This is a re-make of another game with the same name which is also called "Hardwar".

The game is currently is pre-alpha state. There is currently no game to play.

If you are interested in helping develop the game then please see below for build instructions.

## Building

The building instructions should be similar regardless of if you're on windows, osx or linux.

###Linux

    sudo apt-get install libfreetype6-dev nvidia-cg-toolkit libfreeimage-dev \
    zlib1g-dev libzzip-dev libois-dev libcppunit-dev doxygen libxt-dev libxaw7-dev \
    libxxf86vm-dev libxrandr-dev libglu-dev libboost-date-time-dev libboost-thread-dev \
    git npm nodejs-legacy cmake

    sudo npm install -g bower
    bower install
    cmake -H. -B./hard.build
    cd ./hard.build
    make
