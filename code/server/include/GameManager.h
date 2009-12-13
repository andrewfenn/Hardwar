/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright (C) 2009  Andrew Fenn
    
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

#ifndef __SRV_GAME_MANAGER_H_
#define __SRV_GAME_MANAGER_H_

#include <Ogre.h>
#include <libintl.h>

#include "LevelManager.h"

namespace Server
{
   class GameManager
   {
      public:
         ~GameManager();
         GameManager();

         void setConfig(Ogre::ConfigFile);
         bool setup();

         static GameManager* getSingletonPtr();

      private:
         LevelManager mLvlMgr;
         Ogre::ConfigFile mConfig;

         static GameManager *mGameManager;
         GameManager(const GameManager&) { }
         GameManager & operator = (const GameManager&);
   };
}

#endif /* __SRV_GAME_MANAGER_H_ */
