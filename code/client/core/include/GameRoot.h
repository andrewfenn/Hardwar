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

#include <Ogre.h>
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
   #include <dirent.h>
#endif
#include "GameManager.h"

namespace Client
{

class GameRoot : public Ogre::WindowEventListener
{
   public:
      GameRoot();
      ~GameRoot();
      void init();

      /* event window listeners */
      void windowResized(Ogre::RenderWindow *rw);
      bool windowClosing(Ogre::RenderWindow *rw);
      void windowFocusChange(Ogre::RenderWindow *rw);
   private:
      Ogre::Root           *mRoot;
      Ogre::RenderWindow   *mRenderWindow;
      GameManager          *mGameMgr;

      bool loadPlugin(const Ogre::String);
      bool loadPlugins();
      bool configureGame();
      void setupResources();
};
}

