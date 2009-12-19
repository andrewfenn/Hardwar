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
   mConState = status_connecting;
   mAdmin = 0;
}

Client::~Client()
{
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
   mtest = mPeer->incomingPeerID;
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
   Message lMessages;
   dataPacket lReceivedPacket;
   dataPacket lResponsePacket;
   bool lStartedDownload = false;
   Building::iterator lBuildIter;

   while(!mThreadController.hasStopped())
   {
      if (mMessages.size() > 0)
      {
         lMessages = getMessages();

         for (mEvent=lMessages.begin(); mEvent != lMessages.end(); mEvent++)
         {
            lReceivedPacket = dataPacket((*mEvent).second.packet->data, (*mEvent).second.packet->dataLength);
            switch((*mEvent).first)
            {
               case SERVER_CHANNEL_MOVEMENT:
                  /* This channel is for movement */
               break;
               default:
                  /* Since we're mostly getting movement updates we construct our
                     switch this way for speed. */
                  switch((*mEvent).first)
                  {
                     case SERVER_CHANNEL_ADMIN:
                        processAdminReqs(lReceivedPacket);
                     break;
                     case SERVER_CHANNEL_GENERIC:
                        /* This channel of join requests and pings */
                        switch(mConState)
                        {
                           case status_downloading:
                 /*             if (lReceivedPacket.getMessage() == accepted)
                              {
                                 if (!lStartedDownload)
                                 {
                                    lBuildIter = LevelManager::getSingletonPtr()->getBuildings();
                                    lStartedDownload = true;
                                 }
                                 
                                 if (LevelManager::getSingletonPtr()->end(lBuildIter))
                                 {
                                    mConState = status_ingame;
                                    lResponsePacket = dataPacket(status_changed);
                                    lResponsePacket.append(&mConState, sizeof(clientStatus));
                                    send(lResponsePacket, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE);
                                 }
                                 else
                                 {
                                    LevelManager::getSingletonPtr()->sendBuildingData((unsigned int)0,(*lBuildIter).second, mPeer);
                                    lBuildIter++;
                                 }
                              }*/
                           break;
                           case status_filecheck:
                              /* TODO: Add file checking */
                              if (lReceivedPacket.getMessage() == accepted)
                              {
                                 mConState = status_downloading;
                                 lResponsePacket = dataPacket(status_changed);
                                 lResponsePacket.append(&mConState, sizeof(clientStatus));
                                 send(lResponsePacket, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE);
                              }
                           break;
                           default:
                              if (lReceivedPacket.getMessage() == join_game)
                              {
                                 /* New client joined. Begin by sending back the connection status */
                                 /* TODO: Check if server is full */
                                 /* TODO: Check address isn't banned */
                                 mConState = status_filecheck;
                                 lResponsePacket = dataPacket(status_changed);
                                 lResponsePacket.append(&mConState, sizeof(clientStatus));
                                 send(lResponsePacket, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE);                        
                              }
                           break;
                        }
                     break;
                  } /* end second switch */
               break;
            } /* end first switch */            
            /* finished with the packet, destory it */
            enet_packet_destroy((*mEvent).second.packet);
         } /* end for packet loop */
      }
      else
      {
         sleep(1);
      }
   }
   printf(gettext("Thread: Client - %d - terminated\n"), mPeer->incomingPeerID);
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
         mAdmin = new Admin();
      }
   }
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
