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

#ifndef __ADMIN_H_
#define __ADMIN_H_

#include "enet/enet.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <map>

#include "srvstructs.h"

namespace Server
{
/** The Admin Channel Manager
        @remarks
            The Server::Admin class is where packets from SERVER_CHANNEL_ADMIN
            are processed. The class deals with logging in, placing buildings,
            and other activities done with admin privileges.
    */
class Admin
{
   public:
      Admin();
      ~Admin();
      /** Adds a new network message to the list of network messages to be
          processed. */
      void addMessage(const ENetEvent);
      /** Starts a new thread which processes admin network messages. */
      void makeThread(void);
      /** Stops the thread. */
      void stopThread();
   private:
      void loop(void);
      bool sendMessage(ENetPeer*,const void*, size_t, enet_uint8, enet_uint32);
      bool mRunThread;
      typedef std::multimap<enet_uint8,ENetEvent> Message;
      Message mMessages;
      boost::thread mThread;
};
}
#endif /* __ADMIN_H_ */

