/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2009-2010  Andrew Fenn
    
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

#include "ZoneManager.h"

using namespace Server;

ZoneManager::ZoneManager()
{
   mZones.push_back(Zone());
}

ZoneManager::~ZoneManager()
{
   if(mSQLdb)
   {
      sqlite3_close(mSQLdb);
	}
}

bool ZoneManager::openDatabase(Ogre::String name)
{
   int result;
	int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
   result = sqlite3_open_v2(name.c_str(), &mSQLdb, flags, 0);
   if(result != SQLITE_OK)
   {
      /* couldn't load the file */
      std::cout << name.c_str() << gettext(" could not be opened.") << std::endl;
      std::cout << "SQLite Error: " << sqlite3_errmsg(mSQLdb) << std::endl;
      sqlite3_close(mSQLdb);
      return false;
   }
   return true;
}

bool ZoneManager::loadData(Ogre::String name)
{
   mFilename = name;
   if (!openDatabase(name))
   {
      return false;
   }

   if (!loadBuildings())
   {
      std::cout << gettext("Error loading buildings") << std::endl;
      return false;
   }

   std::cout << gettext("Loaded buildings") << std::endl;
   return true;
}

void ZoneManager::saveWorld()
{
   Hardwar::Buildings buildings = getAllBuildings();
   unsigned int zone;
   Ogre::Vector3 pos;
   Ogre::Vector3 rot;

   /* TODO: Save process..
      1) touch new file
      2) create new file
      3) save data to new file
      4) close new file
      5) rename old file to .backup
      6) rename created file to old file name
      7) delete old file
   */
   int error;
   char * errorMsg;

   error = sqlite3_exec(mSQLdb, "DELETE FROM buildings WHERE 1=1;", 0, 0, &errorMsg);
   if (error != SQLITE_DONE)
   {
      std::cout << "SQLite Error: " << errorMsg << std::endl;
      sqlite3_close(mSQLdb);
   }

   std::string sql = std::string("INSERT INTO buildings \
               (`id`,`crater`,`mesh`,`position_x`,`position_y`, \
               `position_z`,`rotation_x`,`rotation_y`,`rotation_z`) \
               VALUES(NULL, ?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8);");
   sqlite3_stmt* statement;
   for (Hardwar::Buildings::iterator building=buildings.begin(); building != buildings.end(); building++)
   {
      zone = building->first;
      Hardwar::Building hanger = building->second;
      pos = hanger.getPosition();
      rot = hanger.getRotation();

      sqlite3_prepare_v2(mSQLdb, sql.c_str(), sql.size(), &statement, 0);
      sqlite3_bind_int(statement, 1, zone);
      sqlite3_bind_text(statement, 2, hanger.getMeshName().c_str(), hanger.getMeshName().length(), 0);
      sqlite3_bind_int(statement, 3, pos.x);
      sqlite3_bind_int(statement, 4, pos.y);
      sqlite3_bind_int(statement, 5, pos.z);
      sqlite3_bind_int(statement, 6, rot.z);
      sqlite3_bind_int(statement, 7, rot.z);
      sqlite3_bind_int(statement, 8, rot.z);

      if (sqlite3_step(statement) != SQLITE_DONE)
      {
         std::cout << "SQLite Error: " << sqlite3_errmsg(mSQLdb) << std::endl;
         sqlite3_finalize(statement);
         sqlite3_close(mSQLdb);
      }
      sqlite3_reset(statement);
   }
   sqlite3_finalize(statement);
   std::cout << gettext("Saved buildings") << ":" << buildings.size() << std::endl; 
}

Hardwar::Buildings ZoneManager::getAllBuildings()
{
   Hardwar::Buildings master;
   unsigned int zoneCount = 0;
   for (Zones::iterator zone=mZones.begin(); zone != mZones.end(); zone++)
   {
      std::vector<Hardwar::Building> list = zone->getBuildings();
      std::vector<Hardwar::Building>::iterator building;
      for (building=list.begin(); building != list.end(); building++)
      {
         master.insert(std::pair<unsigned int,Hardwar::Building>(zoneCount, (*building)));
      }
      zoneCount++;
   }
   return master;
}

bool ZoneManager::loadBuildings()
{
   int result;
   bool lResult = true;
   bool done = false;
   sqlite3_stmt* statement;
   Hardwar::Building lBuilding;
   Ogre::Vector3 point;

   /* get all the buildings out the database */
   std::string sql = std::string("SELECT * FROM buildings");
   if (SQLITE_OK != sqlite3_prepare_v2(mSQLdb, sql.c_str(), -1, &statement, 0))
   {
      std::cout << "SQLite error: " << sqlite3_errmsg(mSQLdb) << std::endl;
      sqlite3_finalize(statement);
      return false;
   }

   while((result = sqlite3_step(statement)) && !done)
   {
      switch (result)
      {
         default:
	      case SQLITE_DONE:
		      done = true;
	      break;
	      case SQLITE_ROW:
		      /* get mesh name */
		      lBuilding.setMeshName(Ogre::String((const char*) sqlite3_column_text(statement,2)));
		      /* get position */
		      point.x = sqlite3_column_double(statement,3);
		      point.y = sqlite3_column_double(statement,4);
		      point.z = sqlite3_column_double(statement,5);
            lBuilding.setPosition(point);

		      /* get rotation */
		      point.x = sqlite3_column_double(statement,6);
		      point.y = sqlite3_column_double(statement,7);
		      point.z = sqlite3_column_double(statement,8);
            lBuilding.setRotation(point);

            get(sqlite3_column_int(statement,1))->addBuilding(lBuilding);
	      break;
      }
   }

   result = sqlite3_finalize(statement);
   if(result != SQLITE_OK )
   {
      std::cout << "SQLite error: " << sqlite3_errmsg(mSQLdb) << std::endl;
      lResult = false;
   }
   return lResult;
}

Zone* ZoneManager::get(unsigned int zone)
{
   return &mZones.at(zone);
}
