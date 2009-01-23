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

Server::Server(int port=26500, std::string address = std::string("127.0.0.1"))
{
    if (setupServer(port, address) == 1)
    {
        serverLoop();
    }
}

Server::~Server()
{
    enet_host_destroy(mServer);
    enet_deinitialize();
}

bool Server::setupServer(int port, std::string address)
{
    mServer=NULL;

    if (enet_initialize() != 0)
    {
        printf("Unable to initialize the network library.\n");
        return false;
    }

    mAddress.host = ENET_HOST_ANY;
    mAddress.port = port;

    printf("Starting server on port: %d\n", port);
    mServer = enet_host_create (&mAddress /* the address to bind the server host to */, 
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
    mWorldMgr = new WorldManager;
    if (mWorldMgr->loadWorldData(Ogre::String("world/default.db")))
    {
        return true;
    }
    return false;
}

void Server::serverLoop()
{
    bool serverRunning = true;
    std::cout << "Server running.." << std::endl;

    while (serverRunning)
    {
        /* Wait up to 1000 milliseconds for an event. */
        while (enet_host_service(mServer, &mEvent, 1000) > 0)
        {
            switch (mEvent.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                    printf ("A new client connected from %x:%u.\n", 
                        mEvent.peer->address.host,
                        mEvent.peer->address.port);

                    /* Store any relevant client information here. */
                    mClients.push(new Client(mEvent.peer->address.port));
                break;
                case ENET_EVENT_TYPE_RECEIVE:
                    printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
                        mEvent.packet->dataLength,
                        mEvent.packet->data,
                        mEvent.peer->data,
                        mEvent.channelID);

                    /* Clean up the packet now that we're done using it. */
                    enet_packet_destroy(mEvent.packet);
                break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    printf ("%s disconected.\n", mEvent.peer->data);

                    /* Reset the peer's client information. */
                    mEvent.peer->data = NULL;
                break;
            }
        }
    }
}
