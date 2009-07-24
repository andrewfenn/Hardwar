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
   mConAttempts = 1;
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
   mStatus = lStatus;
}

unsigned short Network::getRetryAttempts(void)
{
   return mConAttempts;
}

clientStatus Network::getConStatus(void)
{
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
         case STATUS_DISCONNECTED:
         break;
         default:
            threadLoopMessages();
            threadLoopGame();
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
   for (mitEvent=mMessages.begin(); mitEvent != mMessages.end(); mitEvent++)
   {
      switch((*mitEvent).first)
      {
         case SERVER_CHANNEL_ADMIN:
            if (Ogre::UTFString((char*)(*mitEvent).second.packet->data) == "login")
            {
               nextPacket();
               Console* lConsole = Console::getSingletonPtr();
               if (atoi((char*) (*mitEvent).second.packet->data))
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
         case SERVER_CHANNEL_GENERIC:
            switch (mStatus)
            {
               case STATUS_INGAME:
               case STATUS_DOWNLOADING:
                  if (Ogre::String((char*)(*mitEvent).second.packet->data) == "addbuilding")
                  {
                     Ogre::Vector3 position;
                     Ogre::Vector3 rotation;
                     Ogre::String mesh;

                     /* position */
                     nextPacket();
                     position.x = Ogre::StringConverter::parseInt((char*)(*mitEvent).second.packet->data);
                     nextPacket();
                     position.y = Ogre::StringConverter::parseInt((char*)(*mitEvent).second.packet->data);
                     nextPacket();
                     position.z = Ogre::StringConverter::parseInt((char*)(*mitEvent).second.packet->data);

                     /* rotation */
                     nextPacket();
                     rotation.x = Ogre::StringConverter::parseInt((char*)(*mitEvent).second.packet->data);
                     nextPacket();
                     rotation.y = Ogre::StringConverter::parseInt((char*)(*mitEvent).second.packet->data);
                     nextPacket();
                     rotation.z = Ogre::StringConverter::parseInt((char*)(*mitEvent).second.packet->data);

                     /* get the mesh name */
                     nextPacket();
                     mesh = Ogre::String((char*)(*mitEvent).second.packet->data);

                     /* add the object */      
                     Ogre::SceneManager* lSceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr");
                     printf("Pos: %s, Rot: %s, Mesh: %s\n", Ogre::StringConverter::toString(position).c_str(), Ogre::StringConverter::toString(rotation).c_str(),mesh.c_str());

                     try
                     {
                        Ogre::Entity *lEntity = lSceneMgr->createEntity(Ogre::StringConverter::toString(position), mesh);
                        Ogre::SceneNode * lSceneNode = lSceneMgr->getRootSceneNode()->createChildSceneNode();
                        lSceneNode->attachObject(lEntity);
                        lSceneNode->setPosition(position);
                        lSceneNode->setDirection(rotation);
                     }
                     catch(Ogre::Exception& e)
                     {
                        Console::getSingletonPtr()->addToConsole(Console::getSingletonPtr()->getConsoleError(), "addbuilding", e.getFullDescription());
                     }

                     message("ok", strlen("ok")+1, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE);
                  }
               default:
                  if (Ogre::String((char*)(*mitEvent).second.packet->data) == "constatus")
                  {
                     clientStatus lStatus;
                     nextPacket();
                     memcpy(&lStatus, (*mitEvent).second.packet->data, sizeof((*mitEvent).second.packet->data));
                     setConStatus(lStatus);
                  }
               break;
         }
         break;
         default:
            Console* lConsole = Console::getSingletonPtr();
            lConsole->addToConsole(Ogre::String((char*) (*mitEvent).second.packet->data));
         break;
      }
      enet_packet_destroy((*mitEvent).second.packet);
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
   ENetAddress address;

   /* TODO: Add config option to increase speed, set the 0 (unlimited) at the moment */
   mNetHost = enet_host_create (0 /* create a Network host */,
                                       1 /* only allow 1 outgoing connection */,
                    57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
                    14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);
   if (mNetHost == 0)
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

   if (mPeer == 0)
   {
      fprintf(stderr, gettext("No available peers for initiating an ENet connection.\n"));
      return false;
   }

   /* Wait up to n seconds for the connection attempt to succeed. */
   if (enet_host_service (mNetHost, & mEvent, mTimeout*1000) > 0 && mEvent.type == ENET_EVENT_TYPE_CONNECT)
   {
      printf(gettext("Connection succeeded.\n"));
      return true;
   }
   else
   {
      /* Either the n seconds are up or a disconnect event was
       * received. Reset the peer in the event the n seconds
       * had run out without any significant event.
       */
      enet_peer_reset(mPeer);
      printf(gettext("Connection failed.\n"));
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
   ENetPacket * packet = enet_packet_create(msg, size, priority);

   if (enet_peer_send(mPeer, channel, packet) < 0)
   {
      result = false;
   }

   enet_host_flush(mNetHost);
   return result;
}

void Network::nextPacket(void)
{
   enet_packet_destroy((*mitEvent).second.packet);
   /* FIXME: This could screw up if the packet hasn't arrived yet */
   mitEvent++;
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
