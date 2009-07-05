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

#include "Admin.h"

using namespace Server;

Admin::Admin()
{

}

Admin::~Admin()
{

}

void Admin::processRequest(Message::iterator &message)
{
   char* lstring = (char*)(*message).second.packet->data;
   if (strcmp(lstring, "addbuilding") == 0)
   {
      Ogre::Vector3 building;
      Ogre::String mesh;

      nextPacket(message);
      lstring = (char*)(*message).second.packet->data;
      building.x = Ogre::StringConverter::parseInt(lstring);

      nextPacket(message);
      lstring = (char*)(*message).second.packet->data;
      building.y = Ogre::StringConverter::parseInt(lstring);

      nextPacket(message);
      lstring = (char*)(*message).second.packet->data;
      building.z = Ogre::StringConverter::parseInt(lstring);

      nextPacket(message);
      mesh = Ogre::String((char*)(*message).second.packet->data);

      LevelManager::getSingletonPtr()->addBuilding((unsigned int)0, mesh, building);

      ClientManager* lClientMgr = ClientManager::getSingletonPtr();
      lClientMgr->broadcastMsg("addbuilding", sizeof("addbuilding")+1, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE);
      /* position */
      lClientMgr->broadcastMsg(Ogre::StringConverter::toString(building.x).c_str(), strlen(Ogre::StringConverter::toString(building.x).c_str())+1, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE);
      lClientMgr->broadcastMsg(Ogre::StringConverter::toString(building.y).c_str(), strlen(Ogre::StringConverter::toString(building.y).c_str())+1, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE);
      lClientMgr->broadcastMsg(Ogre::StringConverter::toString(building.z).c_str(), strlen(Ogre::StringConverter::toString(building.z).c_str())+1, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE);

      /* Rotation (which is zero) */
      lClientMgr->broadcastMsg(Ogre::String("0").c_str(), strlen(Ogre::String("0").c_str())+1, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE);
      lClientMgr->broadcastMsg(Ogre::String("0").c_str(), strlen(Ogre::String("0").c_str())+1, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE);
      lClientMgr->broadcastMsg(Ogre::String("0").c_str(), strlen(Ogre::String("0").c_str())+1, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE);

      /* mesh name */
      lClientMgr->broadcastMsg(mesh.c_str(), strlen(mesh.c_str())+1, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE);

   }
   else
   {
      printf ("len:%u - value:%s - channel %u.\n",
                                (intptr_t) (*message).second.packet->dataLength,
                                         (char*) (*message).second.packet->data,
                                                              (*message).first);
   }
}

void Admin::nextPacket(Message::iterator &message)
{
   enet_packet_destroy((*message).second.packet);
   /* FIXME: This could screw up if the packet hasn't arrived yet */
   message++;
}
