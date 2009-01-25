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
        /* setup number of avaliable clients */
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

enet_uint8 Server::getIdFromPeer(ENetPeer* pPeer)
{
	int result = pPeer - mServer->peers;
	if ( result < 0 || result > 1000)
		return -1;
	return enet_uint8( result );
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
                {
                    printf ("A new client connected from %x\n",
                                                     mEvent.peer->address.host);

                    /* TODO: Check if server is full */
                    /* TODO: Check address isn't banned */
                    /* Store any relevant client information here. */
                    Player player;
                    strcat(player.name, "Player");
                    player.conState = STATUS_CONNECTED;

                    int id = getIdFromPeer(mEvent.peer);
                    mClients[id] = player;

                    /* TODO: Add file checking */
                    
                    /* Send client to lobby */
                    message(mEvent.peer,&player,sizeof(player),0,ENET_PACKET_FLAG_RELIABLE);
                }
                break;
                case ENET_EVENT_TYPE_RECEIVE:
                    printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
                                 mEvent.packet->dataLength, mEvent.packet->data,
                                    (char*)mEvent.peer->data, mEvent.channelID);

                    /* Clean up the packet now that we're done using it. */
                    enet_packet_destroy(mEvent.packet);
                break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    printf ("%s disconected.\n", (char*)mEvent.peer->data);

                    /* Reset the peer's client information. */
                    mEvent.peer->data = NULL;
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
