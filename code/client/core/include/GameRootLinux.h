/*
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2012 - 2015  Andrew Fenn

    Hardwar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Hardwar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "GameRoot.h"

namespace Client
{
    /** The Game Root Linux
            @remarks
                The Client::GameRootPlatform is a parent class that handles the linux specific
            implementation of functionality that GameRoot inherits.
        */
    class GameRootLinux: public GameRoot
    {
        public:
            /** Check if the game is already running.
            @return bool true if not running, false if another process has already started. */
            bool isLocked();
            /** Sets a file somewhere platform dependent that notifys other instances of the game
            that it is running. */
            void setLocked(const bool& locked);
            /** Attempts to load an Ogre plugin from file. mRoot must be initalised at this point. */
            bool loadPlugins();
            /** Gets the location of the user's home directory */
            Ogre::UTFString getHomeDirectory();
        private:
            void createHomeDir();
    };

    class GameRootSystem : public GameRootLinux {};
}

