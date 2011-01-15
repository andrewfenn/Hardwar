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

#include "Client.h"

using namespace Server;

Client::Client()
{
   mConState = status_connecting;
   mAdmin = 0;
}

Client::~Client()
{
   if (mAdmin)
   {
      delete mAdmin;
      mAdmin = 0;
   }
}

void Client::addMessage(const ENetEvent lEvent)
{
   boost::mutex::scoped_lock lMutex(mEventMutex);
   mMessages.insert(std::pair<enet_uint8,ENetEvent>(lEvent.channelID, lEvent));
}

void Client::addThread(void)
{
   /* creates a new thread */
   mThread = boost::thread(boost::bind(&Client::loop, this));
}

void Client::removeThread(void)
{
   mThreadController.stop();
   mThread.join();
}

void Client::setPeer(ENetPeer* lpeer)
{
   mPeer = lpeer;
}

Message Client::getMessages(void)
{
   Message lMessages;
   boost::mutex::scoped_lock lMutex(mEventMutex);
   lMessages = mMessages;
   mMessages.clear();
   return lMessages;
}

void Client::loop(void)
{
   while(!mThreadController.hasStopped())
   {
      if (mMessages.size() > 0)
      {
         switch(mConState)
         {
            case status_connecting:
               processConnecting();
            break;
            case status_filecheck:
               processFilecheck();
            break;
            case status_downloading:
               processDownloading();
            break;
            case status_ingame:
               processInGame();
            break;
            default:
            break;
         }
      }
      else
      {
         sleep(1);
      }
   }
   std::cout << gettext("Thread: Client") << " - " << mPeer->incomingPeerID << " " << gettext("terminated") << std::endl;
}

void Client::changeStatus(const clientStatus status)
{
   mConState = status;
   dataPacket lResponsePacket = dataPacket(status_changed);
   lResponsePacket.append(&mConState, sizeof(clientStatus));
   send(lResponsePacket, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE);
}

void Client::processConnecting()
{
   Message lMessages = getMessages();
   dataPacket lReceivedPacket;
   for (Message::iterator lEventItr=lMessages.begin(); lEventItr != lMessages.end(); lEventItr++)
   {
      lReceivedPacket = dataPacket((*lEventItr).second.packet->data, (*lEventItr).second.packet->dataLength);
      if (lReceivedPacket.getMessage() == join_game)
      {
         /* New client joined. Begin by sending back the connection status */
         /* TODO: Check if server is full */
         /* TODO: Check address isn't banned */
         changeStatus(status_filecheck);
      }
      /* destory the packet */
      enet_packet_destroy((*lEventItr).second.packet);
   }
}

void Client::processFilecheck()
{
   Message lMessages = getMessages();
   dataPacket lReceivedPacket;
   for (Message::iterator lEventItr=lMessages.begin(); lEventItr != lMessages.end(); lEventItr++)
   {
      lReceivedPacket = dataPacket((*lEventItr).second.packet->data, (*lEventItr).second.packet->dataLength);
      if (lReceivedPacket.getMessage() == accepted)
      {
         changeStatus(status_downloading);
      }
      /* destory the packet */
      enet_packet_destroy((*lEventItr).second.packet);
   }
}

void Client::processDownloading()
{
   Message lMessages = getMessages();
   dataPacket lReceivedPacket;
   dataPacket lResponsePacket;
   bool requested = false;

   for (Message::iterator lEventItr=lMessages.begin(); lEventItr != lMessages.end(); lEventItr++)
   {
      lReceivedPacket = dataPacket((*lEventItr).second.packet->data, (*lEventItr).second.packet->dataLength);
      if (lReceivedPacket.getMessage() == accepted)
      {
         changeStatus(status_ingame);
      }
      /* destory the packet */
      enet_packet_destroy((*lEventItr).second.packet);
   }
}

void Client::processInGame()
{
   Message lMessages = getMessages();
   dataPacket lReceivedPacket;
   dataPacket lResponsePacket;

   for (Message::iterator lEventItr=lMessages.begin(); lEventItr != lMessages.end(); lEventItr++)
   {
      lReceivedPacket = dataPacket((*lEventItr).second.packet->data, (*lEventItr).second.packet->dataLength);

      processAdminReqs(lReceivedPacket);
      /* destory the packet */
      enet_packet_destroy((*lEventItr).second.packet);
   }
}

void Client::processAdminReqs(dataPacket lPacket)
{
   if (mAdmin != 0)
   {
      /* is logged in */
      mAdmin->processRequest(lPacket);
   }
   else
   {
      if (lPacket.getMessage() == admin_login)
      {
         Ogre::String hash;
         lPacket.moveString(hash, lPacket.size());
         std::cout << hash << std::endl;
         /* FIXME: There is no password */
         packetMessage response = accepted;

         dataPacket lPacket = dataPacket(admin_login);
         lPacket.append(&response, sizeof(packetMessage));
         send(lPacket, SERVER_CHANNEL_ADMIN, ENET_PACKET_FLAG_RELIABLE);
         if (!mAdmin)
         {
            mAdmin = new Admin();
         }
      }
   }
}

bool Client::isAdmin()
{
   if (!mAdmin)
   {
      return false;
   }
   return true;
}

Admin* Client::getAdmin()
{
   return mAdmin;
}

bool Client::send(dataPacket data, const enet_uint8 channel, const enet_uint32 priority)
{
   bool result = true;
   ENetPacket * packet = enet_packet_create(data.getContents(), data.size(), priority);
   if (enet_peer_send(mPeer, channel, packet) < 0)
   {
      result = false;
   }
   return result;
}
