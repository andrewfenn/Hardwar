/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright (C) 2009  Andrew Fenn
    
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

#ifndef __GAME_H_
#define __GAME_H_

#include <libintl.h>
#include <Ogre.h>

#include "ClientManager.h"
#include "ZoneManager.h"

namespace Server
{
   class Game
   {
      public:
         Game();
         ~Game();
         bool setup(Ogre::ConfigFile, ENetHost*);
         void process();
         ClientManager* getClientMgr();
         void addClient(ENetPeer* peer);
         void removeClient(ENetPeer* peer);
      private:
         ClientManager    mClientMgr;
         ZoneManager      mZoneMgr;
         Ogre::ConfigFile mConfig;
         void processBuildingReqs(Admin*);
   };
}
#endif /* __GAME_H_ */
