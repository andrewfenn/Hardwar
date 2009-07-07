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

#include "Server.h"

using namespace Server;

ServerMain::ServerMain()
{ }

ServerMain::ServerMain(Ogre::ConfigFile config)
{
   mConfig = config;
   if (setup(Ogre::StringConverter::parseInt(mConfig.getSetting("DefaultPort", "Network", "26500")),
                  mConfig.getSetting("LocalAddress", "Network", "127.0.0.1")) == 1)
   {
      /* setup number of avaliable clients */
      serverLoop();
   }
}

ServerMain::~ServerMain()
{
   enet_host_destroy(mServer);
   enet_deinitialize();
}

bool ServerMain::setup(int port, Ogre::String ip)
{
   ENetAddress address;
   mServer=NULL;

   if (enet_initialize() != 0)
   {
      printf(gettext("Unable to initialize the network library.\n"));
      return false;
   }

   address.host = ENET_HOST_ANY;
   address.port = port;

   printf(gettext("Starting server\n"));
   printf(gettext("Port: %d\n"), port);

   mServer = enet_host_create (&address /* the address to bind the server host to */, 
                              32      /* allow up to 32 clients and/or outgoing connections */,
                               0      /* assume any amount of incoming bandwidth */,
                               0      /* assume any amount of outgoing bandwidth */);
   if (mServer == NULL)
   {
      printf(gettext("An error occurred while trying to create an ENet server host.\n"));
      enet_deinitialize();
      return false;
   }

   mClientMgr = ClientManager::getSingletonPtr();
   mClientMgr->setHost(mServer);

   if (setupGame())
   {
      return true;
   }
   return false;
}

bool ServerMain::setupGame()
{
   bool result = true;

   mLvlMgr = new LevelManager;
   result = mLvlMgr->loadData(Ogre::String("world/default.db"));

   return result;
}

void ServerMain::serverLoop()
{
   bool serverRunning = true;
   printf(gettext("Server running..\n"));
   ENetEvent lEvent;

   while (serverRunning)
   {
      enet_host_flush(mServer);
      /* Wait up to 1000 milliseconds for an event. */
      while (enet_host_service(mServer, &lEvent, 1000) > 0)
      {
         switch (lEvent.type)
         {
            case ENET_EVENT_TYPE_CONNECT:
               mClientMgr->addClient(lEvent.peer);
            break;
            case ENET_EVENT_TYPE_RECEIVE:
               switch (lEvent.channelID)
               {
                  default:
                     /* Any packets we recieve are added to the specific client that needs
                        them. They are then deleted in the thread after they have been
                        used. */
                        mClientMgr->addMessage(lEvent);
                  break;
               }
            break;
            case ENET_EVENT_TYPE_DISCONNECT:
               mClientMgr->removeClient(lEvent.peer);
            break;
            default:
            break;
         }
      }
   }
}
