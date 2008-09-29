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

#include "Client.h"


Client::Client() {
    std::cout << "Starting a test client connection." << std::endl;
    
    if (enet_initialize() != 0) {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
    }


    ENetHost * client;

    client = enet_host_create (NULL /* create a client host */,
                1 /* only allow 1 outgoing connection */,
                57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
                14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);

    if (client == NULL)
    {
        fprintf (stderr, 
                 "An error occurred while trying to create an ENet client host.\n");
        exit (EXIT_FAILURE);
    }


    ENetAddress address;
    ENetEvent event;
    ENetPeer *peer;

    /* Connect to some.server.net:1234. */
    enet_address_set_host (& address, "localhost");
    address.port = 7000;

    /* Initiate the connection, allocating the two channels 0 and 1. */
    peer = enet_host_connect (client, & address, 2);    
    
    if (peer == NULL) {
       fprintf (stderr, "No available peers for initiating an ENet connection.\n");
       exit (EXIT_FAILURE);
    }
    
    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service (client, & event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        puts ("Connection to localhost:7000 succeeded.");
    } else {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset (peer);

        puts ("Connection to localhost:7000 failed.");
    }


    while (true) {
          ENetEvent event;
          
          /* Wait up to 1000 milliseconds for an event. */
          while (enet_host_service (client, & event, 1000) > 0)
          {
              switch (event.type)
              {
              case ENET_EVENT_TYPE_CONNECT:
                  printf ("A new client connected from %x:%u.\n", 
                          event.peer -> address.host,
                          event.peer -> address.port);

                  /* Store any relevant client information here. */
                  //event.peer -> data = "Client information";

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
   

    enet_host_destroy(client);

    std::cout << "Test client finished" << std::endl;
}

Client::~Client() {
    enet_deinitialize();
}
