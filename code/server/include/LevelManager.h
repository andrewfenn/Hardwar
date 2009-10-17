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

#ifndef __LEVEL_MGR_H_
#define __LEVEL_MGR_H_

#include <Ogre.h>
#include <sqlite3.h>

#include "hwstructs.h"
#include "ClientManager.h"

namespace Server
{
   typedef std::multimap<unsigned int,HWBuilding> Building;

   class LevelManager
   {
      public:
         ~LevelManager(void);
         LevelManager(void);
         static LevelManager* getSingletonPtr(void);
         /** Load up our world.db file which contains building locations and other 
             data which is used to get a game going.
            @param name
                   The filename being used which contains the level data.
         */
         bool loadData(Ogre::String);
         /** Loads all the building data in the SQL file */
         bool loadBuildings(void);
         bool addBuilding(const unsigned int crater, const HWBuilding);

         Building::iterator getBuildings(void);
         bool end(Building::iterator);

         void sendBuildingData(unsigned int crater, const HWBuilding building, ENetPeer* lpeer = 0);
         unsigned int numBuildings(void);
      private:
         Building mBuildings;
         sqlite3 *mSQLdb;
         static LevelManager *mLevelManager;
         LevelManager(const LevelManager&) { }
         LevelManager & operator = (const LevelManager&);
   };
}
#endif /* __LEVEL_MGR_H_ */
