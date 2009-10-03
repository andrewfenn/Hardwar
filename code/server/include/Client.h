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

#ifndef __CLIENT_H_
#define __CLIENT_H_

#include "enet/enet.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "srvstructs.h"
#include "Admin.h"
#include "ClientManager.h"
#include "ThreadController.h"

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
      private:
         void loop(void);
         bool sendMessage(const void*, size_t, enet_uint8, enet_uint32);
         void processAdminReqs(void);
         void nextPacket(void);
         Message getMessages(void);

         ThreadController mThreadController;

         ENetPeer* mPeer;
         clientStatus mConState;
         int mtest;

         Message mMessages;
         Message::iterator mEvent;
         mutable boost::mutex mEventMutex;

         boost::thread mThread;
         Admin *mAdmin;
   };
}
#endif /* __CLIENT_H_ */

