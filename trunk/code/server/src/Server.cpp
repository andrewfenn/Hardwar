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
   if (setupServer(Ogre::StringConverter::parseInt(mConfig.getSetting("default port", "Network Settings")),
                  mConfig.getSetting("local address", "Network Settings")) == 1)
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

bool ServerMain::setupServer(int port, Ogre::String ip)
{
   ENetAddress address;
   mServer=NULL;
   mPlayerCount = 0;

   if (enet_initialize() != 0)
   {
      printf("Unable to initialize the network library.\n");
      return false;
   }

   address.host = ENET_HOST_ANY;
   address.port = port;

   printf("Starting server\n");
   printf("Port: %d\n", port);

   mServer = enet_host_create (&address /* the address to bind the server host to */, 
                              32      /* allow up to 32 clients and/or outgoing connections */,
                               0      /* assume any amount of incoming bandwidth */,
                               0      /* assume any amount of outgoing bandwidth */);
   if (mServer == NULL)
   {
      printf("An error occurred while trying to create an ENet server host.\n");
      enet_deinitialize();
      return false;
   }

   setupGame();

   return true;
}

bool ServerMain::setupGame()
{
   bool result = true;

  /* mWorldMgr = new WorldManager;
   result = mWorldMgr->loadWorldData(Ogre::String("world/default.db"));

   if (result == true)
   {
      mFileMgr = new FileManager;
      mFileMgr->scanFiles();
   }
*/
   return result;
}

void ServerMain::createClient(ENetPeer *lpeer)
{
   printf ("A new client connected with the peer ID: %d\n", lpeer->incomingPeerID);
   mPlayer[lpeer->incomingPeerID] = new Client();
   mPlayer[lpeer->incomingPeerID]->setPeer(lpeer);
   /* start new client thread */
   mPlayer[lpeer->incomingPeerID]->makeThread();
}

void ServerMain::serverLoop()
{
   bool serverRunning = true;
   printf("Server running..\n");
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
               createClient(lEvent.peer);
            break;
            case ENET_EVENT_TYPE_RECEIVE:
               mPlayer[lEvent.peer->incomingPeerID]->addMessage(lEvent);
               enet_packet_destroy(lEvent.packet);
            break;
            case ENET_EVENT_TYPE_DISCONNECT:
            {
               printf ("client: %s disconected.\n", (char*)lEvent.peer->incomingPeerID);
               /* TODO: destory client's thread */
               delete mPlayer[lEvent.peer->incomingPeerID];
               mPlayer.erase(lEvent.peer->incomingPeerID);
            }
            break;
            default:
            break;
         }
      }
   }
}

bool ServerMain::message(ENetPeer *peer,const void* msg, size_t size, 
                                       enet_uint8 channel, enet_uint32 priority)
{
   bool result = true;
   ENetPacket * packet = enet_packet_create (msg, size, priority);

   if (enet_peer_send(peer, channel, packet) <0)
   {
      result = false;
   }

   enet_host_flush(mServer);
   return result;
}
