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

Client::Client()
{
    Client(26500, std::string("127.0.0.1"));
}

Client::Client(int port=26500, std::string address = std::string("127.0.0.1"))
{
    if (this->connect(port, address) == 1)
    {
        clientLoop();
    }
}

int Client::connect(int port, std::string address)
{
    if (enet_initialize() != 0) {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
        return 0;
    }

    mNetHost = enet_host_create (NULL /* create a client host */,
                1 /* only allow 1 outgoing connection */,
                57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
                14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);

    if (mNetHost == NULL)
    {
        fprintf (stderr, 
                 "An error occurred while trying to create an ENet client host.\n");
        enet_deinitialize();
        return 0;
    }

    std::cout << "Connecting to " << address << ":" << port << std::endl;

    if (enet_address_set_host(&mAddress, address.c_str()) < 0)
    {
        printf("enet_address_set_host() failed.\n");
        return 0;
    }
    mAddress.port = port;

    /* Initiate the connection, allocating the two channels 0 and 1. */
    mPeer = enet_host_connect (mNetHost, &mAddress, 2);

    if (!mPeer) {
       fprintf (stderr, "No available peers for initiating an ENet connection.\n");
       enet_deinitialize();
       return 0;
    }
    
    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service (mNetHost, & mEvent, 5000) > 0 && mEvent.type == ENET_EVENT_TYPE_CONNECT)
    {
        std::cout << "Connection to " << address << ":" << port << " succeeded" << std::endl;
        return 1;
    }
    else
    {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset (mPeer);
        enet_deinitialize();
        std::cout << "Connection to " << address << ":" << port << " failed" << std::endl;
    }

    return 0;
}

void Client::clientLoop()
{
    while (true)
    {
        /* Wait up to 1000 milliseconds for an event. */
        while (enet_host_service(mNetHost, &mEvent, 1000) > 0)
        {
            switch (mEvent.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                    printf ("A new client connected from %x:%u.\n", 
                        mEvent.peer->address.host,
                        mEvent.peer->address.port);

                    /* Store any relevant client information here. */
                    //mEvent.peer->data = "Client information";
                break;
                case ENET_EVENT_TYPE_RECEIVE:
                    printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
                        mEvent.packet->dataLength,
                        mEvent.packet->data,
                        mEvent.peer->data,
                        mEvent.channelID);

                    /* Clean up the packet now that we're done using it. */
                    enet_packet_destroy (mEvent.packet);
                break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    printf ("%s disconected.\n", mEvent.peer->data);
                    /* Reset the peer's client information. */
                    mEvent.peer->data = NULL;
                break;
            }
        }
    }

    enet_host_destroy(mNetHost);
}

Client::~Client() {
    enet_deinitialize();
}
