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

#ifndef __NETWORK_H_
#define __NETWORK_H_

#include <string>
#include <stdio.h>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "enet/enet.h"

#include "srvstructs.h"
#include "Console.h"
#include "GameSettings.h"

namespace Client
{
/** Network Manager
        @remarks
            The Client::Network class manages a thread which deals with 
            connecting and dealing with data sent from the server.
    */
class Network
{
   public:
      ~Network();
      static Network* getSingletonPtr(void);

      /** Connects to a server */
      void connect(void);
      /** Looks for new UDP messages being recieved from the server */
      bool pollMessage(ENetEvent*);
      /** Sets the IP address of the server to connect to
         @param address
                The IP address of the server
      */
      bool setAddress(std::string);
      /** Sets the port address of the server to connect to
         @param address
                The port address of the server
      */
      bool setPort(int);
      enet_uint32 getTimeout(void);
      /** Gets the number of times a connection has failed */
      unsigned short getRetryAttempts(void);
      /** Sends a message to the server
         @param data
                The data being sent
         @param size
                The size of the data
         @param channel
                An Enet specific variable. Which channel the data is being sent 
                on.
         @param priority
                An Enet specific variable.
      */
      bool message(const void*, size_t, enet_uint8, enet_uint32);
      /** Starts the network thread */
      void startThread(void);
      /** Stops the network thread */
      void stopThread(void);
      /** Gets the client's connection status */
      clientStatus getConStatus(void);

      ENetHost*         mNetHost;

   private:
      ENetEvent         mEvent;
      ENetPeer*         mPeer;
      int               mPort;
      bool              mConnected;
      std::string       mAddress;
      clientStatus      mStatus;
      unsigned short    mConAttempts;
      unsigned short    mRetryLimit;
      unsigned short    mTimeout;

      static Network *mNetwork;
      typedef std::multimap<enet_uint8,ENetEvent> Message;
      Message mMessages;
      void addMessage(const ENetEvent);

      Network(void);
      Network(const Network&) { }
      Network & operator = (const Network&);

      bool connect(unsigned int, std::string);
      bool sendJoinRequest(void);
      void threadLoopConnect(void);
      void threadLoopMessages(void);
      void threadLoopGame(void);
      void setConStatus(clientStatus);

      boost::thread mThread;
      bool mRunThread;
};

}
#endif /* __NETWORK_H_ */

