/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2010  Andrew Fenn

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

#include "GameManager.h"
#include <Ogre.h>

namespace Client
{
/** The Game Root
        @remarks
            The Client::GameRoot class handles the OS specific startup and shutdown of
        tasks.
    */
class GameRoot : public Ogre::WindowEventListener
{
    public:
        GameRoot();
       ~GameRoot();
        void init();

        /** Attempts to load an Ogre plugin from file. mRoot must be initalised at this point. */
        bool loadPlugin(const Ogre::String);
        /* event window listeners */
        void windowResized(Ogre::RenderWindow *rw);
        bool windowClosing(Ogre::RenderWindow *rw);
        void windowFocusChange(Ogre::RenderWindow *rw);

        virtual bool isLocked() = 0;
        virtual void setLocked(const bool& locked) = 0;
        virtual bool loadPlugins() = 0;

    protected:
        Ogre::Root           *mRoot;
        Ogre::RenderWindow   *mRenderWindow;
        GameManager          *mGameMgr;
        bool configureGame();
};

}

