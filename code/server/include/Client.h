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

#ifndef __CLIENT_H_
#define __CLIENT_H_

#include "enet/enet.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "DataPacket.h"
#include "Admin.h"
#include "ClientManager.h"
#include "ThreadController.h"
#include "Building.h"

namespace Server
{
   class Admin;
   typedef std::multimap<enet_uint8,ENetEvent> Message;

   /** The Client Channel Manager
           @remarks
               The Server::Client class is where packets related to the client such
               as movement are processed. Every client that joins has their own
               Server::Client object and thread.
       */
   class Client
   {
      public:
         Client();
         ~Client();
         /** Adds a new network message to the list of network messages to be
             processed. */
         void addMessage(const ENetEvent);
         /** Starts a new thread which processes a client's messages. */
         void addThread(void);
         /** Stops and removes the thread */
         void removeThread(void);
         /** Sets the unique peer ID given to the client by Enet */
         void setPeer(ENetPeer*);
         /** Sends a packet of data to the client */
         bool send(dataPacket data, const enet_uint8 channel, const enet_uint32 priority);
         /** Sends a packet of data to the client then waits for an accepted response
            @return false if client sends rejected message
         */
         bool sendAndWait(dataPacket data, const enet_uint8 channel, const enet_uint32 priority);
         /** Add more buildings to send to the client */
         void addBuildings(Hardwar::Buildings list);
         bool isAdmin();
         Admin* getAdmin();
      private:
         void loop();

         void changeStatus(const clientStatus status);
         void processConnecting();
         void processFilecheck();
         void processDownloading();
         void processInGame();
         void processAdminReqs(dataPacket lPacket);
         Message getMessages();

         ThreadController mThreadController;

         ENetPeer* mPeer;
         clientStatus mConState;

         Message mMessages;
         mutable boost::mutex mEventMutex;

         boost::thread mThread;
         Admin *mAdmin;

         /** Buildings to send to the player */
         Hardwar::Buildings mBuildings;
   };
}
#endif /* __CLIENT_H_ */

