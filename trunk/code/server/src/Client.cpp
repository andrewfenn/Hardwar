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

using namespace Server;

Client::Client()
{
   mRunClient = true;
   mConState = STATUS_CONNECTING;
}

Client::~Client()
{
   mRunClient = false;
}

void Client::addMessage(const ENetEvent lEvent)
{
   mMessages.insert(std::pair<enet_uint8,ENetPacket*>(lEvent.channelID, lEvent.packet));
   printf ("\nline 38 --%u - %s - client:%d - channel %u.\n",
                                       (intptr_t) lEvent.packet->dataLength,
                                                (char*) lEvent.packet->data,
                                                          mPeer->incomingPeerID,
                                                              lEvent.channelID);
}

void Client::makeThread(void)
{
   /* creates a new thread */
   boost::thread mThread(boost::bind(&Client::loop, this));
   return;
}

void Client::setPeer(ENetPeer* lpeer)
{
   mPeer = lpeer;
}

void Client::loop(void)
{
   /* New client joined. Begin by sending back the connection status */
   /* TODO: Check if server is full */
   /* TODO: Check address isn't banned */
   /* TODO: Add file checking */
   mConState = STATUS_CONNECTED;
   sendMessage(&mConState, sizeof(mConState), 0, ENET_PACKET_FLAG_RELIABLE);

   Message::iterator itEvent;
   Message lMessages;
   while(mRunClient)
   {
      if (mMessages.size() > 0)
      {
         /* TODO: mutex lock before copying and deleting */
         lMessages = mMessages;
         mMessages.clear();

         for (itEvent=lMessages.begin(); itEvent != lMessages.end(); itEvent++ )
         {
            printf ("line 78 --%u - %s - client:%d - channel %u.\n",
                                         (intptr_t) (*itEvent).second->dataLength,
                                                  (char*) (*itEvent).second->data,
                                                            mPeer->incomingPeerID,
                                                            (*itEvent).first);
            switch((*itEvent).first)
            {
               case 0: /* This channel of join requests and pings */
               {
                  char* data = (char*)(*itEvent).second->data;
                  if (strcmp(data, "ping") == 0)
                  {
                     printf("replied\n");
                     sendMessage("pong", strlen("pong")+1, 0, ENET_PACKET_FLAG_UNSEQUENCED);
                  }
               }
               break;
               case 1:
               {
                  /* This channel is for movement */

               }
               break;
            }
         }
      }
   }
}

bool Client::sendMessage(const void* msg, size_t size,
                                       enet_uint8 channel, enet_uint32 priority)
{
   bool result = true;
   ENetPacket * packet = enet_packet_create(msg, size, priority);
   if (enet_peer_send(mPeer, channel, packet) < 0)
   {
      result = false;
   }
   return result;
}
