/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright (C) 2008-2009  Andrew Fenn
    
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

namespace Server
{
   ServerMain::ServerMain(Ogre::ConfigFile config)
   {
      if (setup(Ogre::StringConverter::parseInt(mConfig.getSetting("DefaultPort", "Network", "26500")),
                     mConfig.getSetting("LocalAddress", "Network", "127.0.0.1")) == 1)
      {
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
      mServer=0;

      if (enet_initialize() != 0)
      {
         std::cout << gettext("Unable to initialize the network library.") << std::endl;
         return false;
      }

      address.host = ENET_HOST_ANY;
      address.port = port;

      std::cout << gettext("Starting server") << std::endl;
      std::cout << gettext("Port:") << port << std::endl;

      mServer = enet_host_create (&address /* the address to bind the server host to */, 
                                 32      /* allow up to 32 clients and/or outgoing connections */,
                                  0      /* assume any amount of incoming bandwidth */,
                                  0      /* assume any amount of outgoing bandwidth */);
      if (mServer == 0)
      {
         std::cout << gettext("An error occurred while trying to create an ENet server host.") << std::endl;
         enet_deinitialize();
         return false;
      }

      if (!mGame.setup(mConfig, mServer))
      {
         return false;
      }
      return true;
   }

   void ServerMain::serverLoop()
   {
      bool serverRunning = true;
      std::cout << gettext("Server running..") << std::endl;
      ENetEvent lEvent;

      while (serverRunning)
      {
         process();
         /* Wait up to 500 milliseconds for an event. */
         while (enet_host_service(mServer, &lEvent, 500) > 0)
         {
            switch (lEvent.type)
            {
               case ENET_EVENT_TYPE_CONNECT:
                  mGame.addClient(lEvent.peer);
               break;
               case ENET_EVENT_TYPE_RECEIVE:
                  switch (lEvent.channelID)
                  {
                     default:
                        /* Any packets we recieve are added to the specific client that needs
                           them. They are then deleted in the thread after they have been
                           used. */
                           mGame.getClientMgr()->message(lEvent);
                     break;
                  }
               break;
               case ENET_EVENT_TYPE_DISCONNECT:
                  mGame.removeClient(lEvent.peer);
               break;
               default:
               break;
            }
         }
         enet_host_flush(mServer);
      }
   }

   void ServerMain::process()
   {
      mGame.process();
   }
} /* namespace Server */
