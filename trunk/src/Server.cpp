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


Server::Server()
{
   Server(26500, std::string("127.0.0.1"));
}

Server::Server(int port=26500, std::string ip = std::string("127.0.0.1"))
{
   if (setupServer(port, ip) == 1)
   {
      /* setup number of avaliable clients */
      serverLoop();
   }
}

Server::~Server()
{
   enet_host_destroy(mServer);
   enet_deinitialize();
}

bool Server::setupServer(int port, std::string ip)
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

   printf("Starting server on port: %d\n", port);
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

bool Server::setupGame()
{
   bool result = true;

   mWorldMgr = new WorldManager;
   result = mWorldMgr->loadWorldData(Ogre::String("world/default.db"));

   if (result == true)
   {
      mFileMgr = new FileManager;
      mFileMgr->scanFiles();
   }

   return result;
}

void Server::serverLoop()
{
   bool serverRunning = true;
   std::cout << "Server running.." << std::endl;
   ENetEvent event;

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

            Player player;
            strcpy(player.name, "Player");
            player.conState = STATUS_CONNECTING;

            event.peer->data = (void*)mPlayerCount;
            mClients[mPlayerCount] = player;
            mPlayerCount++;
            }
            break;
            case ENET_EVENT_TYPE_RECEIVE:
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
                        unsigned int id = (intptr_t) event.peer->data;
                        mClients[id].conState = STATUS_CONNECTED;
                        /* Send client to lobby */
                        message(event.peer,&mClients[id],sizeof(mClients[id]),0,ENET_PACKET_FLAG_RELIABLE);
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
              /* Clean up the packet now that we're done using it. */
              enet_packet_destroy(event.packet);
            break;
            case ENET_EVENT_TYPE_DISCONNECT:
               printf ("client:%d disconected.\n", (char*)event.peer->data);

               /* Reset the peer's client information. */
               event.peer->data = NULL;
            break;
            default:
            break;
         }
      }
   }
}

bool Server::message(ENetPeer *peer,const void* msg, size_t size, 
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
