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


Server::Server() {
   // Start the world manager and load up a world
   mWorldMgr = new WorldManager;
   // load up our SQLite world database
//   if (!mWorldMgr->loadWorldData("world/default.db", mSceneMgr))
  //    exit(1);
   setupServer();
}

Server::~Server() {
    enet_deinitialize();
}

void Server::setupServer() {
    std::cout << "Starting a dedicated server." << std::endl;
    
    if (enet_initialize() != 0)
    {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
    }

    /* Bind the server to the default localhost.     */
    /* A specific host address can be specified by   */
    /* enet_address_set_host (& address, "x.x.x.x"); */

    address.host = ENET_HOST_ANY;
    /* Bind the server to port 1234. */
    address.port = 7000;

    server = enet_host_create (& address /* the address to bind the server host to */, 
                                 32      /* allow up to 32 clients and/or outgoing connections */,
                                  0      /* assume any amount of incoming bandwidth */,
                                  0      /* assume any amount of outgoing bandwidth */);
    if (server == NULL) {
        std::cerr << "An error occurred while trying to create an ENet server host." << std::endl;
        exit (EXIT_FAILURE);
    }

    serverLoop();

    enet_host_destroy(server);
}

void Server::serverLoop() {
   bool serverRunning = true;
   ENetEvent event;
   
   while (serverRunning) {
    
    /* Wait up to 1000 milliseconds for an event. */
    while (enet_host_service (server, & event, 1000) > 0) {
        switch (event.type) {
           case ENET_EVENT_TYPE_CONNECT:
               printf ("A new client connected from %x:%u.\n", 
                       event.peer->address.host,
                       event.peer->address.port);

               /* Store any relevant client information here. */
               //event.peer->data = "Client information";

               break;

           case ENET_EVENT_TYPE_RECEIVE:
               printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
                       event.packet -> dataLength,
                       event.packet -> data,
                       event.peer -> data,
                       event.channelID);

               /* Clean up the packet now that we're done using it. */
               enet_packet_destroy (event.packet);
               
               break;
              
           case ENET_EVENT_TYPE_DISCONNECT:
               printf ("%s disconected.\n", event.peer -> data);

               /* Reset the peer's client information. */

               event.peer -> data = NULL;
        }
      }
   }
   std::cout << "Server Stopped" << std::endl;
}
