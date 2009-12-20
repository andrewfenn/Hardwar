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

#ifndef __ZONE_H_
#define __ZONE_H_

#include <Ogre.h>
#include <sqlite3.h>
#include <stdio.h>
#include <libintl.h>

#include "Building.h"

namespace Server
{
   typedef std::multimap<unsigned int,Hardwar::Building> Buildings;

   class Zone
   {
      public:
         Zone();
         ~Zone();
         bool addBuilding(const unsigned int id, Hardwar::Building);
         Buildings getBuildings(void);
         unsigned int numBuildings(void);
      private:
         Buildings mBuildings;
   };
}
#endif /* __ZONE_H_ */
