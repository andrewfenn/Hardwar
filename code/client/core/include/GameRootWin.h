/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2012  Andrew Fenn

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

namespace Client
{
/** The Game Root Windows
        @remarks
            The Client::GameRootPlatform is a parent class that handles the windows specific implementation
        of functionality that GameRoot inherits.
    */
class GameRootPlatform
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
};

}

