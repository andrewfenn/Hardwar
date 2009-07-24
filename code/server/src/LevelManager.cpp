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

bool LevelManager::loadData(Ogre::String name)
{
   int result;

   result = sqlite3_open(name.c_str(), &mSQLdb);
   if(result)
   {
      /* couldn't load the file */
      fprintf(stderr, "'%s' could not be opened.\n", name.c_str());
      fprintf(stderr, "SQLite Error: %s\n", sqlite3_errmsg(mSQLdb));

      sqlite3_close(mSQLdb);
      return false;
   }

   if (loadBuildings())
   {
      printf("Loaded buildings: %d\n", numBuildings());
      return true;
   }
   else
   {
      fprintf(stderr, "Problem loading buildings\n");
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
   HWBuilding lBuilding;

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
			   lBuilding.mesh = Ogre::String((const char * )
                                              sqlite3_column_text(statement,2));
			   /* get position */
			   lBuilding.position.x = sqlite3_column_double(statement,3);
			   lBuilding.position.y = sqlite3_column_double(statement,4);
			   lBuilding.position.z = sqlite3_column_double(statement,5);
			   /* get rotation */
			   lBuilding.rotation.x = sqlite3_column_double(statement,6);
			   lBuilding.rotation.y = sqlite3_column_double(statement,7);
			   lBuilding.rotation.z = sqlite3_column_double(statement,8);

            mBuildings.insert(std::pair<unsigned int,HWBuilding>(sqlite3_column_int(statement,1), lBuilding));
		   break;
	   }
   }	

   result = sqlite3_finalize(statement);
   if(result != SQLITE_OK )
   {
      fprintf(stderr, "SQLite Error: %s\n", sqlite3_errmsg(mSQLdb));
      lResult = false;
   }
   return lResult;
}

bool LevelManager::addBuilding(unsigned int crater, const Ogre::String mesh, 
                               const Ogre::Vector3 position, 
                               const Ogre::Vector3 rotation)
{
   HWBuilding lBuilding;
   lBuilding.position = position;
   lBuilding.rotation = rotation;
   lBuilding.mesh = mesh;
   mBuildings.insert(std::pair<unsigned int,HWBuilding>(crater, lBuilding));
   printf("New Building - Position: %s - Rotation: %s - Mesh: %s\n",
                              Ogre::StringConverter::toString(position).c_str(),
                              Ogre::StringConverter::toString(rotation).c_str(),
                              mesh.c_str());


   sendBuildingData(crater, mesh, position, rotation);
   return true;
}

void LevelManager::sendBuildingData(unsigned int crater, const Ogre::String mesh, const Ogre::Vector3 position, const Ogre::Vector3 rotation, ENetPeer* lpeer)
{
   ClientManager* lClientMgr = ClientManager::getSingletonPtr();

   /* Tell all new clients about the new building */
   lClientMgr->sendMsg("addbuilding", sizeof("addbuilding")+1, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE, lpeer);
   /* position */
   lClientMgr->sendMsg(Ogre::StringConverter::toString(position.x).c_str(), strlen(Ogre::StringConverter::toString(position.x).c_str())+1, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE, lpeer);
   lClientMgr->sendMsg(Ogre::StringConverter::toString(position.y).c_str(), strlen(Ogre::StringConverter::toString(position.y).c_str())+1, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE, lpeer);
   lClientMgr->sendMsg(Ogre::StringConverter::toString(position.z).c_str(), strlen(Ogre::StringConverter::toString(position.z).c_str())+1, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE, lpeer);

   /* Rotation (which is zero) */
   lClientMgr->sendMsg(Ogre::StringConverter::toString(rotation.x).c_str(), strlen(Ogre::StringConverter::toString(rotation.x).c_str())+1, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE, lpeer);
   lClientMgr->sendMsg(Ogre::StringConverter::toString(rotation.y).c_str(), strlen(Ogre::StringConverter::toString(rotation.y).c_str())+1, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE, lpeer);
   lClientMgr->sendMsg(Ogre::StringConverter::toString(rotation.z).c_str(), strlen(Ogre::StringConverter::toString(rotation.z).c_str())+1, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE, lpeer);

   /* mesh name */
   lClientMgr->sendMsg(mesh.c_str(), strlen(mesh.c_str())+1, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE, lpeer);
}

LevelManager* LevelManager::getSingletonPtr(void)
{
   if(!mLevelManager)
   {
      mLevelManager = new LevelManager();
   }

   return mLevelManager;
}
