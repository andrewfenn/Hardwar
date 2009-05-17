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
#include "srvstructs.h"
#include "enet/enet.h"

namespace Client
{

class Network
{
   public:
      Network();
      ~Network();

      void connect(void);
      bool pollMessage(ENetEvent*);

      bool setAddress(std::string);
      bool setPort(int);

      enet_uint32 getTimeout(void);
      unsigned short getRetryAttempts(void);

      bool message(const void*, size_t, enet_uint8, enet_uint32);

      void startThread(void);
      void stopThread(void);

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

      bool connect(unsigned int, std::string);
      bool sendJoinRequest(void);
      void threadLoop(void);
      void setConStatus(clientStatus);

      boost::thread mThread;
      bool mRunThread;
};

}
#endif /* __NETWORK_H_ */

