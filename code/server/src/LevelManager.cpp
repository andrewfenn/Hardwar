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

#include "LevelManager.h"

using namespace Server;

LevelManager* LevelManager::mLevelManager;

LevelManager::LevelManager(void)
{

}

LevelManager::~LevelManager(void)
{
   if(mSQLdb)
   {
      sqlite3_close(mSQLdb);
	}
}

bool LevelManager::loadData(const Ogre::String name)
{
   int result;
   result = sqlite3_open(name.c_str(), &mSQLdb);
   if(result)
   {
      /* couldn't load the file */
      std::cout << name.c_str() << gettext(" could not be opened.") << std::endl;
      std::cout << gettext("SQLite Error: ") << sqlite3_errmsg(mSQLdb) << std::endl;

      sqlite3_close(mSQLdb);
      return false;
   }

   if (loadBuildings())
   {
      std::cout << gettext("Loaded buildings: ") << numBuildings() << std::endl;
      return true;
   }
   else
   {
      std::cout << gettext("Problem loading buildings") << std::endl;
   }
   return false;
}

Building::iterator LevelManager::getBuildings(void)
{
   Building::iterator lBuilding = mBuildings.begin();
   return lBuilding;
}

bool LevelManager::end(Building::iterator lIter)
{
   if (lIter != mBuildings.end())
   {
      return false;
   }
   return true;
}

unsigned int LevelManager::numBuildings(void)
{
   return mBuildings.size();
}

bool LevelManager::loadBuildings(void)
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

            mBuildings.insert(std::pair<unsigned int,Hardwar::Building>(sqlite3_column_int(statement,1), lBuilding));
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

bool LevelManager::addBuilding(const unsigned int crater, Hardwar::Building building)
{
   mBuildings.insert(std::pair<unsigned int,Hardwar::Building>(crater, building));

   Ogre::Vector3 pos, rot;
   Ogre::String mesh;
   pos = building.getPosition();
   rot = building.getRotation();
   mesh = building.getMeshName();
   printf("New Building - Position: %s - Rotation: %s - Mesh: %s\n",
                              Ogre::StringConverter::toString(pos).c_str(),
                              Ogre::StringConverter::toString(rot).c_str(),
                              mesh.c_str());


   sendBuildingData(crater, building);
   return true;
}

void LevelManager::sendBuildingData(unsigned int crater, Hardwar::Building building, ENetPeer* lpeer)
{
   ClientManager* lClientMgr = ClientManager::getSingletonPtr();

   dataPacket packet = dataPacket(add_building);
   packet.append(&building.getPosition(), sizeof(Ogre::Vector3));
   packet.append(&building.getRotation(), sizeof(Ogre::Vector3));
   packet.appendString(building.getMeshName());
   lClientMgr->sendMsg(packet, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE, lpeer);
}

LevelManager* LevelManager::getSingletonPtr(void)
{
   if(!mLevelManager)
   {
      mLevelManager = new LevelManager();
   }

   return mLevelManager;
}
