/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2008-2010  Andrew Fenn
    
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

#include <Ogre.h>

#include "RootGameState.h"
#include "GameTask.h"

namespace Client
{
/** The Game Manager
        @remarks
            The Client::GameManager class is our root class which deals with the
            initialisation and running of our game.
    */
class GameManager
{
    public:
        Ogre::SceneManager   *mSceneMgr;

        GameManager(Ogre::Root*);
        ~GameManager();

        /** Signals to begin shutdown of the game. */
        void shutdown();
        void windowChangedSize(Ogre::RenderWindow*);
        void windowChangedFocus(Ogre::RenderWindow*);

    private:
        void preloadResources();

        Ogre::Root           *mRoot;
        Ogre::RenderWindow   *mRenderWindow;
        Ogre::Camera         *mCamera;
        Ogre::Viewport       *mViewport;
        /** The root state that the game is on. New states can be children to this root state.
        States are used to seperate game functionality such as menus, loading screens, in game, etc. */
        RootGameState        *mRootState;

        /** If true will shutdown the game */
        bool                 mShutdown;
        /** If the game has started rendering */
        bool                 mStarted;
        /** List of tasks that require running when performing an update */
        GameTaskList         mTasks;

        /** Delay time in miliseconds calculated from the max fps */
        unsigned short       mDelayTime;
        /** Maximum number of frames per second to allow before sleeping */
        unsigned short       mMaxFPS;
};
}

