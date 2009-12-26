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
   mStatus = status_disconnected;
   GameSettings* lSettings = GameSettings::getSingletonPtr();
   mRetryLimit = Ogre::StringConverter::parseInt(lSettings->getOption("NetworkRetryLimit"));
   mTimeout    = Ogre::StringConverter::parseInt(lSettings->getOption("NetworkTimeout"));
   mConAttempts = 0;
}

bool Network::setPort(const int port)
{
   if (mStatus == status_disconnected)
   {
      mPort = port;
      return true;
   }
   return false;
}

bool Network::setAddress(const std::string address)
{
   if (mStatus == status_disconnected)
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
      setConStatus(status_connecting);
   }
}

void Network::setConStatus(const clientStatus lStatus)
{
   Console::getSingletonPtr()->addToConsole("Changing state: "+Ogre::StringConverter::toString(lStatus));
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
      mThread = boost::thread(boost::bind(&Network::threadLoopConnect, this));
   }
}

void Network::stopThread(void)
{
   if (mRunThread)
   {
      mStatus = status_disconnected;
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
         case status_connecting:
            if (sendJoinRequest())
            {
               setConStatus(status_listening);
            }
            else
            {
               if (mConAttempts >= mRetryLimit)
               {
                  setConStatus(status_disconnected);
               }
               else
               {
                  mConAttempts++;
               }
            }
         break;
         case status_disconnected:
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
   ENetEvent lEvent;
   Message lMessages = getMessages();
   for (mitEvent=lMessages.begin(); mitEvent != lMessages.end(); mitEvent++)
   {
      lEvent = (*mitEvent).second;
      dataPacket lReceivedPacket = dataPacket(lEvent.packet->data, lEvent.packet->dataLength);
      switch((*mitEvent).first)
      {
         case SERVER_CHANNEL_ADMIN:
            if (lReceivedPacket.getMessage() == admin_login)
            {
               Console* lConsole = Console::getSingletonPtr();
               packetMessage msg;
               lReceivedPacket.move(&msg, sizeof(packetMessage));
               if (msg == accepted)
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
               case status_ingame:
               case status_downloading:
                  if (lReceivedPacket.getMessage() == add_building)
                  {
                     Console::getSingletonPtr()->addToConsole("Add Building Request");
                     Hardwar::Building building;
                     building.unserialize(lReceivedPacket);

                     /* add the object */      
                     Ogre::SceneManager* lSceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr");
                     Console::getSingletonPtr()->addToConsole(Ogre::String("Pos:")+Ogre::StringConverter::toString(building.getPosition()).c_str()+Ogre::String(", Rot: ")+Ogre::StringConverter::toString(building.getRotation()).c_str()+"\n");

                     bool isAdded = false;
                     try
                     {
                        /* We create an entity with the name of the buildings position. This means two buildings can't exist
                        in the same position. */
                        Ogre::Entity *lEntity = lSceneMgr->createEntity(Ogre::String("Building/")+Ogre::StringConverter::toString(building.getPosition()), building.getMeshName());
                        Ogre::SceneNode * lSceneNode = lSceneMgr->getRootSceneNode()->createChildSceneNode();
                        lSceneNode->attachObject(lEntity);
                        lSceneNode->setPosition(building.getPosition());
                        lSceneNode->setDirection(building.getRotation());
                        isAdded = true;
                     }
                     catch(Ogre::Exception& e)
                     {
                        Console::getSingletonPtr()->addToConsole(Console::getSingletonPtr()->getConsoleError(), "addbuilding", e.getFullDescription());
                     }
                     dataPacket lPacket;
                     if (isAdded)
                     {
                        Console::getSingletonPtr()->addToConsole("Building Added");
                     }
                     else
                     {
                        Console::getSingletonPtr()->addToConsole("Building Add Error");
                     }
                  }
               default:
                  {
                     if (lReceivedPacket.getMessage() == status_changed)
                     {
                        clientStatus lStatus;
                        lReceivedPacket.move(&lStatus, sizeof(clientStatus));
                        setConStatus(lStatus);
                     }
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
}

void Network::addMessage(const ENetEvent lEvent)
{
   boost::mutex::scoped_lock lMutex(mMessageMutex);
   mMessages.insert(std::pair<enet_uint8,ENetEvent>(lEvent.channelID, lEvent));
}

Message Network::getMessages(void)
{
   boost::mutex::scoped_lock lMutex(mMessageMutex);
   Message lMessages = mMessages;
   mMessages.clear();
   return lMessages;
}

bool Network::sendJoinRequest(void)
{
   if (connect(mPort, mAddress))
   {
      dataPacket packet = dataPacket(join_game);
      message(packet, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE);
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
   mPeer = enet_host_connect(mNetHost, &address, SERVER_MAX_CHANNELS);

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

bool Network::message(dataPacket data, const enet_uint8 channel, const enet_uint32 priority)
{
   bool result = true;
   ENetPacket * packet = enet_packet_create(data.getContents(), data.size(), priority);

   if (enet_peer_send(mPeer, channel, packet) < 0)
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
