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

#include "Game.h"

using namespace Server;

Game::Game()
{
}

bool Game::setup(Ogre::ConfigFile config, ENetHost* host)
{
   mConfig = config;
   mClientMgr.setHost(host);
   if (!mZoneMgr.loadData(mConfig.getSetting("Database", "Game", "./world/default.db")))
   {
      return false;
   }
   return true;
}

ClientManager* Game::getClientMgr()
{
   return &mClientMgr;
}

void Game::addClient(ENetPeer* peer)
{
   mClientMgr.add(peer);
   Client* client = mClientMgr.get(peer);
   if (client)
   {
     client->addBuildings(mZoneMgr.getAllBuildings());
   }
}

void Game::removeClient(ENetPeer* peer)
{
   mClientMgr.remove(peer);
}

void Game::process()
{
   Admin* admin;
   Clients* clients = mClientMgr.list();
   for (Clients::iterator client=clients->begin(); client != clients->end(); client++)
   {
      if (client->second->isAdmin())
      {
         /* Add any buildings that an admin has placed down. */
         admin = client->second->getAdmin();
         Hardwar::Buildings buildings = admin->getBuildings();

         for (Hardwar::Buildings::iterator building=buildings.begin(); building != buildings.end(); building++)
         {
            mZoneMgr.get(building->first)->addBuilding(building->second);
            dataPacket packet = dataPacket(add_building);
            packet = building->second.serialize(packet);
            mClientMgr.send(packet, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE);
         }
      }
   }
}

Game::~Game()
{
}
