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

#include "Network.h"

using namespace Client;

Network* Network::mNetwork;

Network::Network()
{
   if (enet_initialize() != 0) {
      fprintf(stderr, gettext("An error occurred while initializing ENet\n"));
   }
   mPort = 26500;
   mAddress = std::string("127.0.0.1");
   mRunThread = false;
   mStatus = STATUS_DISCONNECTED;
   GameSettings* lSettings = GameSettings::getSingletonPtr();
   mRetryLimit = Ogre::StringConverter::parseInt(lSettings->getOption("NetworkRetryLimit"));
   mTimeout    = Ogre::StringConverter::parseInt(lSettings->getOption("NetworkTimeout"));
   mConAttempts = 0;
}

bool Network::setPort(int port)
{
   if (mStatus == STATUS_DISCONNECTED)
   {
      mPort = port;
      return true;
   }
   return false;
}

bool Network::setAddress(std::string address)
{
   if (mStatus == STATUS_DISCONNECTED)
   {
      mAddress = address;
      return true;
   }
   return false;
}

void Network::connect(void)
{
   if (mPort > 0 && !mAddress.empty())
   {
      setConStatus(STATUS_CONNECTING);
   }
}

void Network::setConStatus(clientStatus lStatus)
{
   /* TODO: Add mutex */
   mStatus = lStatus;
}

unsigned short Network::getRetryAttempts(void)
{
   return mConAttempts;
}

clientStatus Network::getConStatus(void)
{
   /* TODO: Add mutex */
   return mStatus;
}

void Network::startThread(void)
{
   if (!mRunThread)
   {
      /* create a new thread */
      boost::thread mThread(boost::bind(&Network::threadLoopConnect, this));
   }
}

void Network::stopThread(void)
{
   if (mRunThread)
   {
      mStatus = STATUS_DISCONNECTED;
      mRunThread = false;
      mThread.join();
   }
}

enet_uint32 Network::getTimeout(void)
{
   return mPeer->nextTimeout;
}

void Network::threadLoopConnect(void)
{
   mRunThread = true;
   ENetEvent lEvent;
   clientStatus lStatus;

   while (mRunThread)
   {
      switch(mStatus)
      {
         case STATUS_CONNECTING:
            if (sendJoinRequest())
            {
               setConStatus(STATUS_LISTENING);
            }
            else
            {
               mConAttempts++;
               if (mConAttempts > mRetryLimit)
               {
                  setConStatus(STATUS_DISCONNECTED);
               }
            }
         break;
         case STATUS_LISTENING:
         {
            if (pollMessage(&lEvent))
            {
               switch (lEvent.type)
               {
                  case ENET_EVENT_TYPE_RECEIVE:
                     memcpy(&lStatus, lEvent.packet->data, sizeof(lEvent.packet->data));
                     setConStatus(lStatus);
                     /* Clean up the packet now that we are done using it */
                     enet_packet_destroy(lEvent.packet);
                  break;
                  default:
                  break;
               }
            }
         }
         break;
         case STATUS_CONNECTED:
            threadLoopMessages();
            threadLoopGame();
         break;
         default:
         break;
      }
   }

   enet_host_destroy(mNetHost);
   enet_deinitialize();
}

void Network::threadLoopMessages()
{
   Ogre::UTFString lResponse;
   ENetEvent lEvent;
   while (enet_host_service(mNetHost, &lEvent, 1000) > 0)
   {
      switch(lEvent.type)
      {
         case ENET_EVENT_TYPE_RECEIVE:
            addMessage(lEvent);
         break;
         default:
         break;
      }
   }
}

void Network::threadLoopGame()
{
   Message::iterator itEvent;
   for (itEvent=mMessages.begin(); itEvent != mMessages.end(); itEvent++)
   {
      switch((*itEvent).first)
      {
         case SERVER_CHANNEL_ADMIN:
            if (Ogre::UTFString((char*)(*itEvent).second.packet->data) == "login")
            {
               enet_packet_destroy((*itEvent).second.packet);
               itEvent++;
               Console* lConsole = Console::getSingletonPtr();
               if (atoi((char*) (*itEvent).second.packet->data))
               {
                  /* Login Successful */               
                  lConsole->addToConsole(lConsole->getConsoleSuccess(), Ogre::UTFString("rcon_password"), Ogre::UTFString(gettext("Logged in as admin")));
                  GameSettings::getSingletonPtr()->setOption("isAdmin", Ogre::UTFString("1"));
               }
               else
               {
                  /* Failed to login correctly */
                  lConsole->addToConsole(lConsole->getConsoleError(), Ogre::UTFString("rcon_password"), Ogre::UTFString(gettext("Login failed")));
               }
            }
         break;
         default:
         break;
      }
      enet_packet_destroy((*itEvent).second.packet);
   }
   mMessages.clear();
}

void Network::addMessage(const ENetEvent lEvent)
{
   mMessages.insert(std::pair<enet_uint8,ENetEvent>(lEvent.channelID, lEvent));
}

bool Network::sendJoinRequest(void)
{
   if (connect(mPort, mAddress))
   {
      message("join", strlen("join")+1, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE);
      return true;
   }
   return false;
}

bool Network::connect(unsigned int port, std::string ip)
{
   /* TODO: Add all print out messages to a log */
   ENetAddress address;

   /* TODO: Add config option to increase speed */
   mNetHost = enet_host_create (NULL /* create a Network host */,
                                       1 /* only allow 1 outgoing connection */,
                    57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
                     14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);

   if (mNetHost == NULL)
   {
      fprintf (stderr, gettext("An error occurred while trying to create an ENet Network host.\n"));
      return false;
   }

   printf(gettext("Connecting to %s:%d\n"), ip.c_str(), port);

   if (enet_address_set_host(&address, ip.c_str()) < 0)
   {
      printf("enet_address_set_host() failed.\n");
      return false;
   }
   address.port = port;

   /* Initiate the connection */
   mPeer = enet_host_connect (mNetHost, &address, SERVER_MAX_CHANNELS);

   if (!mPeer)
   {
      fprintf(stderr, gettext("No available peers for initiating an ENet connection.\n"));
      return false;
   }

   /* Wait up to n seconds for the connection attempt to succeed. */
   if (enet_host_service (mNetHost, & mEvent, mTimeout*1000) > 0 && mEvent.type == ENET_EVENT_TYPE_CONNECT)
   {
      printf(gettext("Connection succeeded\n"));
      return true;
   }
   else
   {
      /* Either the n seconds are up or a disconnect event was
       * received. Reset the peer in the event the n seconds
       * had run out without any significant event.
       */
      enet_peer_reset(mPeer);
      printf(gettext("Connection failed\n"));
   }

   return false;
}

bool Network::pollMessage(ENetEvent *pEvent)
{
   if (enet_host_service(mNetHost, pEvent, 0) > 0)
   {
      return true;
   }
   return false;
}

bool Network::message(const void* msg, size_t size, enet_uint8 channel, enet_uint32 priority)
{
   bool result = true;
   ENetPacket * packet = enet_packet_create (msg, size, priority);

   if (enet_peer_send(mPeer, channel, packet) <0)
   {
      result = false;
   }

   enet_host_flush(mNetHost);
   return result;
}

Network::~Network()
{
   stopThread();
}

Network* Network::getSingletonPtr(void)
{
   if(!mNetwork)
   {
      mNetwork = new Network();
   }

   return mNetwork;
}
