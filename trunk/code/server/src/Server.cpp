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

void ServerMain::serverLoop()
{
   bool serverRunning = true;
   std::cout << "Server running.." << std::endl;
   ENetEvent event;
   Player lNewClient;
   lNewClient.conState = STATUS_CONNECTING;

   while (serverRunning)
   {
      /* Wait up to 1000 milliseconds for an event. */
      while (enet_host_service(mServer, &event, 1000) > 0)
      {
         switch (event.type)
         {
            case ENET_EVENT_TYPE_CONNECT:
            {
               char address[20];
               enet_address_get_host_ip(&event.peer->address, address, sizeof(address));
               printf ("A new client connected from %s\n", address);
               mPlayer[event.peer->incomingPeerID] = lNewClient;
               /* TODO: start new client thread */
            }
            break;
            case ENET_EVENT_TYPE_RECEIVE:
               mPlayer[event.peer->incomingPeerID].events.push_back(event);
              /* Clean up the packet now that we're done using it. */
              enet_packet_destroy(event.packet);
            break;
            case ENET_EVENT_TYPE_DISCONNECT:
            {
               printf ("client: %s disconected.\n", (char*)event.peer->data);
               /* TODO: destory client's thread */
               
               /* remove player */
               mPlayer.erase(event.peer->incomingPeerID); 
               /* Reset the peer's client information. */
               event.peer->data = NULL;
            }
            break;
            default:
            break;
         }
      }
   }
}

void ServerMain::clientLoop()
{
   /* TODO: Get the client data, put this in a while loop and thread it up */
   ENetEvent event;
   
   switch(event.channelID)
   {
      case 0: /* This channel of join requests and pings */
      {
         char* data = (char*)event.packet->data;
         if (strcmp(data, "join") == 0)
         {
            /* TODO: Check if server is full */
            /* TODO: Check address isn't banned */
            /* Store any relevant client information here. */


            /* TODO: Add file checking */
            mPlayer[event.peer->incomingPeerID].conState = STATUS_CONNECTED;
            /* Send client to lobby */
            message(event.peer,&mPlayer[event.peer->incomingPeerID],sizeof(mPlayer[event.peer->incomingPeerID]),0,ENET_PACKET_FLAG_RELIABLE);
         }
         else if (strcmp(data, "ping") == 0)
         {
            message(event.peer,"pong",strlen("pong")+1,0, ENET_PACKET_FLAG_UNSEQUENCED);
         }
      }
      break;
      case 1:
      {
         /* This channel is for movement */
         printf ("A packet of length %u containing %s was received from client:%d on channel %u.\n",
                                (intptr_t) event.packet->dataLength,
                               event.packet->data, event.peer->data,
                                                   event.channelID);
      }
      break;
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
