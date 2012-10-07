/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2008-2012  Andrew Fenn
    
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

#pragma once

#include <boost/thread.hpp>
#include "enet/enet.h"
#include <string>

#include "DataPacket.h"
#include "GameTask.h"

namespace Client
{
/** Network Manager
        @remarks
            The Client::Network class manages a thread which deals with 
            connecting and dealing with data sent from the server.
    */
typedef std::multimap<enet_uint8,ENetEvent> Message;

class NetworkTask : public GameTask
{
   public:
      ~NetworkTask();
      NetworkTask();

      void init() { }
      void shutdown() {}
      void update() {}
      void changeSize(Ogre::RenderWindow*) {}
      void changeFocus(Ogre::RenderWindow*) {}

      /** Connects to a server */
      void connect();
      /** Looks for new UDP messages being recieved from the server */
      bool pollMessage(ENetEvent*);
      /** Sets the IP address of the server to connect to
         @param address
                The IP address of the server
      */
      bool setAddress(const std::string);
      /** Sets the port address of the server to connect to
         @param address
                The port address of the server
      */
      bool setPort(const int);
      enet_uint32 getTimeout();
      /** Gets the number of times a connection has failed */
      unsigned short getRetryAttempts();
      /** Sends a message to the server
         @param data
                The data being sent
         @param channel
                An Enet specific variable. Which channel the data is being sent 
                on.
         @param priority
                An Enet specific variable.
      */
      bool message(dataPacket data, const enet_uint8, const enet_uint32);
      /** Starts the network thread */
      void startThread();
      /** Stops the network thread */
      void stopThread();
      /** Gets the client's connection status */
      clientStatus getConStatus();
      /** Sets the client's connection status */
      void setConStatus(const clientStatus);
      /** Returns the enet host */
      ENetHost* getHost();
   private:
      ENetHost*         mNetHost;

      ENetEvent         mEvent;
      ENetPeer*         mPeer;
      int               mPort;
      std::string       mAddress;
      clientStatus      mStatus;
      unsigned short    mConAttempts;
      unsigned short    mRetryLimit;
      unsigned short    mTimeout;

      Message::iterator mitEvent;
      Message mMessages;
      mutable boost::mutex mMessageMutex;
      void addMessage(const ENetEvent);
      Message getMessages(void);

      bool connect(unsigned int, std::string);
      bool sendJoinRequest(void);
      void threadLoopConnect(void);
      void threadLoopMessages(void);
      void threadLoopGame(void);

      boost::thread mThread;
      bool mRunThread;
};

}

