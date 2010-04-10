/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2008-2010  Andrew Fenn
    
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
#include <libintl.h>
#include "enet/enet.h"

#include "DataPacket.h"
#include "Building.h"
#include "Console.h"
#include "GameSettings.h"
#include "ZoneManager.h"

namespace Client
{
/** Network Manager
        @remarks
            The Client::Network class manages a thread which deals with 
            connecting and dealing with data sent from the server.
    */
typedef std::multimap<enet_uint8,ENetEvent> Message;

class Network
{
   public:
      ~Network();
      Network();

      /** Connects to a server */
      void connect(void);
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
      enet_uint32 getTimeout(void);
      /** Gets the number of times a connection has failed */
      unsigned short getRetryAttempts(void);
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
      void startThread(void);
      /** Stops the network thread */
      void stopThread(void);
      /** Gets the client's connection status */
      clientStatus getConStatus(void);
      /** Sets the client's connection status */
      void setConStatus(const clientStatus);
      /** Returns the enet host */
      ENetHost* getHost();
      /** Sets access to the ZoneManager */
      void set(ZoneManager*);
   private:
      ENetHost*         mNetHost;
      ZoneManager       *mZoneMgr;

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
#endif /* __NETWORK_H_ */

