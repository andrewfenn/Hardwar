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

#include "WorldManager.h"
#include "enet/enet.h"

namespace Client
{

class Network
{
   public:
      Network();
      ~Network();
      bool sendJoinRequest(void);
      bool pollMessage(ENetEvent*);
      bool message(const void*, size_t, enet_uint8, enet_uint32);
      bool setPort(int);
      bool setAddress(std::string);

      ENetHost*         mNetHost;

   private:
      ENetEvent         mEvent;
      ENetPeer*         mPeer;
      int               mPort;
      bool              mConnected;
      std::string       mAddress;
      bool connect(unsigned int, std::string);
};

}
#endif /* __NETWORK_H_ */

