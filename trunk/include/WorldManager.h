/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright (C) 2008  Andrew Fenn
    
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

#ifndef __HW_WORLD_MANAGER_H_
#define __HW_WORLD_MANAGER_H_

#include <OgreString.h>
#include <OgreEntity.h>
#include <OgreStringConverter.h>
#include <sqlite3.h>

#include "hwstructs.h"


class WorldManager {
   public:
      WorldManager( void );
      ~WorldManager( void );
      bool loadWorldData(Ogre::String);
      bool saveWorldData();
      bool loadBuildings();
      bool deleteBuilding(Ogre::String); 
      bool addBuilding(Ogre::Vector3, const char*);
      Ogre::SceneManager      *mSceneMgr;	
   private:
      std::vector<Building>   mBuildings;
      unsigned int            mBuildCount;
      /* All buildings should be attached to the world node */
      Ogre::SceneNode         *mWorldNode;
      Ogre::Root              *mRoot;
      sqlite3                 *mSQLdb;
      bool drawBuildings();
};
#endif

