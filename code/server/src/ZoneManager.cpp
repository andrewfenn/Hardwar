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

bool ZoneManager::loadData(Ogre::String name)
{
   int result;
   result = sqlite3_open_v2(name.c_str(), &mSQLdb, SQLITE_OPEN_NOMUTEX, 0);
   if(result)
   {
      /* couldn't load the file */
      std::cout << name.c_str() << gettext(" could not be opened.") << std::endl;
      std::cout << "SQLite Error: " << sqlite3_errmsg(mSQLdb) << std::endl;
      sqlite3_close(mSQLdb);
      return false;
   }

   if (loadBuildings())
   {
      std::cout << gettext("Loaded buildings") << std::endl;
      return true;
   }
   else
   {
      std::cout << gettext("Problem loading buildings") << std::endl;
   }
   return false;
}

bool ZoneManager::loadBuildings(void)
{
   int result;
   bool lResult = true;
   bool done = false;
   sqlite3_stmt* statement;
   Hardwar::Building lBuilding;
   Ogre::Vector3 point;

   /* get all the buildings out the database */
   std::string sql = std::string("SELECT * FROM buildings");
   sqlite3_prepare_v2(mSQLdb,sql.c_str(),sql.size(),&statement,0);
   while((result = sqlite3_step(statement)) && !done) {
	   switch (result) {
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

            mZones.at(sqlite3_column_int(statement,1)).addBuilding(sqlite3_column_int(statement,0), lBuilding);
		   break;
	   }
   }	

   result = sqlite3_finalize(statement);
   if(result != SQLITE_OK )
   {
      std::cout << "SQLite Error: " << sqlite3_errmsg(mSQLdb) << std::endl;
      lResult = false;
   }
   return lResult;
}
