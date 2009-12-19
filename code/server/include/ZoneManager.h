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

#ifndef __ZONE_MGR_H_
#define __ZONE_MGR_H_

#include <Ogre.h>
#include <sqlite3.h>
#include <vector>
#include <libintl.h>

#include "Zone.h"

namespace Server
{
   typedef std::vector<Zone> Zones;

   class ZoneManager
   {
      public:
         ZoneManager();
         bool loadData(Ogre::String name);
         ~ZoneManager();
      private:
         sqlite3 *mSQLdb;
         Zones mZones;

         bool loadBuildings();
   };
}
#endif /* __ZONE_MGR_H_ */
